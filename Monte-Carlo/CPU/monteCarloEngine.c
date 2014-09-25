//monteCarloEngine.cu
//Scott Grauer-Gray
//May 10, 2012
//Function for running Monte Carlo on the GPU

//needed for the monte carlo CPU kernels
#include "monteCarloKernelsCpu.c"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define RISK_VAL 0.06f
#define DIV_VAL 0.0f
#define VOLT_VAL 0.200f
#define UNDERLYING_VAL 30.0f
#define STRIKE_VAL 40.0f
#define DISCOUNT_VAL 0.94176453358424872f



//run monte carlo...
void runMonteCarlo()
{
	//int nSamplesArray[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000}

	int numSamples = 400000;

	//for (int numTime=0; numTime < 12; numTime++)
	{
		//int numSamples = nSamplesArray[numTime];

		printf("Number of Samples: %d\n\n", numSamples);

		//declare and initialize the struct used for the option
		monteCarloOptionStruct optionStruct;
		optionStruct.riskVal = RISK_VAL;
		optionStruct.divVal = DIV_VAL;
		optionStruct.voltVal = VOLT_VAL;
		optionStruct.underlyingVal = UNDERLYING_VAL;
		optionStruct.strikeVal = STRIKE_VAL;
		optionStruct.discountVal = DISCOUNT_VAL;


		//declare pointers for data on CPU
		dataType* samplePrices;
		dataType* sampleWeights;
		dataType* times;
		monteCarloOptionStruct* optionStructs;

		//allocate space for data on CPU
		samplePrices = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		sampleWeights = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		times = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		optionStructs = (monteCarloOptionStruct*)malloc(NUM_OPTIONS*sizeof(monteCarloOptionStruct));

		long seconds, useconds;
		dataType mtimeCpu;   
		struct timeval start;
		struct timeval end;

		for (int optNum = 0; optNum < NUM_OPTIONS; optNum++)
		{
			optionStructs[optNum] = optionStruct;
		}

		//initialize values for data on CPU
		//declare pointers for data on GPU
		dataType* samplePricesGpu;
		dataType* sampleWeightsGpu;
		dataType* timesGpu;
		monteCarloOptionStruct* optionStructsGpu;

		srand(time(NULL));
		
		/* initialize random seed: */
		srand ( rand());

		//free memory space on the CPU
		free(samplePrices);
		free(sampleWeights);
		free(times);

		
		//declare pointers for data on CPU
		dataType* samplePricesCpu;
		dataType* sampleWeightsCpu;
		dataType* timesCpu;

		//allocate space for data on CPU
		samplePricesCpu = (dataType*)malloc(numSamples*sizeof(dataType));
		sampleWeightsCpu = (dataType*)malloc(numSamples*sizeof(dataType));
		timesCpu = (dataType*)malloc(numSamples*sizeof(dataType));

	   
		gettimeofday(&start, NULL);
		 
		monteCarloGpuKernelCpu(samplePricesCpu, sampleWeightsCpu, timesCpu, (1.0f / (dataType)SEQUENCE_LENGTH), optionStructs, numSamples);


		gettimeofday(&end, NULL);

		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		mtimeCpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5;
		printf("Run on CPU\n");
		printf("Processing time on CPU: %f (ms)\n", mtimeCpu);	

		//retrieve the average price
		dataType cumPrice = 0.0f;

		//add all the computed prices together
		for (int numSamp = 0; numSamp < numSamples; numSamp++)
		{
			cumPrice += samplePricesCpu[numSamp];
		}

		dataType avgPrice = cumPrice / numSamples;
		printf("Average Price (CPU computation): %f\n\n", avgPrice);

		//free memory space on the CPU
		free(samplePricesCpu);
		free(sampleWeightsCpu);
		free(timesCpu);
		free(optionStructs);

	}
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
	runMonteCarlo();

	char c;
	c = getchar();
	printf("%c\n", c);
}
