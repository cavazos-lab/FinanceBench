//blackScholesAnalyticEngine.cu
//Scott Grauer-Gray
//Functions for running black scholes using the analytic engine (from Quantlib) on the GPU

//needed for optionInputStruct
#include "blackScholesAnalyticEngineStructs.cuh"

//needed for the kernel(s) to run on the GPU
#include "blackScholesAnalyticEngineKernels.cu"

#include "blackScholesAnalyticEngineKernelsCpu.cu"
#include <stdio.h>

#include <sys/time.h>
#include <time.h>

#define NUM_DIFF_SETTINGS 37

//function to run the black scholes analytic engine on the gpu
void runBlackScholesAnalyticEngine()
{
	//int nSamplesArray[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000};

	int numberOfSamples = 5000000;
	//for (int numTime=14; numTime < 15; numTime++)
	{
		int numVals = numberOfSamples;//nSamplesArray[numTime];

		optionInputStruct* values = new optionInputStruct[numVals];

		for (int numOption = 0; numOption < numVals; numOption++)
		{
		  if ((numOption % NUM_DIFF_SETTINGS) == 0)
		  {
				optionInputStruct currVal = { CALL,  40.00,  42.00, 0.08, 0.04, 0.75, 0.35,  5.0975, 1.0e-4};
				values[numOption] = currVal;
		  }
		  if ((numOption % NUM_DIFF_SETTINGS) == 1)
		  {
	      		optionInputStruct currVal = { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.15,  0.0205, 1.0e-4};
				values[numOption] = currVal;
		  }
		  if ((numOption % NUM_DIFF_SETTINGS) == 2)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.15,  1.8734, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 3)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.15,  9.9413, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 4)
		  {
		  		optionInputStruct currVal = { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.25,  0.3150, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 5)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.25,  3.1217, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 6)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.25, 10.3556, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 7)
		  {
		 		optionInputStruct currVal =  { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.35,  0.9474, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 8)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.35,  4.3693, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 9)
		  {
		  		optionInputStruct currVal = { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.35, 11.1381, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 10)
		  {
		 		optionInputStruct currVal =  { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.15,  0.8069, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 11)
		  {
		 		optionInputStruct currVal =  { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.15,  4.0232, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 12)
		  {
			optionInputStruct currVal =  { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.15, 10.5769, 1.0e-4};
				values[numOption] = currVal;
		  }
	      if ((numOption % NUM_DIFF_SETTINGS) == 13)
		  {
				optionInputStruct currVal =   { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.25,  2.7026, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 14)
		  {
				optionInputStruct currVal =   { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.25,  6.6997, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 15)
		  {
				optionInputStruct currVal =   { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.25, 12.7857, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 16)
		  {
				optionInputStruct currVal =   { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.35,  4.9329, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 17)
		  {
			optionInputStruct currVal =  { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.35,  9.3679, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 18)
		  {
			optionInputStruct currVal = { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.35, 15.3086, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 19)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.15,  9.9210, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 20)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.15,  1.8734, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 21)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.15,  0.0408, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 22)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.25, 10.2155, 1.0e-4};
				values[numOption] = currVal;
		  }
	      if ((numOption % NUM_DIFF_SETTINGS) == 23)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.25,  3.1217, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 24)
		  {
				optionInputStruct currVal =    { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.25,  0.4551, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 25)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.35, 10.8479, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 26)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.35,  4.3693, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 27)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.35,  1.2376, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 28)
		  {
		 		optionInputStruct currVal =  { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.15, 10.3192, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 29)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.15,  4.0232, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 30)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.15,  1.0646, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 31)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.25, 12.2149, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 32)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.25,  6.6997, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 33)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.25,  3.2734, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 34)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.35, 14.4452, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 35)
		  {
		  		optionInputStruct currVal =  { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.35,  9.3679, 1.0e-4};
				values[numOption] = currVal;
	      }
		  if ((numOption % NUM_DIFF_SETTINGS) == 36)
		  {
		 		optionInputStruct currVal =   { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.35,  5.7963, 1.0e-4};
				values[numOption] = currVal;
		  }
		}



	// Run GPU code
	
	//initialize the arrays

	//declare and allocate the input and output data on the CPU
	float* outputVals = (float*)malloc(numVals * sizeof(float));

	//declare the data on the GPU
	optionInputStruct* optionsGpu;
	float* outputValsGpu;

	//allocate space for data on GPU
	cudaMalloc(&optionsGpu, numVals * sizeof(optionInputStruct));
	cudaMalloc(&outputValsGpu, numVals * sizeof(float));

	//copy the data from the CPU to the GPU
	cudaMemcpy(optionsGpu, values, numVals * sizeof(optionInputStruct), cudaMemcpyHostToDevice);

	// setup execution parameters
	dim3  grid( (size_t)ceil((float)numVals / (float)THREAD_BLOCK_SIZE), 1, 1);
	dim3  threads( THREAD_BLOCK_SIZE, 1, 1);

	printf("Number of options: %d\n\n", numVals);
	long seconds, useconds;    
	float mtimeCpu, mtimeGpu;

	struct timeval start;
	gettimeofday(&start, NULL);

	getOutValOption <<< grid, threads >>>(optionsGpu, outputValsGpu, numVals);
	cudaDeviceSynchronize();

	struct timeval end;
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;

	mtimeGpu = ((seconds) * 1000 + ((float)useconds)/1000.0) + 0.5f;

	printf("Run on GPU\n");
	printf("Processing time on GPU (CUDA): %f (ms)\n", mtimeGpu);

	//copy the resulting option values back to the CPU
	cudaMemcpy(outputVals, outputValsGpu, numVals * sizeof(float), cudaMemcpyDeviceToHost);

	float totResult = 0.0f;
	for (int i=0; i<numVals; i++)
	{
		totResult += outputVals[i];
	}

	printf("Summation of output prices on GPU: %f\n", totResult);

	cudaFree(optionsGpu);
	cudaFree(outputValsGpu);

	printf("Output price at index %d on GPU: %f\n\n", numVals/2, outputVals[numVals/2]);


	gettimeofday(&start, NULL);


	//run on CPU
	for (size_t numOption=0; numOption < numVals; numOption++)
	{
			getOutValOptionCpu(values, outputVals, numOption, numVals);	
	}
	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
    	useconds = end.tv_usec - start.tv_usec;

    	mtimeCpu = ((seconds) * 1000 + ((float)useconds)/1000.0) + 0.5f;

	printf("Run on CPU\n");
        printf("Processing time on CPU: %f (ms)\n", mtimeCpu);
    
	totResult = 0.0f;
	for (int i=0; i<numVals; i++)
	{
		totResult += outputVals[i];
	}

	printf("Summation of output prices on CPU: %f\n", totResult);
	printf("Output price at index %d on CPU:: %f\n\n", numVals/2, outputVals[numVals/2]);

	printf("Speedup on GPU: %f\n", mtimeCpu / mtimeGpu);

	delete [] values;
	free(outputVals);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
	runBlackScholesAnalyticEngine();
	char c;
	c = getchar();
	printf("%c\n", c);
}
