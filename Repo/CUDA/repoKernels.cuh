//repoKernels.cuh
//Scott Grauer-Gray
//Headers for kernels for running repo on the GPU

#ifndef REPO_KERNELS_CUH
#define REPO_KERNELS_CUH

#include "repoStructs.cuh"

__device__ int monthLengthKernelGpu(int month, bool leapYear);

__device__ int monthOffsetKernelGpu(int m, bool leapYear);


__device__ int yearOffsetKernelGpu(int y);


__device__ bool isLeapKernelGpu(int y);


__device__ repoDateStruct intializeDateKernelGpu(int d, int m, int y);

__device__ dataType yearFractionGpu(repoDateStruct d1,
					repoDateStruct d2, int dayCounter);

__device__ int dayCountGpu(repoDateStruct d1, repoDateStruct d2, int dayCounter);


__device__ dataType couponNotionalGpu();

__device__ dataType bondNotionalGpu();


__device__ dataType fixedRateCouponNominalGpu();

__device__ bool eventHasOccurredGpu(repoDateStruct currDate, repoDateStruct eventDate);


__device__ bool cashFlowHasOccurredGpu(repoDateStruct refDate, repoDateStruct eventDate);



__device__ repoDateStruct advanceDateGpu(repoDateStruct date, int numMonthsAdvance);

__device__ int getNumCashFlowsGpu(inArgsStruct inArgs, int repoNum);


__device__ dataType getDirtyPriceGpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);

__device__ dataType getAccruedAmountGpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType discountingBondEngineCalculateSettlementValueGpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType bondAccruedAmountGpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType bondFunctionsAccruedAmountGpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs) ;


__device__ dataType cashFlowsAccruedAmountGpu(cashFlowsStruct cashFlows,
                                  bool includeSettlementDateFlows,
                                  repoDateStruct settlementDate,
								  int numLegs, inArgsStruct inArgs, int repoNum);


__device__ dataType fixedRateCouponAccruedAmountGpu(cashFlowsStruct cashFlows, int numLeg, repoDateStruct d, inArgsStruct inArgs, int repoNum) ;

	



__device__ dataType cashFlowsNpvGpu(cashFlowsStruct cashFlows,
                        repoYieldTermStruct discountCurve,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
						int numLegs);




__device__ dataType repoYieldTermStructureDiscountGpu(repoYieldTermStruct ytStruct, repoDateStruct t);

__device__ dataType flatForwardDiscountImplGpu(intRateStruct intRate, dataType t) ;



__device__ dataType interestRateDiscountFactorGpu(intRateStruct intRate, dataType t);

__device__ dataType interestRateCompoundFactorGpu(intRateStruct intRate, dataType t);


__device__ dataType fixedRateCouponAmountGpu(cashFlowsStruct cashFlows, int numLeg);

__device__ dataType interestRateCompoundFactorFourArgsGpu(intRateStruct intRate, repoDateStruct d1,
				                           repoDateStruct d2, int dayCounter);



__device__ dataType fixedRateBondForwardSpotIncomeGpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getImpliedYieldGpu(inArgsStruct inArgs, dataType forwardValue, dataType underlyingSpotValue, dataType spotIncomeIncDiscCurve, int repoNum);


__device__ dataType interestRateImpliedRateGpu(dataType compound,                                        
                              int comp,
                              dataType freq,
                              dataType t);


__device__ dataType getMarketRepoRateGpu(repoDateStruct d,
                                   int comp,
                                   dataType freq,
								   repoDateStruct referenceDate,
								   inArgsStruct inArgs, int repoNum);


__device__ couponStruct cashFlowsNextCashFlowGpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs);


__device__ int cashFlowsNextCashFlowNumGpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs);


__device__ dataType getBondYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
		     inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getBondFunctionsYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
					 inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getCashFlowsYieldGpu(cashFlowsStruct leg,
                          dataType npv,
                          int dayCounter,
                          int compounding,
                          dataType frequency,
                          bool includeSettlementDateFlows,
                          repoDateStruct settlementDate,
                          repoDateStruct npvDate,
						  int numLegs,
                          dataType accuracy,
                          int maxIterations,
                          dataType guess);

__device__ dataType solverSolveGpu(solverStruct solver,
					irrFinderStruct f,
					dataType accuracy,
					dataType guess,
					dataType step,
					cashFlowsStruct cashFlows,
					int numLegs);


__device__ dataType cashFlowsNpvYieldGpu(cashFlowsStruct cashFlows,
                        intRateStruct y,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
						int numLegs);

__device__ dataType fOpGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType fDerivativeGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


__device__ bool closeGpu(dataType x, dataType y);

__device__ bool closeThreeArgsGpu(dataType x, dataType y, int n);

__device__ dataType enforceBoundsGpu(dataType x);


__device__ dataType solveImplGpu(solverStruct solver, irrFinderStruct f,
				dataType xAccuracy, cashFlowsStruct cashFlows, int numLegs);




__device__ dataType modifiedDurationGpu(cashFlowsStruct cashFlows,
								intRateStruct y,
								bool includeSettlementDateFlows,
								repoDateStruct settlementDate,
								repoDateStruct npvDate,
								int numLegs);


#endif //REPO_KERNELS_CUH
