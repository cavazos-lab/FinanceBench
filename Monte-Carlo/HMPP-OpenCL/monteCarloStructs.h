//monteCarloStructs.h
//Scott Grauer-Gray
//Header file with structures used in monte carlo computation

#ifndef MONTE_CARLO_STRUCTS_H
#define MONTE_CARLO_STRUCTS_H

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
