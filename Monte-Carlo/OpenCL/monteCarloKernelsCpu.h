//monteCarloKernelsCpu.cuh
//Scott Grauer-Gray
//May 10, 2012
//Headers for monte carlo function on the CPU

#ifndef MONTE_CARLO_KERNELS_CPU_CUH
#define MONTE_CARLO_KERNELS_CPU_CUH

//needed for constants related to monte carlo
#include "monteCarloConstants.h"


//function to set up the random states
void setup_kernelCpu();

//function to compute the inverse normal distribution
float compInverseNormDistCpu(float x) ;

float interestRateCompoundFactCpu(float t, float rate);

float interestRateDiscountFactCpu(float t, float rate);

float flatForwardDiscountImplCpu(float t, float rate);

float yieldTermStructDiscountCpu(float t, float rate);

float interestRateImpliedRateCpu(float compound, float t);

float yieldTermStructForwardRateCpu(float t1, float t2, float rate);

float processDriftCpu(float t, float x, monteCarloOptionStruct optionStruct);

float discretizationDriftCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float localVoltLocVolCpu(float t, float underlyingLevel, monteCarloOptionStruct optionStruct);

float processDiffCpu(float t, float x, monteCarloOptionStruct optionStruct);

float discDiffCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float stdDeviationCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float applyCpu(float x0, float dx);

float discDriftCpu(float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float processEvolveCpu(float t0, float x0, float dt, float dw, monteCarloOptionStruct optionStruct);

//retrieve the current sequence
void getSequenceCpu(float* sequence, float sampleNum);


float getProcessValX0Cpu(monteCarloOptionStruct optionStruct);

void getPathCpu(float* path, size_t sampleNum, float dt, int numSample, monteCarloOptionStruct optionStruct);
	

float getPriceCpu(float val);

//initialize the path
void initializePathCpu(float* path);

void monteCarloGpuKernelCpu(float* samplePrices, float* sampleWeights, float* times, float dt, monteCarloOptionStruct* optionStructs, int numSamples);



#endif //MONTE_CARLO_KERNELS_CPU_CUH
