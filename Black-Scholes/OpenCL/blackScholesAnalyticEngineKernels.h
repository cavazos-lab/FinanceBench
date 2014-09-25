//blackScholesAnalyticEngineKernels.cuh
//Scott Grauer-Gray
//Kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH

#include <stdio.h>
#include <stdlib.h>

//needed for the structs used on the code
#include "blackScholesAnalyticEngineStructs.h"



//device kernel to retrieve the compound factor in interestRate
dataType interestRateCompoundFactor(dataType t, yieldTermStruct currYieldTermStruct);


//device kernel to retrieve the discount factor in interestRate
dataType interestRateDiscountFactor(dataType t, yieldTermStruct currYieldTermStruct);



//device function to get the variance of the black volatility function
dataType getBlackVolBlackVar(blackVolStruct volTS);


//device function to get the discount on a dividend yield
dataType getDiscountOnDividendYield(dataType yearFraction, yieldTermStruct dividendYieldTermStruct);


//device function to get the discount on the risk free rate
dataType getDiscountOnRiskFreeRate(dataType yearFraction, yieldTermStruct riskFreeRateYieldTermStruct);


//device kernel to run the error function
dataType errorFunct(normalDistStruct normDist, dataType x);


//device kernel to run the operator function in cumulative normal distribution
dataType cumNormDistOp(normalDistStruct normDist, dataType z);


//device kernel to run the gaussian function in the normal distribution
dataType gaussianFunctNormDist(normalDistStruct normDist, dataType x);


//device kernel to retrieve the derivative in a cumulative normal distribution
dataType cumNormDistDeriv(normalDistStruct normDist, dataType x);


//device function to initialize the cumulative normal distribution structure
void initCumNormDist(normalDistStruct& currCumNormDist);


//device function to initialize variable in the black calculator
void initBlackCalcVars(blackCalcStruct& blackCalculator, payoffStruct payoff);


//device function to initialize the black calculator
void initBlackCalculator(blackCalcStruct& blackCalc, payoffStruct payoff, dataType forwardPrice, dataType stdDev, dataType riskFreeDiscount);


//device function to retrieve the output resulting value
dataType getResultVal(blackCalcStruct blackCalculator);


//global function to retrieve the output value for an option
__kernel void getOutValOption(__global optionInputStruct* options, __global dataType* outputVals, int numVals);

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CUH
