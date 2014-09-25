//blackScholesAnalyticEngineStructs.cuh
//Scott Grauer-Gray
//Structs for running black scholes using the analytic engine (from quantlib) on the GPU

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH
#define BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH

//define the total number of samples
#define NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC 200000

//define the thread block size
#define THREAD_BLOCK_SIZE 256

typedef struct
{
	int day;
	int month;
	int year;
} dateStruct;


typedef struct
{
	int type;
	float strike;
} payoffStruct;


typedef struct
{
	float typeExercise;
	float yearFractionTime;
} exerciseStruct;

typedef struct
{
	float rate;
	float freq;
	int comp;
} interestRateStruct;

typedef struct
{
	float timeYearFraction;
	float forward;
	float compounding;
	float frequency;
	float intRate;
} yieldTermStruct;

typedef struct
{
	float timeYearFraction;
	float following;
	float volatility;
} blackVolStruct;

typedef struct
{
	float x0;
	yieldTermStruct dividendTS;
	yieldTermStruct riskFreeTS;
	blackVolStruct blackVolTS;
} blackScholesMertStruct;

typedef struct
{
	blackScholesMertStruct process;
	float tGrid;
	float xGrid;
	float dampingSteps;
	float schemeDesc;
	float localVol;
} engineStruct;


typedef struct
{
	payoffStruct payoff;
	float yearFractionTime;
	blackScholesMertStruct pricingEngine;
} optionStruct;

typedef struct
{
	float strike;
	float forward;
	float stdDev;
	float discount;
	float variance;
	float d1;
	float d2;
	float alpha;
	float beta;
	float DalphaDd1;
	float DbetaDd2;
	float n_d1;
	float cum_d1;
	float n_d2;
	float cum_d2;
	float x;
	float DxDs;
	float DxDstrike;
} blackCalcStruct;

typedef struct
{
	float average;
	float sigma;
	float denominator;
	float derNormalizationFactor;
	float normalizationFactor;
} normalDistStruct;

//define into for each type of option
#define CALL 0
#define PUT 1

typedef struct
{ 
	int type;
	float strike;
	float spot;
	float q;
	float r;
	float t;
	float vol;
	float value;
	float tol;
} optionInputStruct;

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_STRUCTS_CUH
