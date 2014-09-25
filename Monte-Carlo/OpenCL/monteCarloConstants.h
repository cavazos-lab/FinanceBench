//monteCarloConstants.cuh
//Scott Grauer-Gray
//May 10, 2012
//Constants for running monte carlo on the GPU/CPU

#ifndef MONTE_CARLO_CONSTANTS_CUH
#define MONTE_CARLO_CONSTANTS_CUH

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


//define the thread block size
#define THREAD_BLOCK_SIZE 256

//define the number of samples
#define NUM_SAMPLES 400000

//number of options
#define NUM_OPTIONS 1

//number of threads to use for processing each option
#define NUM_THREADS_PER_OPTION NUM_SAMPLES


#endif //MONTE_CARLO_CONSTANTS_CUH

