//repoKernelsCpu.cuh
//Scott Grauer-Gray
//July 6, 2012
//Header for repo kernels on the CPU

#ifndef REPO_KERNELS_CPU
#define REPO_KERNELS_CPU

#include "repoStructs.h"

int monthLengthKernelCpu(int month, bool leapYear);


int monthOffsetKernelCpu(int m, bool leapYear);


int yearOffsetKernelCpu(int y);


bool isLeapKernelCpu(int y);


repoDateStruct intializeDateKernelCpu(int d, int m, int y);


dataType yearFractionCpu(repoDateStruct d1,
					repoDateStruct d2, int dayCounter);


int dayCountCpu(repoDateStruct d1, repoDateStruct d2, int dayCounter);


dataType couponNotionalCpu();


dataType bondNotionalCpu();


dataType fixedRateCouponNominalCpu();


bool eventHasOccurredCpu(repoDateStruct currDate, repoDateStruct eventDate);


bool cashFlowHasOccurredCpu(repoDateStruct refDate, repoDateStruct eventDate);


repoDateStruct advanceDateCpu(repoDateStruct date, int numMonthsAdvance);

int getNumCashFlowsCpu(inArgsStruct inArgs, int repoNum);

void setCashFlowsCpu(inArgsStruct inArgs, int repoNum);


void getRepoResultsGpuCpu(inArgsStruct inArgs, resultsStruct results, int totNumRuns);


dataType getDirtyPriceCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType getAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType discountingBondEngineCalculateSettlementValueCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType bondAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType bondFunctionsAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType cashFlowsAccruedAmountCpu(cashFlowsStruct cashFlows,
                                  bool includeSettlementDateFlows,
                                  repoDateStruct settlementDate,
				  int numLegs, inArgsStruct inArgs, int repoNum);


dataType fixedRateCouponAccruedAmountCpu(cashFlowsStruct cashFlows, int numLeg, repoDateStruct d, inArgsStruct inArgs, int repoNum);

	



dataType cashFlowsNpvCpu(cashFlowsStruct cashFlows,
                        repoYieldTermStruct discountCurve,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
			int numLegs);



dataType repoYieldTermStructureDiscountCpu(repoYieldTermStruct ytStruct, repoDateStruct t);


dataType flatForwardDiscountImplCpu(intRateStruct intRate, dataType t);


dataType interestRateDiscountFactorCpu(intRateStruct intRate, dataType t);


dataType interestRateCompoundFactorCpu(intRateStruct intRate, dataType t);


dataType fixedRateCouponAmountCpu(cashFlowsStruct cashFlows, int numLeg);


dataType interestRateCompoundFactorCpu(intRateStruct intRate, repoDateStruct d1,
				                           repoDateStruct d2, int dayCounter);


dataType fixedRateBondForwardSpotIncomeCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType getImpliedYieldCpu(inArgsStruct inArgs, dataType forwardValue, dataType underlyingSpotValue, dataType spotIncomeIncDiscCurve, int repoNum);


dataType interestRateImpliedRateCpu(dataType compound,                                        
                              int comp,
                              dataType freq,
                              dataType t);

							  

dataType getMarketRepoRateCpu(repoDateStruct d,
                              int comp,
                              dataType freq,
			      repoDateStruct referenceDate,
			      inArgsStruct inArgs, int repoNum);


couponStruct cashFlowsNextCashFlowCpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs);

int cashFlowsNextCashFlowNumCpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs);


dataType getBondYieldCpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
	  	     inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


dataType getBondFunctionsYieldCpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
		     inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);



dataType solverSolveCpu(solverStruct solver,
					irrFinderStruct f,
					dataType accuracy,
					dataType guess,
					dataType step,
					cashFlowsStruct cashFlows,
					int numLegs);


dataType cashFlowsNpvYieldCpu(cashFlowsStruct cashFlows,
                        intRateStruct y,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
			int numLegs);


dataType fOpCpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


dataType fDerivativeCpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


bool closeCpu(dataType x, dataType y);


bool closeCpu(dataType x, dataType y, int n);


dataType enforceBoundsCpu(dataType x);


dataType solveImplCpu(solverStruct solver, irrFinderStruct f,
						dataType xAccuracy, cashFlowsStruct cashFlows, int numLegs);



dataType modifiedDurationCpu(cashFlowsStruct cashFlows,
								intRateStruct y,
								bool includeSettlementDateFlows,
								repoDateStruct settlementDate,
								repoDateStruct npvDate,
								int numLegs);




dataType getCashFlowsYieldCpu(cashFlowsStruct cashFlows,
                          dataType npv,
                          int dayCounter,
                          int compounding,
                          dataType frequency,
                          bool includeSettlementDateFlows,
                          repoDateStruct settlementDate,
                          repoDateStruct npvDate,
			  int numLegs,
                          dataType accuracy = 1.0e-10,
                          int maxIterations = 100,
                          dataType guess = 0.05f);


#endif //REPO_KERNELS_CPU
