//monteCarloEngine.cu
//Scott Grauer-Gray
//May 10, 2012
//Function for running Monte Carlo on the GPU

//needed for the monte carlo GPU kernels
#include "monteCarloKernels.cu"

//needed for the monte carlo CPU kernels
#include "monteCarloKernelsCpu.cu"

//needed for curand
#include <curand_kernel.h>

//needed for the thrust library (can be used for summation on GPU)
/* #include <thrust/transform_reduce.h> */
/* #include <thrust/functional.h> */
/* #include <thrust/device_vector.h> */
/* #include <thrust/host_vector.h> */

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



//initialize the inputs
void initializeInputs(dataType* samplePrices, dataType* sampleWeights, dataType* times)
{
}

//run monte carlo...
void runMonteCarlo()
{
	//int nSamplesArray[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};//, 					1000000, 2000000};//, 5000000};//, 10000000, 20000000};

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
		dataType mtimeGpu, mtimeCpu;   
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

		//declare what's necessary to use curand
		curandState* devStates;

		/* Allocate space for prng states on device */
		cudaMalloc (( void **) & devStates , numSamples * sizeof ( curandState ) );

		//allocate space for data on GPU
		cudaMalloc(&samplePricesGpu, NUM_OPTIONS*numSamples*sizeof(dataType));
		cudaMalloc(&sampleWeightsGpu, NUM_OPTIONS*numSamples*sizeof(dataType));
		cudaMalloc(&timesGpu, NUM_OPTIONS*numSamples*sizeof(dataType));
		cudaMalloc(&optionStructsGpu, NUM_OPTIONS*sizeof(monteCarloOptionStruct));

		//transfer data to GPU
		cudaMemcpy(samplePricesGpu, samplePrices, NUM_OPTIONS*numSamples*sizeof(dataType), cudaMemcpyHostToDevice);
		cudaMemcpy(sampleWeightsGpu, sampleWeights, NUM_OPTIONS*numSamples*sizeof(dataType), cudaMemcpyHostToDevice);
		cudaMemcpy(timesGpu, times, NUM_OPTIONS*numSamples*sizeof(dataType), cudaMemcpyHostToDevice);
		cudaMemcpy(optionStructsGpu, optionStructs, NUM_OPTIONS*sizeof(monteCarloOptionStruct), cudaMemcpyHostToDevice);

	
	
		srand(time(NULL));

		//for (int numTime=0; numTime < 100; numTime++)
		{

		/* initialize random seed: */
		srand ( rand());

		printf("Run on GPU\n");
		gettimeofday(&start, NULL);

		// setup execution parameters
		dim3 grid1( (size_t)ceil((dataType)numSamples / ((dataType)THREAD_BLOCK_SIZE)), 1, 1);
		dim3 threads1( THREAD_BLOCK_SIZE, 1, 1);

		//initializes the states for the random number generator		
		setup_kernel <<< grid1, threads1 >>> (devStates, rand(), numSamples);

		cudaDeviceSynchronize();

		// setup execution parameters
	    	dim3 grid2( (size_t)ceil((dataType)numSamples / (THREAD_BLOCK_SIZE)), 1, 1);
	    	dim3 threads2( THREAD_BLOCK_SIZE, 1, 1);

		monteCarloGpuKernel <<< grid2, threads2 >>>(samplePricesGpu, sampleWeightsGpu, timesGpu, (1.0f / (dataType)SEQUENCE_LENGTH), devStates, optionStructsGpu, numSamples);

		cudaDeviceSynchronize();

		gettimeofday(&end, NULL);

		seconds  = end.tv_sec  - start.tv_sec;
	    	useconds = end.tv_usec - start.tv_usec;

	    	mtimeGpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5;


	    	printf("Processing time on GPU (CUDA): %f (ms)\n", mtimeGpu);

		// remove comments to perform summation on GPU using thrust library
		/* //create a pointer to point to the prices */
		/* thrust::device_ptr<dataType> devicePointerPrices(samplePricesGpu);	 */

		/* //compute the value for each option */
		/* for (int numOpt=0; numOpt < NUM_OPTIONS; numOpt++) */
		/* { */
		/* 	//use the reduction function in thrust to retrieve the summation of the prices */
		/* 	dataType sum = thrust::reduce(devicePointerPrices + numOpt*numSamples, devicePointerPrices + (numOpt+1)*(numSamples), (dataType) 0, thrust::plus<dataType>()); */

		/* 	dataType avgPrice = sum / numSamples; */
		/* 	printf("avgPrice: %f\n", avgPrice); */
		/* } */

	

		//transfer data back to host
		cudaMemcpy(samplePrices, samplePricesGpu, numSamples*sizeof(dataType), cudaMemcpyDeviceToHost);
	
		cudaMemcpy(sampleWeights, sampleWeightsGpu, numSamples*sizeof(dataType), cudaMemcpyDeviceToHost);
		cudaMemcpy(times, timesGpu, numSamples*sizeof(dataType), cudaMemcpyDeviceToHost);


		//retrieve the average price
		dataType cumPrice = 0.0f;

		//add all the computed prices together
		for (int numSamp = 0; numSamp < numSamples; numSamp++)
		{
			cumPrice += samplePrices[numSamp];
		}

		dataType avgPrice = cumPrice / numSamples;
		printf("Average Price (GPU computation): %f\n\n", avgPrice);
	

		}

		//free memory space on the GPU
		cudaFree(samplePricesGpu);
		cudaFree(sampleWeightsGpu);
		cudaFree(timesGpu);
		cudaFree(optionStructsGpu);

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

		printf("Speedup on GPU: %f\n", mtimeCpu / mtimeGpu);

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
