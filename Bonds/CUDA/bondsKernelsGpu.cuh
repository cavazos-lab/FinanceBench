//bondsKernelsGpu.cuh
//Scott Grauer-Gray
//Header for bonds kernels to run on the GPU w/ CUDA

#ifndef BONDS_KERNELS_GPU
#define BONDS_KERNELS_GPU

#include "bondsStructs.cuh"
#include <stdbool.h>

__device__ int monthLengthKernelGpu(int month, bool leapYear);


__device__ int monthOffsetKernelGpu(int m, bool leapYear);


__device__ int yearOffsetKernelGpu(int y);


__device__ bool isLeapKernelGpu(int y);


__device__ bondsDateStruct intializeDateKernelGpu(int d, int m, int y);


__device__ dataType yearFractionGpu(bondsDateStruct d1,
					bondsDateStruct d2, int dayCounter);


__device__ int dayCountGpu(bondsDateStruct d1, bondsDateStruct d2, int dayCounter);


__device__ dataType couponNotionalGpu();


__device__ dataType bondNotionalGpu();

__device__ dataType fixedRateCouponNominalGpu();


__device__ bool eventHasOccurredGpu(bondsDateStruct currDate, bondsDateStruct eventDate);


__device__ bool cashFlowHasOccurredGpu(bondsDateStruct refDate, bondsDateStruct eventDate);


__device__ bondsDateStruct advanceDateGpu(bondsDateStruct date, int numMonthsAdvance);

__device__ int getNumCashFlowsGpu(inArgsStruct inArgs, int bondNum);

__device__ void setCashFlowsGpu(inArgsStruct inArgs, int bondNum);


__device__ dataType getDirtyPriceGpu(inArgsStruct inArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType discountingBondEngineCalculateSettlementValueGpu(inArgsStruct inArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType bondAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType bondFunctionsAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs);






__device__ dataType cashFlowsAccruedAmountGpu(cashFlowsStruct cashFlows,
                                  bool includeSettlementDateFlows,
                                  bondsDateStruct settlementDate,
								  int numLegs, inArgsStruct inArgs, int bondNum);


__device__ dataType fixedRateCouponAccruedAmountGpu(cashFlowsStruct cashFlows, int numLeg, bondsDateStruct d, inArgsStruct inArgs, int bondNum);

	



__device__ dataType cashFlowsNpvGpu(cashFlowsStruct cashFlows,
                        bondsYieldTermStruct discountCurve,
                        bool includeSettlementDateFlows,
                        bondsDateStruct settlementDate,
                        bondsDateStruct npvDate,
						int numLegs);



__device__ dataType bondsYieldTermStructureDiscountGpu(bondsYieldTermStruct ytStruct, bondsDateStruct t);


__device__ dataType flatForwardDiscountImplGpu(intRateStruct intRate, dataType t);


__device__ dataType interestRateDiscountFactorGpu(intRateStruct intRate, dataType t);


__device__ dataType interestRateCompoundFactorGpuTwoArgs(intRateStruct intRate, dataType t);


__device__ dataType fixedRateCouponAmountGpu(cashFlowsStruct cashFlows, int numLeg);


__device__ dataType interestRateCompoundFactorGpu(intRateStruct intRate, bondsDateStruct d1,
				                           bondsDateStruct d2, int dayCounter);


__device__ dataType fixedRateBondForwardSpotIncomeGpu(inArgsStruct inArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getImpliedYieldGpu(inArgsStruct inArgs, dataType forwardValue, dataType underlyingSpotValue, dataType spotIncomeIncDiscCurve, int bondNum);


__device__ dataType interestRateImpliedRateGpu(dataType compound,                                        
                              int comp,
                              dataType freq,
                              dataType t);

							  

__device__ dataType getMarketRepoRateGpu(bondsDateStruct d,
                                   int comp,
                                   dataType freq,
								   bondsDateStruct referenceDate,
								   inArgsStruct inArgs, int bondNum);


__device__ couponStruct cashFlowsNextCashFlowGpu(cashFlowsStruct cashFlows,
                            bondsDateStruct settlementDate,
							int numLegs);

__device__ int cashFlowsNextCashFlowNumGpu(cashFlowsStruct cashFlows,
                            bondsDateStruct settlementDate,
							int numLegs);


__device__ dataType getBondYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     bondsDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
					 inArgsStruct currInArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType getBondFunctionsYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     bondsDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
					 inArgsStruct currInArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs);



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
                        bondsDateStruct settlementDate,
                        bondsDateStruct npvDate,
						int numLegs);

__device__ dataType fOpGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


__device__ dataType fDerivativeGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs);


__device__ bool closeGpu(dataType x, dataType y);


__device__ bool closeGpuThreeArgs(dataType x, dataType y, int n);


__device__ dataType enforceBoundsGpu(dataType x);


__device__ dataType solveImplGpu(solverStruct solver, irrFinderStruct f,
						dataType xAccuracy, cashFlowsStruct cashFlows, int numLegs);



__device__ dataType modifiedDurationGpu(cashFlowsStruct cashFlows,
								intRateStruct y,
								bool includeSettlementDateFlows,
								bondsDateStruct settlementDate,
								bondsDateStruct npvDate,
								int numLegs);




__device__ dataType getCashFlowsYieldGpu(cashFlowsStruct cashFlows,
                          dataType npv,
                          int dayCounter,
                          int compounding,
                          dataType frequency,
                          bool includeSettlementDateFlows,
                          bondsDateStruct settlementDate,
                          bondsDateStruct npvDate,
			  int numLegs,
			  dataType accuracy/* = 1.0e-10*/,
                          int maxIterations/* = 100*/,
			  dataType guess/* = 0.05f*/);


#endif //BONDS_KERNELS_GPU

