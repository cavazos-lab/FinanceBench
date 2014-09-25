//blackScholesAnalyticEngineKernelsCpu.cuh
//Scott Grauer-Gray
//Declarations of kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH


//needed for the constants in the error function
#include "errorFunctConsts.cuh"


//device kernel to retrieve the compound factor in interestRate
float interestRateCompoundFactorCpu(float t, yieldTermStruct currYieldTermStruct);


//device kernel to retrieve the discount factor in interestRate
float interestRateDiscountFactorCpu(float t, yieldTermStruct currYieldTermStruct);


//device function to get the variance of the black volatility function
float getBlackVolBlackVarCpu(blackVolStruct volTS);


//device function to get the discount on a dividend yield
float getDiscountOnDividendYieldCpu(float yearFraction, yieldTermStruct dividendYieldTermStruct);


//device function to get the discount on the risk free rate
float getDiscountOnRiskFreeRateCpu(float yearFraction, yieldTermStruct riskFreeRateYieldTermStruct);


//device kernel to run the error function
float errorFunctCpu(normalDistStruct normDist, float x);


//device kernel to run the operator function in cumulative normal distribution
float cumNormDistOpCpu(normalDistStruct normDist, float z);


//device kernel to run the gaussian function in the normal distribution
float gaussianFunctNormDistCpu(normalDistStruct normDist, float x);


//device kernel to retrieve the derivative in a cumulative normal distribution
float cumNormDistDerivCpu(normalDistStruct normDist, float x);


//device function to initialize the cumulative normal distribution structure
void initCumNormDistCpu(normalDistStruct& currCumNormDist);


//device function to initialize variable in the black calculator
void initBlackCalcVarsCpu(blackCalcStruct& blackCalculator, payoffStruct payoff);


//device function to initialize the black calculator
void initBlackCalculatorCpu(blackCalcStruct& blackCalc, payoffStruct payoff, float forwardPrice, float stdDev, float riskFreeDiscount);


//device function to retrieve the output resulting value
float getResultValCpu(blackCalcStruct blackCalculator);


//global function to retrieve the output value for an option
void getOutValOptionCpu(optionInputStruct* options, float* outputVals, int numVals);

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH
