//monteCarloConstants.cuh
//Scott Grauer-Gray
//May 10, 2012
//Constants for running monte carlo on the GPU/CPU

#ifndef MONTE_CARLO_CONSTANTS_H
#define MONTE_CARLO_CONSTANTS_H

#define DEFAULT_SEQ_VAL 1.0f
#define	DEFAULT_SEQ_WEIGHT 1.0f


#define RISK_VAL 0.06f
#define DIV_VAL 0.0f
#define VOLT_VAL 0.200f
#define UNDERLYING_VAL 30.0f
#define STRIKE_VAL 40.0f
#define DISCOUNT_VAL 0.94176453358424872f
#define START_PATH_VAL 1.0f
#define DEFAULT_SEQ_VAL 1.0f
#define	DEFAULT_SEQ_WEIGHT 1.0f
#define SEQUENCE_LENGTH 250


#define A_1 -39.696830286653757
#define A_2 220.94609842452050
#define A_3 -275.92851044696869
#define A_4 138.35775186726900
#define A_5 -30.664798066147160
#define A_6 2.5066282774592392
#define B_1 -54.476098798224058
#define B_2 161.58583685804089
#define B_3 -155.69897985988661
#define B_4 66.801311887719720
#define B_5 -13.280681552885721


//define the thread block size
#define THREAD_BLOCK_SIZE 256

//define the number of samples
#define NUM_SAMPLES 400000

//number of options
#define NUM_OPTIONS 1

//number of threads to use for processing each option
#define NUM_THREADS_PER_OPTION NUM_SAMPLES

#endif //MONTE_CARLO_CONSTANTS_CUH

