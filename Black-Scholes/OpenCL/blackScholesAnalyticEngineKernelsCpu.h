//blackScholesAnalyticEngineKernelsCpu.cuh
//Scott Grauer-Gray
//Declarations of kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH

#include "blackScholesAnalyticEngineStructs.h"

//needed for the constants in the error function
#include "errorFunctConsts.h"


//device kernel to retrieve the compound factor in interestRate
dataType interestRateCompoundFactorCpu(dataType t, yieldTermStruct currYieldTermStruct);


//device kernel to retrieve the discount factor in interestRate
dataType interestRateDiscountFactorCpu(dataType t, yieldTermStruct currYieldTermStruct);


//device function to get the variance of the black volatility function
dataType getBlackVolBlackVarCpu(blackVolStruct volTS);


//device function to get the discount on a dividend yield
dataType getDiscountOnDividendYieldCpu(dataType yearFraction, yieldTermStruct dividendYieldTermStruct);


//device function to get the discount on the risk free rate
dataType getDiscountOnRiskFreeRateCpu(dataType yearFraction, yieldTermStruct riskFreeRateYieldTermStruct);


//device kernel to run the error function
dataType errorFunctCpu(normalDistStruct normDist, dataType x);


//device kernel to run the operator function in cumulative normal distribution
dataType cumNormDistOpCpu(normalDistStruct normDist, dataType z);


//device kernel to run the gaussian function in the normal distribution
dataType gaussianFunctNormDistCpu(normalDistStruct normDist, dataType x);


//device kernel to retrieve the derivative in a cumulative normal distribution
dataType cumNormDistDerivCpu(normalDistStruct normDist, dataType x);


//device function to initialize the cumulative normal distribution structure
void initCumNormDistCpu(normalDistStruct& currCumNormDist);


//device function to initialize variable in the black calculator
void initBlackCalcVarsCpu(blackCalcStruct& blackCalculator, payoffStruct payoff);


//device function to initialize the black calculator
void initBlackCalculatorCpu(blackCalcStruct& blackCalc, payoffStruct payoff, dataType forwardPrice, dataType stdDev, dataType riskFreeDiscount);


//device function to retrieve the output resulting value
dataType getResultValCpu(blackCalcStruct blackCalculator);


//global function to retrieve the output value for an option
void getOutValOptionCpu(optionInputStruct* options, dataType* outputVals, int numVals);

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CUH
