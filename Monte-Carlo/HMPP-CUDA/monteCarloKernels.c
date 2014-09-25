//monteCarloKernels.cu
//Scott Grauer-Gray
//May 10, 2012
//GPU Kernels for running monte carlo


#include "monteCarloKernels.h"

//function to compute the inverse normal distribution
float compInverseNormDist(float x) 
{
  float z = x - 0.5;
  float r = z*z;
  z = (((((A_1 * r + A_2) * r+A_3) * r+A_4) * r + A_5) * r + A_6) * z /
    (((((B_1 * r + B_2) * r+B_3) * r+B_4) * r + B_5) * r + 1.0);
  return z;
}

float interestRateCompoundFact(float t, float rate)
{
  //assuming "continuous" option
  return exp(rate*t);
}

float interestRateDiscountFact(float t, float rate)
{
  return 1.0/interestRateCompoundFact(t, rate);
}

float flatForwardDiscountImpl(float t, float rate)
{
  return interestRateDiscountFact(t, rate);
}

float yieldTermStructDiscount(float t, float rate)
{
  return flatForwardDiscountImpl(t, rate);
}

float interestRateImpliedRate(float compound, float t)
{
  float r = log(compound) / t;
  return r;
}


float yieldTermStructForwardRate(float t1, float t2, float rate)
{
  float compound = interestRateDiscountFact(t1, rate) / interestRateDiscountFact(t2, rate);
  return interestRateImpliedRate(compound, t2 - t1);
}

float processDrift(float t, float x, monteCarloOptionStruct optionStruct)
{
  float sigma = processDiff(t, x, optionStruct);
  float t1 = t + 0.0001;

  return yieldTermStructForwardRate(t, t1, optionStruct.riskVal) - yieldTermStructForwardRate(t, t1, optionStruct.divVal) - (0.5 * sigma * sigma);
} 

float discretizationDrift(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
  return processDrift(t0, x0, optionStruct) * dt;
}

float localVoltLocVol(float t, float underlyingLevel, monteCarloOptionStruct optionStruct)
{
  return optionStruct.voltVal;
}

float processDiff(float t, float x, monteCarloOptionStruct optionStruct)
{
  return localVoltLocVol(t, x, optionStruct);
}

float discDiff(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
  return processDiff(t0, x0, optionStruct) * sqrt(dt);
}

float stdDeviation(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
  return discDiff(t0, x0, dt, optionStruct);
}

float apply(float x0, float dx)
{
  return (x0 * exp(dx));
}

float discDrift(float t0, float x0, float dt, monteCarloOptionStruct optionStruct)
{
  return processDrift(t0, x0, optionStruct) * dt;
}

float processEvolve(float t0, float x0, float dt, float dw, monteCarloOptionStruct optionStruct)
{
  return apply(x0, discDrift(t0, x0, dt, optionStruct) + stdDeviation(t0, x0, dt, optionStruct) *dw);
}

//retrieve the current sequence
void getSequence(float* sequence, float sampleNum)
{
  size_t iInSeq;

  #pragma hmppcg noParallel
  for (iInSeq = 0; iInSeq < SEQUENCE_LENGTH; iInSeq++)
    {
      sequence [iInSeq] = DEFAULT_SEQ_VAL;
    }
}


float getProcessValX0(monteCarloOptionStruct optionStruct)
{
  return optionStruct.underlyingVal;
}

void getPath(float* path, size_t sampleNum, float dt, mt19937state* state, monteCarloOptionStruct optionStruct)
{
    float weight = DEFAULT_SEQ_WEIGHT;
    path [0] = getProcessValX0(optionStruct);
    size_t i;

    #pragma hmppcg noParallel
    for (i = 1; i < SEQUENCE_LENGTH; i++) 
    {
          float t = i * dt;
	  float randVal = generateRandFloat (&(state [sampleNum]));
          float inverseCumRandVal = compInverseNormDist (randVal); 
	  path [i] = processEvolve(t, path [i - 1], dt, inverseCumRandVal, optionStruct); 
    }
}
	

float getPrice (float val)
{
  float v = STRIKE_VAL - val;
  if (v < 0)
    return 0.0f;
  return v * DISCOUNT_VAL;
}

void initializePath(float* path)
{
  int i;

  #pragma hmppcg noParallel
  for (i = 0; i < SEQUENCE_LENGTH; i++)
    {
      path [i] = START_PATH_VAL;
    }
}

