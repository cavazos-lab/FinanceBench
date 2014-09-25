//monteCarloKernels.cuh
//Scott Grauer-Gray
//May 10, 2012
//Kernel headers for running monte carlo on the GPU

#ifndef MONTE_CARLO_KERNELS_CUH
#define MONTE_CARLO_KERNELS_CUH

//needed for constants related to monte carlo
#include "monteCarloConstants.h"

#include "mt19937.h"

//needed for structs related to monte carlo
#include "monteCarloStructs.h"


//function to compute the inverse normal distribution
dataType compInverseNormDist(dataType x) ;


dataType interestRateCompoundFact(dataType t, dataType rate);


dataType interestRateDiscountFact(dataType t, dataType rate);


dataType flatForwardDiscountImpl(dataType t, dataType rate);


dataType yieldTermStructDiscount(dataType t, dataType rate);


dataType interestRateImpliedRate(dataType compound, dataType t);


dataType yieldTermStructForwardRate(dataType t1, dataType t2, dataType rate);


dataType processDrift(dataType t, dataType x, monteCarloOptionStruct optionStruct);


dataType discretizationDrift(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);


dataType localVoltLocVol(dataType t, dataType underlyingLevel, monteCarloOptionStruct optionStruct);


dataType processDiff(dataType t, dataType x, monteCarloOptionStruct optionStruct);


dataType discDiff(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);


dataType stdDeviation(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);


dataType apply(dataType x0, dataType dx);


dataType discDrift(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);


dataType processEvolve(dataType t0, dataType x0, dataType dt, dataType dw, monteCarloOptionStruct optionStruct);


//retrieve the current sequence
void getSequence(dataType* sequence, dataType sampleNum);


dataType getProcessValX0(monteCarloOptionStruct optionStruct);


void getPath(dataType* path, size_t sampleNum, dataType dt, __global mt19937state* state, monteCarloOptionStruct optionStruct);
	

dataType getPrice(dataType val);


//initialize the path
void initializePath(dataType* path);


__kernel void monteCarloGpuKernel(__global dataType* samplePrices, __global dataType* sampleWeights, __global dataType* times, dataType dt, __global mt19937state* randStates, __global monteCarloOptionStruct* optionStructs, int numSamples);




#endif //MONTE_CARLO_KERNELS_CUH
