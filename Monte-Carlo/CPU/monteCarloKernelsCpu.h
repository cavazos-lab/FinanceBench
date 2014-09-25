//monteCarloKernelsCpu.cuh
//Scott Grauer-Gray
//May 10, 2012
//Headers for monte carlo function on the CPU

#ifndef MONTE_CARLO_KERNELS_CPU_CUH
#define MONTE_CARLO_KERNELS_CPU_CUH

//needed for constants related to monte carlo
#include "monteCarloConstants.h"
#include "monteCarloStructs.h"
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

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


//function to set up the random states
void setup_kernelCpu();

//function to compute the inverse normal distribution
dataType compInverseNormDistCpu(dataType x) ;

dataType interestRateCompoundFactCpu(dataType t, dataType rate);

dataType interestRateDiscountFactCpu(dataType t, dataType rate);

dataType flatForwardDiscountImplCpu(dataType t, dataType rate);

dataType yieldTermStructDiscountCpu(dataType t, dataType rate);

dataType interestRateImpliedRateCpu(dataType compound, dataType t);

dataType yieldTermStructForwardRateCpu(dataType t1, dataType t2, dataType rate);

dataType processDriftCpu(dataType t, dataType x, monteCarloOptionStruct optionStruct);

dataType discretizationDriftCpu(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);

dataType localVoltLocVolCpu(dataType t, dataType underlyingLevel, monteCarloOptionStruct optionStruct);

dataType processDiffCpu(dataType t, dataType x, monteCarloOptionStruct optionStruct);

dataType discDiffCpu(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);

dataType stdDeviationCpu(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);

dataType applyCpu(dataType x0, dataType dx);

dataType discDriftCpu(dataType t0, dataType x0, dataType dt, monteCarloOptionStruct optionStruct);

dataType processEvolveCpu(dataType t0, dataType x0, dataType dt, dataType dw, monteCarloOptionStruct optionStruct);

//retrieve the current sequence
void getSequenceCpu(dataType* sequence, dataType sampleNum);


dataType getProcessValX0Cpu(monteCarloOptionStruct optionStruct);

void getPathCpu(dataType* path, size_t sampleNum, dataType dt, int numSample, monteCarloOptionStruct optionStruct);
	

dataType getPriceCpu(dataType val);

//initialize the path
void initializePathCpu(dataType* path);

void monteCarloGpuKernelCpu(dataType* samplePrices, dataType* sampleWeights, dataType* times, dataType dt, monteCarloOptionStruct* optionStructs, int numSamples);



#endif //MONTE_CARLO_KERNELS_CPU_CUH
