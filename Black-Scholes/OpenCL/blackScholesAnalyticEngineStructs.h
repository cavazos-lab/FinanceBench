//blackScholesAnalyticEngineStructs.h
//Scott Grauer-Gray
//Structs for running black scholes using the analytic engine (from quantlib) on the GPU

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH

//define the total number of samples
#define NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC 200000

//define the thread block size
#define THREAD_BLOCK_SIZE 256

typedef float dataType;

typedef struct
{
	int day;
	int month;
	int year;
} dateStruct;


typedef struct
{
	int type;
	dataType strike;
} payoffStruct;


typedef struct
{
	dataType typeExercise;
	dataType yearFractionTime;
} exerciseStruct;

typedef struct
{
	dataType rate;
	dataType freq;
	int comp;
} interestRateStruct;

typedef struct
{
	dataType timeYearFraction;
	dataType forward;
	dataType compounding;
	dataType frequency;
	dataType intRate;
} yieldTermStruct;

typedef struct
{
	dataType timeYearFraction;
	dataType following;
	dataType volatility;
} blackVolStruct;

typedef struct
{
	dataType x0;
	yieldTermStruct dividendTS;
	yieldTermStruct riskFreeTS;
	blackVolStruct blackVolTS;
} blackScholesMertStruct;

typedef struct
{
	blackScholesMertStruct process;
	dataType tGrid;
	dataType xGrid;
	dataType dampingSteps;
	dataType schemeDesc;
	dataType localVol;
} engineStruct;


typedef struct
{
	payoffStruct payoff;
	dataType yearFractionTime;
	blackScholesMertStruct pricingEngine;
} optionStruct;

typedef struct
{
	dataType strike;
	dataType forward;
	dataType stdDev;
	dataType discount;
	dataType variance;
	dataType d1;
	dataType d2;
	dataType alpha;
	dataType beta;
	dataType DalphaDd1;
	dataType DbetaDd2;
	dataType n_d1;
	dataType cum_d1;
	dataType n_d2;
	dataType cum_d2;
	dataType x;
	dataType DxDs;
	dataType DxDstrike;
} blackCalcStruct;

typedef struct
{
	dataType average;
	dataType sigma;
	dataType denominator;
	dataType derNormalizationFactor;
	dataType normalizationFactor;
} normalDistStruct;

//define into for each type of option
#define CALL 0
#define PUT 1

typedef struct
{ 
	int type;
	dataType strike;
	dataType spot;
	dataType q;
	dataType r;
	dataType t;
	dataType vol;
	dataType value;
	dataType tol;
} optionInputStruct;

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH
