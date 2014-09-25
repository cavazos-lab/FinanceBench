//monteCarloKernels.cu
//Scott Grauer-Gray
//May 10, 2012
//GPU Kernels for running monte carlo

#include "monteCarloKernels.h"
#include "mt19937.h"

#define A_1 -39.696830286653757
#define A_2 220.94609842452050
#define A_3 -275.92851044696869
#define A_4 138.35775186726900
#define A_5 -30.664798066147160
#define A_6 2.5066282774592392
#define B_1 -54.476098798224058
#define B_2 161.58583685804089
#define B_3 -155.69897985988661
#define B_4 66.801311887719720
#define B_5 -13.280681552885721

__kernel void initializeMersenneStateGpu(__global mt19937state* m, unsigned long seed, int numSamples)
{
	//retrieve the thread number
	size_t numThread = get_global_id(0);

	seed += numThread;

	//while (numSample < numSamples)
	if (numThread < numSamples)
	{
  		for (int i = 0; i < __N__; i++)
    		{
      			m[numThread].mt[i] = seed & 0xffff0000;
      			seed = 69069 * seed + 1;
      			m[numThread].mt[i] |= (seed & 0xffff0000) >> 16;
   	   		seed = 69069 * seed + 1;
    		}
	
		m[numThread].mti = __N__;
	}
  
	
}


unsigned long
generateRandIntGpu (__global mt19937state* m)
{
  unsigned long y;
  unsigned long mag01 [2];
  mag01 [0] = 0x0;
  mag01 [1] = MATRIX_A;

  /* mag01[x] = x * MATRIX_A  for x=0,1 */
  
  if (m->mti >= __N__)
    { /* generate N words at one time */
      int kk = 0;
      #pragma hmppcg noParallel
      while (kk < __N__ - __M__)
	{
	  y = (m->mt [kk] & UPPER_MASK) | (m->mt [kk + 1] & LOWER_MASK);
	  m->mt [kk] = m->mt [kk + __M__] ^ (y >> 1) ^ mag01 [y & 0x1];
	  ++kk;
	}
      #pragma hmppcg noParallel
      while (kk < __N__ - 1)
	{
	  y = (m->mt [kk] & UPPER_MASK) | (m->mt [kk + 1] & LOWER_MASK);
	  m->mt [kk] = m->mt [kk + (__M__ - __N__)] ^ (y >> 1) ^ mag01 [y & 0x1];
	  ++kk;
	}
      
      y = (m->mt [__N__ - 1] & UPPER_MASK) | (m->mt [0] & LOWER_MASK);
      m->mt [__N__ - 1] = m->mt [__M__ - 1] ^ (y >> 1) ^ mag01 [y & 0x1];
      
      m->mti = 0;
    }
  
  y = m->mt [m->mti++];
  y ^= TEMPERING_SHIFT_U(y);
  y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
  y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
  y ^= TEMPERING_SHIFT_L(y);
  return y;
}

float
generateRandFloatGpu (__global mt19937state* m)
{ 
  return ( ((float) generateRandIntGpu (m)) * 2.3283064370807974e-10 );
}
/*
float
generateRandDoubleGpu (__global mt19937state* m)
{  
  return ( ((double) generateRandIntGpu (m)) * 2.3283064370807974e-10 );
}*/


//function to compute the inverse normal distribution
dataType compInverseNormDist(dataType x) 
{
	dataType z;
	//if (x < x_low_ || x_high_ < x) {
	//z = tail_value(x);
	//} else {
                
	z = x - 0.5;
    dataType r = z*z;
    z = (((((A_1*r+A_2)*r+A_3)*r+A_4)*r+A_5)*r+A_6)*z /
                    (((((B_1*r+B_2)*r+B_3)*r+B_4)*r+B_5)*r+1.0);
    //}
	return z;
}

dataType interestRateCompoundFact(dataType t, dataType rate)
{
	//assuming "continuous" option
	return exp(rate*t);
}

dataType interestRateDiscountFact(dataType t, dataType rate)
{
	return 1.0/interestRateCompoundFact(t, rate);
}

dataType flatForwardDiscountImpl(dataType t, dataType rate)
{
	return interestRateDiscountFact(t, rate);
}

dataType yieldTermStructDiscount(dataType t, dataType rate)
{
	return flatForwardDiscountImpl(t, rate);
}

dataType interestRateImpliedRate(dataType compound, dataType t)
{
	dataType r = log(compound) / t;
	return r;
}


dataType yieldTermStructForwardRate(dataType t1, dataType t2, dataType rate)
{
	dataType compound = interestRateDiscountFact(t1, rate) / interestRateDiscountFact(t2, rate);
	return interestRateImpliedRate(compound, t2 - t1);
}

dataType processDrift(dataType t, dataType x, monteCarloOptionStruct optionStruct)
{
	dataType sigma = processDiff(t, x, optionStruct);
	dataType t1 = t + 0.0001;

	return yieldTermStructForwardRate(t, t1, optionStruct.riskVal) - yieldTermStructForwardRate(t, t1, optionStruct.divVal) - (0.5 * sigma * sigma);
} 

dataType discretizationDrift(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct)
{
	return processDrift(t0, x0, optionStruct) * dt;
}

dataType localVoltLocVol(dataType t, dataType underlyingLevel, monteCarloOptionStruct optionStruct)
{
	return optionStruct.voltVal;
}

dataType processDiff(dataType t, dataType x, monteCarloOptionStruct optionStruct)
{
	return localVoltLocVol(t, x, optionStruct);
}

dataType discDiff(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct)
{
	return processDiff(t0, x0, optionStruct) * sqrt(dt);
}

dataType stdDeviation(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct)
{
	return discDiff(t0, x0, dt, optionStruct);
}

dataType apply(dataType x0, dataType dx)
{
	return (x0 * exp(dx));
}

dataType discDrift(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct)
{
	return processDrift(t0, x0, optionStruct) * dt;
}

dataType processEvolve(dataType t0, dataType x0, dataType dt, dataType dw, monteCarloOptionStruct optionStruct)
{
	return apply(x0, discDrift(t0, x0, dt, optionStruct) + stdDeviation(t0, x0, dt, optionStruct) *dw);
}

//retrieve the current sequence
void getSequence(dataType* sequence, dataType sampleNum)
{
		for (size_t iInSeq = 0; iInSeq < SEQUENCE_LENGTH; iInSeq++)
		{
			sequence[iInSeq] = DEFAULT_SEQ_VAL;
		}
}


dataType getProcessValX0(monteCarloOptionStruct optionStruct)
{
	return optionStruct.underlyingVal;
}


void getPath(dataType* path, size_t sampleNum, dataType dt, __global mt19937state* state, monteCarloOptionStruct optionStruct)
{
        dataType weight = DEFAULT_SEQ_WEIGHT;

        path[0] = getProcessValX0(optionStruct);

        for (size_t i=1; i<SEQUENCE_LENGTH; i++) 
	{
            dataType t = i*dt; 
	    dataType randVal = generateRandFloatGpu(&(state [sampleNum])); ;
	    dataType inverseCumRandVal = compInverseNormDist(randVal); 
            path[i] = processEvolve(t, path[i-1], dt, inverseCumRandVal, optionStruct); 
        }
}
	

dataType getPrice(dataType val)
{
	return MAX(STRIKE_VAL-val, 0.0)* DISCOUNT_VAL;
}

//initialize the path
void initializePath(dataType* path)
{
	for (int i=0; i < SEQUENCE_LENGTH; i++)
	{
		path[i] = START_PATH_VAL;
	}
}


__kernel void monteCarloGpuKernel(__global dataType* samplePrices, __global dataType* sampleWeights, __global dataType* times, dataType dt, __global mt19937state* randStates, __global monteCarloOptionStruct* optionStructs, int numSamples)
{
	//retrieve the thread number
	size_t numThread = get_global_id(0);

	//retrieve the option number
	int numOption = 0;

	//retrieve the number of sample
	int numSample = numThread;

	size_t outputNum = numSample;

	//while (numSample < numSamples)
	if (numSample < numSamples)
	{
		//declare and initialize the path
		dataType path[SEQUENCE_LENGTH];
		initializePath(path);

		getPath(path, numSample, dt, randStates, optionStructs[numOption]);
		dataType price = getPrice(path[SEQUENCE_LENGTH-1]);
	
		samplePrices[outputNum] = price;
		sampleWeights[outputNum] = DEFAULT_SEQ_WEIGHT;

		//increase the sample and output number if processing multiple samples per thread
	//	numSample += NUM_THREADS_PER_OPTION;
	//	outputNum += NUM_THREADS_PER_OPTION;
	}
}
