//monteCarloKernelsCpu.cuh
//Scott Grauer-Gray
//May 10, 2012
//Headers for monte carlo function on the CPU

#ifndef MONTE_CARLO_KERNELS_CPU_CUH
#define MONTE_CARLO_KERNELS_CPU_CUH

//needed for constants related to monte carlo
//#include "monteCarloConstants.cuh"


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
