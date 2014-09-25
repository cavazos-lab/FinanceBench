//blackScholesAnalyticEngine.c
//Scott Grauer-Gray
//Functions for running black scholes using the analytic engine (from Quantlib) on the GPU

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

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

//needed for optionInputStruct
#include "blackScholesAnalyticEngineStructs.h"
#include "blackScholesAnalyticEngineKernelsCpu.c"
#include <stdio.h>

#include <time.h>

#define NUM_DIFF_SETTINGS 37

// global variables
cl_platform_id platform_id;
cl_device_id device_id;   
cl_uint num_devices;
cl_uint num_platforms;
cl_int errcode;
cl_program clProgram;
cl_context clGPUContext;
cl_kernel clKernel;
cl_command_queue clCommandQue;

#define MAX_SOURCE_SIZE (0x100000)

char str_temp[1024];

cl_mem inputOptions;
cl_mem resultsOutput;

FILE *fp;
char *source_str;
size_t source_size;

void cl_clean_up()
{
	// Clean up
	errcode = clFlush(clCommandQue);
	errcode = clFinish(clCommandQue);
	errcode = clReleaseKernel(clKernel);
	errcode = clReleaseProgram(clProgram);
	errcode = clReleaseMemObject(inputOptions);
	errcode = clReleaseMemObject(resultsOutput);
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
	clKernel = clCreateKernel(clProgram, "getOutValOption", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");
}


void read_cl_file()
{
	// Load the kernel source code into the array source_str
	fp = fopen("blackScholesAnalyticEngineKernels.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
}


//function to run the black scholes analytic engine on the gpu
void runBlackScholesAnalyticEngine()
{
  	//int nSamplesArray[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000, 10000000, 20000000, 50000000};

	int numVals = 5000000;
	//for (int numTime=0; numTime < 17; numTime++)
	{
	//int numVals = nSamplesArray[numTime];
	printf("Number of options: %d\n\n", numVals);

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

	cl_initialization();
	
	inputOptions = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                numVals*sizeof(optionInputStruct), NULL, NULL);

	resultsOutput = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE,
                                numVals*sizeof(dataType), NULL, NULL);

	read_cl_file();
	cl_load_prog();

	double t_start, t_end;
	
	// Run GPU code
	
	//initialize the arrays

	//declare and allocate the input and output data on the CPU
	dataType* outputVals = (dataType*)malloc(numVals * sizeof(dataType));


	clEnqueueWriteBuffer(clCommandQue, inputOptions, CL_TRUE, 0, numVals * sizeof(optionInputStruct), (void*)values, 0, NULL, NULL);

	clSetKernelArg(clKernel, 0, sizeof(cl_mem), (void *) &inputOptions);
		clSetKernelArg(clKernel, 1, sizeof(cl_mem), (void *) &resultsOutput);
		clSetKernelArg(clKernel, 2, sizeof(int), (void *) &numVals);



	size_t localWorkSize[] = {256, 1};
	size_t globalWorkSize[] = {(size_t)ceil((dataType)numVals / (dataType)localWorkSize[0])*localWorkSize[0], 1};

	printf("\nRun on GPU\n");
	struct timeval start;
	gettimeofday(&start, NULL);

	clEnqueueNDRangeKernel(clCommandQue, clKernel, 1, 0, globalWorkSize, localWorkSize,
				                   0, NULL, NULL);

	if(errcode != CL_SUCCESS)
	{
		printf("Error in launching kernel\n");
	}
	clFinish(clCommandQue);
	
	long seconds, useconds;    
	dataType mtimeCpu, mtimeGpu;


	struct timeval end;
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
    	useconds = end.tv_usec - start.tv_usec;

    	mtimeGpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5f;


    	printf("Processing time on GPU: %f (ms)\n", mtimeGpu);


	//copy the resulting option values back to the CPU
	clEnqueueReadBuffer(clCommandQue, resultsOutput, CL_TRUE, 0, numVals*sizeof(dataType), (void*)outputVals, 0, NULL, NULL);

	dataType totResult = 0.0f;
	for (int i=0; i<numVals; i++)
	{
		totResult += outputVals[i];
	}

	printf("Summation of all output prices on GPU: %f\n", totResult);



	printf("Output price for option %d on GPU: %f\n\n", numVals/2, outputVals[numVals/2]);

	gettimeofday(&start, NULL);

	//run on CPU
	for (size_t numOption=0; numOption < numVals; numOption++)
	{
		getOutValOptionCpu(values, outputVals, numOption, numVals);	
	}

	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
    	useconds = end.tv_usec - start.tv_usec;

    	mtimeCpu = ((seconds) * 1000 + ((dataType)useconds)/1000.0) + 0.5f;

	printf("Run on CPU\n");
    	printf("Processing time on CPU: %f (ms)\n", mtimeCpu);
    
	totResult = 0.0f;
	for (int i=0; i<numVals; i++)
	{
		totResult += outputVals[i];
	}

	printf("Summation of all output prices on GPU: %f\n", totResult);


	printf("Output price for option %d on GPU: %f\n\n", numVals/2, outputVals[numVals/2]);
	printf("Speedup on GPU: %f\n", mtimeCpu / mtimeGpu);
	delete [] values;
	free(outputVals);

	cl_clean_up();

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
