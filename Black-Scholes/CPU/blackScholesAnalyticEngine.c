//blackScholesAnalyticEngine.c
//Scott Grauer-Gray
//Functions for running black scholes using the analytic engine (from Quantlib) on the CPU

//needed for optionInputStruct
#include "blackScholesAnalyticEngineStructs.h"

#include "blackScholesAnalyticEngineKernelsCpu.c"
#include <stdio.h>
#include <stdlib.h>

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



	
	//initialize the arrays

	//declare and allocate the input and output data on the CPU
	float* outputVals = (float*)malloc(numVals * sizeof(float));

	struct timeval start, end;



	gettimeofday(&start, NULL);


	//run on CPU
	for (size_t numOption=0; numOption < numVals; numOption++)
	{
			getOutValOptionCpu(values, outputVals, numOption, numVals);	
	}
	gettimeofday(&end, NULL);
	long seconds  = end.tv_sec  - start.tv_sec;
    	long useconds = end.tv_usec - start.tv_usec;

    	float mtimeCpu = ((seconds) * 1000 + ((float)useconds)/1000.0);

	printf("Run on CPU\n");
        printf("Processing time on CPU: %f (ms)\n", mtimeCpu);
    
	float totResult = 0.0f;
	for (int i=0; i<numVals; i++)
	{
		totResult += outputVals[i];
	}

	printf("Summation of output prices on CPU: %f\n", totResult);
	printf("Output price at index %d on CPU:: %f\n\n", numVals/2, outputVals[numVals/2]);

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
