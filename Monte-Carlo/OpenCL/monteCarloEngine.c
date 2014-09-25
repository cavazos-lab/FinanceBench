//monteCarloEngine.c
//Scott Grauer-Gray
//May 10, 2012
//Function for running Monte Carlo on the GPU using OpenCL

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "mt19937.c"

#include "monteCarloStructs.h"

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

// global variables
cl_platform_id platform_id;
cl_device_id device_id;   
cl_uint num_devices;
cl_uint num_platforms;
cl_int errcode;
cl_program clProgram;
cl_context clGPUContext;
cl_kernel clKernel;
cl_kernel clKernelInitializeMersenneState;
cl_command_queue clCommandQue;

#define MAX_SOURCE_SIZE (0x100000)

char str_temp[1024];

cl_mem samplePricesGpu;
cl_mem sampleWeightsGpu;
cl_mem timesGpu;
cl_mem optionStructsGpu;
cl_mem randStatesGpu;

FILE *fp;
char *source_str;
size_t source_size;



void cl_clean_up()
{
	// Clean up
	errcode = clFlush(clCommandQue);
	errcode = clFinish(clCommandQue);
	errcode = clReleaseKernel(clKernel);
	errcode = clReleaseKernel(clKernelInitializeMersenneState);
	errcode = clReleaseProgram(clProgram);
	errcode = clReleaseMemObject(samplePricesGpu);
	errcode = clReleaseMemObject(sampleWeightsGpu);
	errcode = clReleaseMemObject(timesGpu);
	errcode = clReleaseMemObject(optionStructsGpu);
	errcode = clReleaseMemObject(randStatesGpu);
	errcode = clReleaseCommandQueue(clCommandQue);
	errcode = clReleaseContext(clGPUContext);
	if(errcode != CL_SUCCESS) printf("Error in cleanup\n");
}


void cl_initialization()
{	
	// Get platform and device information
	errcode = clGetPlatformIDs(1, &platform_id, &num_platforms);
	if(errcode == CL_SUCCESS) printf("number of platforms is %d\n",num_platforms);
	else printf("Error getting platform IDs\n");

	errcode = clGetPlatformInfo(platform_id,CL_PLATFORM_NAME, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("platform name is %s\n",str_temp);
	else printf("Error getting platform name\n");

	errcode = clGetPlatformInfo(platform_id, CL_PLATFORM_VERSION, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("platform version is %s\n",str_temp);
	else printf("Error getting platform version\n");

	errcode = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices);
	if(errcode == CL_SUCCESS) printf("number of devices is %d\n", num_devices);
	else printf("Error getting device IDs\n");

	errcode = clGetDeviceInfo(device_id,CL_DEVICE_NAME, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("device name is %s\n",str_temp);
	else printf("Error getting device name\n");
	
	// Create an OpenCL context
	clGPUContext = clCreateContext( NULL, 1, &device_id, NULL, NULL, &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating context\n");
 
	//Create a command-queue
	clCommandQue = clCreateCommandQueue(clGPUContext, device_id, 0, &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating command queue\n");
}



void cl_load_prog()
{
	// Create a program from the kernel source
	clProgram = clCreateProgramWithSource(clGPUContext, 1, (const char **)&source_str, (const size_t *)&source_size, &errcode);

	if(errcode != CL_SUCCESS) printf("Error in creating program\n");


	// Build the program
	errcode = clBuildProgram(clProgram, 1, &device_id, "-I .", NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in building program\n");
			if (errcode != CL_SUCCESS)
{
		size_t ret_val_size;
		char *build_log = NULL;

		clGetProgramBuildInfo(clProgram, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
		build_log = (char *) calloc(ret_val_size + 1, sizeof(char));
		clGetProgramBuildInfo(clProgram, device_id, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
		build_log[ret_val_size] = '\0';
		printf("%s", build_log);
		free(build_log);

}

	// Create the OpenCL kernel
	clKernel = clCreateKernel(clProgram, "monteCarloGpuKernel", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");

	clKernelInitializeMersenneState = clCreateKernel(clProgram, "initializeMersenneStateGpu", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");


}


void read_cl_file()
{
	// Load the kernel source code into the array source_str
	fp = fopen("monteCarloKernels.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
}

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

		cl_initialization();
	
		samplePricesGpu = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                NUM_OPTIONS*numSamples*sizeof(dataType), NULL, NULL);

		sampleWeightsGpu = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                NUM_OPTIONS*numSamples*sizeof(dataType), NULL, NULL);

		timesGpu = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                NUM_OPTIONS*numSamples*sizeof(dataType), NULL, NULL);

		optionStructsGpu = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                NUM_OPTIONS*sizeof(monteCarloOptionStruct), NULL, NULL);

		randStatesGpu = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                numSamples*sizeof(mt19937state), NULL, NULL);


		read_cl_file();
		cl_load_prog();
	
		

		printf("numSamps: %d\n", numSamples);

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
		mt19937state* randStates;

		//allocate space for data on CPU
		samplePrices = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		sampleWeights = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		times = (dataType*)malloc(NUM_OPTIONS*numSamples*sizeof(dataType));
		optionStructs = (monteCarloOptionStruct*)malloc(NUM_OPTIONS*sizeof(monteCarloOptionStruct));
		randStates = (mt19937state*)malloc(numSamples*sizeof(mt19937state));

		long seconds, useconds;
		dataType mtimeGpu, mtimeCpu;   
	    	struct timeval start;
	    	struct timeval end;

		for (int optNum = 0; optNum < NUM_OPTIONS; optNum++)
		{
			optionStructs[optNum] = optionStruct;
		}

		int samNum;

    		/*for (samNum = 0; samNum < numSamples; samNum++)
      		{
			initializeMersenneState (&randStates [samNum], rand ());
      		}*/


		
		//transfer data to device
		clEnqueueWriteBuffer(clCommandQue, samplePricesGpu, CL_TRUE, 0, NUM_OPTIONS *numSamples* sizeof(dataType), (void*)samplePrices, 0, NULL, NULL);
		clEnqueueWriteBuffer(clCommandQue, sampleWeightsGpu, CL_TRUE, 0, NUM_OPTIONS *numSamples* sizeof(dataType), (void*)sampleWeights, 0, NULL, NULL);
		clEnqueueWriteBuffer(clCommandQue, timesGpu, CL_TRUE, 0, NUM_OPTIONS *numSamples* sizeof(dataType), (void*)times, 0, NULL, NULL);
		clEnqueueWriteBuffer(clCommandQue, optionStructsGpu, CL_TRUE, 0, NUM_OPTIONS * sizeof(monteCarloOptionStruct), (void*)optionStructs, 0, NULL, NULL);
		clEnqueueWriteBuffer(clCommandQue, randStatesGpu, CL_TRUE, 0, numSamples * sizeof(mt19937state), (void*)randStates, 0, NULL, NULL);

		dataType dt = (1.0f / (dataType)SEQUENCE_LENGTH);
	
		unsigned int timer = 0;


		srand(time(NULL));

		gettimeofday(&start, NULL);

		
		/* initialize random seed: */
		srand ( rand());

		printf("\nRun on GPU\n");

		

		size_t localWorkSize[] = {256, 1};
		size_t globalWorkSize[] = {(size_t)ceil((dataType)numSamples / (dataType)localWorkSize[0])*localWorkSize[0], 1};
		
		unsigned long seed = rand();


		clSetKernelArg(clKernelInitializeMersenneState, 0, sizeof(cl_mem), (void *) &randStatesGpu);
		clSetKernelArg(clKernelInitializeMersenneState, 1, sizeof(unsigned long), (void *) &seed);
		clSetKernelArg(clKernelInitializeMersenneState, 2, sizeof(int), (void *) &numSamples);
		
		clEnqueueNDRangeKernel(clCommandQue, clKernelInitializeMersenneState, 1, 0, globalWorkSize, localWorkSize,
				                   0, NULL, NULL);

		if(errcode != CL_SUCCESS)
		{
			printf("Error in launching kernel\n");
		}
		clFinish(clCommandQue);

		clSetKernelArg(clKernel, 0, sizeof(cl_mem), (void *) &samplePricesGpu);
		clSetKernelArg(clKernel, 1, sizeof(cl_mem), (void *) &sampleWeightsGpu);
		clSetKernelArg(clKernel, 2, sizeof(cl_mem), (void *) &timesGpu);
		clSetKernelArg(clKernel, 3, sizeof(dataType), (void *) &dt);
		clSetKernelArg(clKernel, 4, sizeof(cl_mem), (void *) &randStatesGpu);
		clSetKernelArg(clKernel, 5, sizeof(cl_mem), (void *) &optionStructsGpu);
		clSetKernelArg(clKernel, 6, sizeof(int), (void *) &numSamples);


		clEnqueueNDRangeKernel(clCommandQue, clKernel, 1, 0, globalWorkSize, localWorkSize,
				                   0, NULL, NULL);

		if(errcode != CL_SUCCESS)
		{
			printf("Error in launching kernel\n");
		}

		clFinish(clCommandQue);


		//transfer data back to host
		
		clEnqueueReadBuffer(clCommandQue, samplePricesGpu, CL_TRUE, 0, numSamples*sizeof(dataType), (void*)samplePrices, 0, NULL, NULL);	


		//retrieve the average price
		dataType cumPrice = 0.0f;

		//add all the computed prices together
		for (int numSamp = 0; numSamp < numSamples; numSamp++)
		{
			cumPrice += samplePrices[numSamp];
		}

		dataType avgPrice = cumPrice/numSamples;

		gettimeofday(&end, NULL);

		printf("Average price on GPU: %f\n", avgPrice);
	
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
	        
		mtimeGpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5;

		printf("Processing time on GPU: %f (ms)\n\n", mtimeGpu);

		//free memory space on the GPU
		cl_clean_up();
		

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

		printf("Run on CPU\n");
	   
		gettimeofday(&start, NULL);
		 
		monteCarloGpuKernelCpu(samplePricesCpu, sampleWeightsCpu, timesCpu, (1.0f / (dataType)SEQUENCE_LENGTH), optionStructs, numSamples);


		cumPrice = 0.0f;
		//add all the computed prices together
		for (int numSamp = 0; numSamp < numSamples; numSamp++)
		{

			cumPrice += samplePricesCpu[numSamp];
		}

		avgPrice = cumPrice / numSamples;


		gettimeofday(&end, NULL);

		seconds  = end.tv_sec  - start.tv_sec;
	    	useconds = end.tv_usec - start.tv_usec;

	    	mtimeCpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5;
	    	printf("Processing time on CPU: %f (ms)\n", mtimeCpu);
	

		//retrieve the average price
		cumPrice = 0.0f;

		
		printf("Average price on CPU: %f\n\n", avgPrice);

		printf("GPU Speedup: %f\n", mtimeCpu / mtimeGpu);

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
