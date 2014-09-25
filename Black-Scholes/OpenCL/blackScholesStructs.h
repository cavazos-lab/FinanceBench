//blackScholesStructs.cuh
//Scott Grauer-Gray
//Structs for processing black scholes options

#ifndef BLACK_SCHOLES_STRUCTS_CUH
#define BLACK_SCHOLES_STRUCTS_CUH

typedef float dataType;

typedef struct
{
	int day;
	int month;
	int year;
} dateStruct;


typedef struct
{
	dataType type;
	dataType strike;
} payoffStruct;


typedef struct
{
	dataType typeExercise;
	dataType date;
} exerciseStruct;

typedef struct
{
	dataType rate;
	dataType freq;
	int comp;
} interestRateStruct;

typedef struct
{
	dataType refDate;
	dataType calDate;
	dataType forward;
	dataType compounding;
	dataType frequency;
	interestRateStruct intRate;
} yieldTermStruct;

typedef struct
{
	dataType refDate;
	dataType calDate;
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
	exerciseStruct exercise;
	engineStruct pricingEngine;
} optionStruct;

typedef struct
{
	int direction;
	dataType* lower;
	dataType* diag;
	dataType* upper;
	dataType* i0;
	dataType* i2;
	dataType* reverseIndex;
} tripleBandLinearOpStruct;

typedef struct
{
} solverStruct;

#endif //BLACK_SCHOLES_STRUCTS_CUH
