//blackScholesAnalyticEngineKernels.cuh
//Scott Grauer-Gray
//Kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH

#include <stdio.h>

#include <stdlib.h>

#include <vector>
#include <algorithm>

//needed for the structs used on the code
#include "blackScholesAnalyticEngineStructs.cuh"

//constants used in this code
#define M_1_SQRTPI  0.564189583547756286948
#define M_SQRT_2    0.7071067811865475244008443621048490392848359376887


//device kernel to retrieve the compound factor in interestRate
__device__ float interestRateCompoundFactor(float t, yieldTermStruct currYieldTermStruct);

//device kernel to retrieve the discount factor in interestRate
__device__ float interestRateDiscountFactor(float t, yieldTermStruct currYieldTermStruct);


//device function to get the variance of the black volatility function
__device__ float getBlackVolBlackVar(blackVolStruct volTS);


//device function to get the discount on a dividend yield
__device__ float getDiscountOnDividendYield(float yearFraction, yieldTermStruct dividendYieldTermStruct);


//device function to get the discount on the risk free rate
__device__ float getDiscountOnRiskFreeRate(float yearFraction, yieldTermStruct riskFreeRateYieldTermStruct);

//device kernel to run the error function
__device__ float errorFunct(normalDistStruct normDist, float x);


//device kernel to run the operator function in cumulative normal distribution
__device__ float cumNormDistOp(normalDistStruct normDist, float z);


//device kernel to run the gaussian function in the normal distribution
__device__ float gaussianFunctNormDist(normalDistStruct normDist, float x);

//device kernel to retrieve the derivative in a cumulative normal distribution
__device__ float cumNormDistDeriv(normalDistStruct normDist, float x);

//device function to initialize the cumulative normal distribution structure
__device__ void initCumNormDist(normalDistStruct* currCumNormDist);


//device function to initialize variable in the black calculator
__device__ void initBlackCalcVars(blackCalcStruct* blackCalculator,  payoffStruct payoff);


//device function to initialize the black calculator
__device__ void initBlackCalculator(blackCalcStruct* blackCalc, payoffStruct payoff, float forwardPrice, float stdDev, float riskFreeDiscount);


//device function to retrieve the output resulting value
__device__ float getResultVal(blackCalcStruct* blackCalculator);


//global function to retrieve the output value for an option
__global__ void getOutValOption(optionInputStruct* options, float* outputVals, int numVals);

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH
