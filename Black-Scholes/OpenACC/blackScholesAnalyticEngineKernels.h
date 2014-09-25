//blackScholesAnalyticEngineKernelsCpu.cuh
//Scott Grauer-Gray
//Declarations of kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_H
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_H

//needed for the constants in the error function
#include "ErrorFunctConstants.h"

//device kernel to retrieve the compound factor in interestRate
float
interestRateCompoundFactor (float t, const yieldTermStruct currYieldTermStruct);

//device kernel to retrieve the discount factor in interestRate
float
interestRateDiscountFactor (float t, const yieldTermStruct currYieldTermStruct);

//device function to get the variance of the black volatility function
float
getBlackVolBlackVar (const blackVolStruct volTS);

//device function to get the discount on a dividend yield
float
getDiscountOnDividendYield (float yearFraction, const yieldTermStruct dividendYieldTermStruct);

//device function to get the discount on the risk free rate
float
getDiscountOnRiskFreeRate (float yearFraction, const yieldTermStruct riskFreeRateYieldTermStruct);

//device kernel to run the error function
float
errorFunct (const normalDistStruct normDist, float x);

//device kernel to run the operator function in cumulative normal distribution
float
cumNormDistOp (const normalDistStruct normDist, float z);

//device kernel to run the gaussian function in the normal distribution
float
gaussianFunctNormDist (const normalDistStruct normDist, float x);

//device kernel to retrieve the derivative in a cumulative normal distribution
float
cumNormDistDeriv (const normalDistStruct normDist, float x);

//device function to initialize the cumulative normal distribution structure
void
initCumNormDist (normalDistStruct* currCumNormDist);

//device function to initialize variable in the black calculator
void
initBlackCalcVars (blackCalcStruct* blackCalculator, payoffStruct payoff);

//device function to initialize the black calculator
blackCalcStruct
initBlackCalculator (payoffStruct payoff, float forwardPrice, float stdDev, float riskFreeDiscount);

//device function to retrieve the output resulting value
float
getResultVal (blackCalcStruct blackCalculator);

//global function to retrieve the output value for an option
void
getOutValOption (optionInputStruct* options, float* outputVals, int optionNum);

#endif
