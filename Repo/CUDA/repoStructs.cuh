//repoStructs.cuh
//Scott Grauer-Gray
//July 6, 2012
//Structs for running the repo engine

#ifndef REPO_STRUCTS_CUH
#define REPO_STRUCTS_CUH

typedef double dataType;

#include <cstdlib>
#include <cmath>

#define SIMPLE_INTEREST 0
#define COMPOUNDED_INTEREST 1
#define CONTINUOUS_INTEREST 2
#define SIMPLE_THEN_COMPOUNDED_INTEREST 3

#define ANNUAL_FREQ 1
#define SEMIANNUAL_FREQ 2

#define USE_EXACT_DAY 0
#define USE_SERIAL_NUMS 1

#define QL_EPSILON_GPU 0.000000000000000001f
 
#define COMPUTE_AMOUNT -1


#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define ACCURACY 1.0e-8


typedef struct
{
	int month;
	int day;
	int year;
	int dateSerialNum;
} repoDateStruct;


typedef struct
{
	repoDateStruct startDate;
	repoDateStruct maturityDate;
	float rate;
} bondStruct;


typedef struct
{
	dataType rate;
	dataType freq;
	int comp;
	int dayCounter;
} intRateStruct;


typedef struct
{
	dataType forward;
	dataType compounding;
	dataType frequency;
	intRateStruct intRate;
	repoDateStruct refDate;
	repoDateStruct calDate;
	int dayCounter;
} repoYieldTermStruct;


typedef struct
{
	repoDateStruct paymentDate;
	repoDateStruct accrualStartDate;
	repoDateStruct accrualEndDate;
	dataType amount;	
} couponStruct;


typedef struct
{
	couponStruct* legs;
	intRateStruct intRate;
	int nominal;
	int dayCounter;
} cashFlowsStruct;


typedef struct
{
	dataType* dirtyPrice;
	dataType* accruedAmountSettlement;
	dataType* accruedAmountDeliveryDate;
	dataType* cleanPrice;
	dataType* forwardSpotIncome;
	dataType* underlyingBondFwd;
	dataType* repoNpv;
	dataType* repoCleanForwardPrice;
	dataType* repoDirtyForwardPrice;
	dataType* repoImpliedYield;
	dataType* marketRepoRate;
	dataType* bondForwardVal;
} resultsStruct;


typedef struct
{
	repoYieldTermStruct* discountCurve;
	repoYieldTermStruct* repoCurve;
	repoDateStruct* settlementDate;
	repoDateStruct* deliveryDate;
	repoDateStruct* maturityDate;
	repoDateStruct* repoDeliveryDate;
	dataType* bondCleanPrice;
	bondStruct* bond;
	dataType* dummyStrike;
} inArgsStruct;


typedef struct
{
	dataType npv;
	int dayCounter;
	int comp;
	dataType freq;
	bool includeSettlementDateFlows;
	repoDateStruct settlementDate;
	repoDateStruct npvDate;
} irrFinderStruct;


typedef struct
{
	dataType root_;
	dataType xMin_;
	dataType xMax_;
	dataType fxMin_;
	dataType fxMax_;
	int maxEvaluations_;
	int evaluationNumber_;
	dataType lowerBound_;
	dataType upperBound_;
	bool lowerBoundEnforced_;
	bool upperBoundEnforced_;
} solverStruct;



#endif //REPO_STRUCTS_CUH
