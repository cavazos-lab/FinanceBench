//monteCarloKernelsCpu.cu
//Scott Grauer-Gray
//May 10, 2012
//monte carlo kernels run on the CPU


#include "monteCarloKernelsCpu.h"


//function to set up the random states
void setup_kernelCpu( )
{

	srand(time(NULL));
}


//function to compute the inverse normal distribution
float compInverseNormDistCpu(float x) 
{
	float z;
	//if (x < x_low_ || x_high_ < x) {
	//z = tail_value(x);
	//} else {
                
	z = x - 0.5;
    	float r = z*z;
    	z = (((((A_1*r+A_2)*r+A_3)*r+A_4)*r+A_5)*r+A_6)*z /
                    (((((B_1*r+B_2)*r+B_3)*r+B_4)*r+B_5)*r+1.0);

	return z;
}

float interestRateCompoundFactCpu(float t, float rate)
{
	//assuming "continuous" option
	return exp(rate*t);
}

float interestRateDiscountFactCpu(float t, float rate)
{
	return 1.0/interestRateCompoundFactCpu(t, rate);
}

float flatForwardDiscountImplCpu(float t, float rate)
{
	return interestRateDiscountFactCpu(t, rate);
}

float yieldTermStructDiscountCpu(float t, float rate)
{
	return flatForwardDiscountImplCpu(t, rate);
}

float interestRateImpliedRateCpu(float compound, float t)
{
	float r = log(compound) / t;
	return r;
}


float yieldTermStructForwardRateCpu(float t1, float t2, float rate)
{
	float compound = interestRateDiscountFactCpu(t1, rate) / interestRateDiscountFactCpu(t2, rate);
	return interestRateImpliedRateCpu(compound, t2 - t1);
}

float processDriftCpu(float t, float x, monteCarloOptionStruct optionStruct)
{
	float sigma = processDiffCpu(t, x, optionStruct);
	float t1 = t + 0.0001;

	return yieldTermStructForwardRateCpu(t, t1, optionStruct.riskVal) - yieldTermStructForwardRateCpu(t, t1, optionStruct.divVal) - (0.5 * sigma * sigma);
} 

float discretizationDriftCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
	return processDriftCpu(t0, x0, optionStruct) * dt;
}

float localVoltLocVolCpu(float t, float underlyingLevel, monteCarloOptionStruct optionStruct)
{
	return optionStruct.voltVal;
}

float processDiffCpu(float t, float x, monteCarloOptionStruct optionStruct)
{
	return localVoltLocVolCpu(t, x, optionStruct);
}

float discDiffCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
	return processDiffCpu(t0, x0, optionStruct) * sqrt(dt);
}

float stdDeviationCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
	return discDiffCpu(t0, x0, dt, optionStruct);
}

float applyCpu(float x0, float dx)
{
	return (x0 * exp(dx));
}

float discDriftCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
	return processDriftCpu(t0, x0, optionStruct) * dt;
}

float processEvolveCpu(float t0, float x0, float dt, float dw, monteCarloOptionStruct optionStruct)
{
	return applyCpu(x0, discDriftCpu(t0, x0, dt, optionStruct) + stdDeviationCpu(t0, x0, dt, optionStruct) *dw);
}

//retrieve the current sequence
void getSequenceCpu(float* sequence, float sampleNum)
{
	for (size_t iInSeq = 0; iInSeq < SEQUENCE_LENGTH; iInSeq++)
	{
		sequence[iInSeq] = DEFAULT_SEQ_VAL;
	}
}


float getProcessValX0Cpu(monteCarloOptionStruct optionStruct)
{
	return optionStruct.underlyingVal;
}


void getPathCpu(float* path, size_t sampleNum, float dt, monteCarloOptionStruct optionStruct)
{
        path[0] = getProcessValX0Cpu(optionStruct);

        for (size_t i=1; i<SEQUENCE_LENGTH; i++) 
	{
            	float t = i*dt; 
		float randVal = ((float)rand()) / ((float)RAND_MAX);
		float inverseCumRandVal = compInverseNormDistCpu(randVal); 
            	path[i] = processEvolveCpu(t, path[i-1], dt, inverseCumRandVal, optionStruct); 
        }
}
	

float getPriceCpu(float val)
{
	return fmax(STRIKE_VAL-val, 0.0)* DISCOUNT_VAL;
}

//initialize the path
void initializePathCpu(float* path)
{
	for (int i=0; i < SEQUENCE_LENGTH; i++)
	{
		path[i] = START_PATH_VAL;
	}
}

void monteCarloGpuKernelCpu(float* samplePrices, float* sampleWeights, float* times, float dt, monteCarloOptionStruct* optionStructs, int numSamples)
{
	for (size_t numSample = 0; numSample < numSamples; numSample++)
	{
		//declare and initialize the path
		float path[SEQUENCE_LENGTH];
		initializePathCpu(path);

		int optionStructNum = 0;

		getPathCpu(path, numSample, dt, optionStructs[optionStructNum]);
		float price = getPriceCpu(path[SEQUENCE_LENGTH-1]);
	
		samplePrices[numSample] = price;
		sampleWeights[numSample] = DEFAULT_SEQ_WEIGHT;
	}
}
