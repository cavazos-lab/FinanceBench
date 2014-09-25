//monteCarloStructs.cuh
//Scott Grauer-Gray
//May 24, 2012
//Header file with structures used in monte carlo computation

#ifndef MONTE_CARLO_STRUCTS_CUH
#define MONTE_CARLO_STRUCTS_CUH

typedef float dataType;

//struct for defining what's in an option
typedef struct
{
	dataType riskVal;
	dataType divVal;
	dataType voltVal;
	dataType underlyingVal;
	dataType strikeVal;
	dataType discountVal;
} monteCarloOptionStruct;

#endif //MONTE_CARLO_STRUCTS_CUH
