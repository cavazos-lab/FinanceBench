//monteCarloStructs.cuh
//Scott Grauer-Gray
//May 24, 2012
//Header file with structures used in monte carlo computation

#ifndef MONTE_CARLO_STRUCTS_CUH
#define MONTE_CARLO_STRUCTS_CUH

typedef float dataType;

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

//struct for defining what's in an option
typedef struct
{
	float riskVal;
	float divVal;
	float voltVal;
	float underlyingVal;
	float strikeVal;
	float discountVal;
} monteCarloOptionStruct;

#endif //MONTE_CARLO_STRUCTS_CUH
