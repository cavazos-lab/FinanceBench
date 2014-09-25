//monteCarloKernels.h
//Scott Grauer-Gray
//Kernel headers for running monte carlo on the GPU

#ifndef MONTE_CARLO_KERNELS_H
#define MONTE_CARLO_KERNELS_H

#include <math.h>

//needed for constants related to monte carlo
#include "monteCarloConstants.h"

//needed for structs related to monte carlo
#include "monteCarloStructs.h"

//needed for RNG
#include "mt19937.h"

//function to set up the random states

float compInverseNormDist (float x) ;

float interestRateCompoundFact (float t, float rate);

float interestRateDiscountFact (float t, float rate);

float flatForwardDiscountImpl (float t, float rate);

float yieldTermStructDiscount (float t, float rate);

float interestRateImpliedRate (float compound, float t);

float yieldTermStructForwardRate (float t1, float t2, float rate);

float processDrift (float t, float x, monteCarloOptionStruct optionStruct);

float discretizationDrift (float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float localVoltLocVol (float t, float underlyingLevel, monteCarloOptionStruct optionStruct);

float processDiff (float t, float x, monteCarloOptionStruct optionStruct);

float discDiff (float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float stdDeviation (float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float apply (float x0, float dx);

float discDrift (float t0, float x0, float dt, monteCarloOptionStruct optionStruct);

float processEvolve (float t0, float x0, float dt, float dw, monteCarloOptionStruct optionStruct);

//retrieve the current sequence
void getSequence (float* sequence, float sampleNum);

float getProcessValX0 (monteCarloOptionStruct optionStruct);

void getPath (float* path, size_t sampleNum, float dt, mt19937state* state, monteCarloOptionStruct optionStruct);
	
float getPrice (float val);

//initialize the path
void initializePath (float* path);

#endif //MONTE_CARLO_KERNELS_H
