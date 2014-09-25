//blackScholesAnalyticEngine.c
//Scott Grauer-Gray
//Functions for running black scholes using the analytic engine (from Quantlib) on the GPU

#include <stdio.h>
#include <sys/time.h>

// optionInputStruct
#include "blackScholesAnalyticEngineStructs.h"

// engine code [Accelerator]
#include "blackScholesAnalyticEngineKernels.h"
#include "blackScholesAnalyticEngineKernels.c"


#pragma hmpp options codelet, args[n].transfer=atcall, args[values;outputVals].transfer=manual, target=CUDA
void options (int n, optionInputStruct values[n], float outputVals[n])
{
    int i;

    #pragma hmppcg gridify
    for (i = 0; i < n; ++i)
    {
	getOutValOption (values, outputVals, i);
    }
}


//function to run the black scholes analytic engine on the gpu
void
runBlackScholesAnalyticEngine ()
{
    optionInputStruct* values = (optionInputStruct*) malloc (NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC * sizeof (optionInputStruct));
    {
        const int beginSize = 24;
        optionInputStruct begin[] = {
            //type, strike,   spot,    q,    r,    t,  vol,   value,    tol
            { CALL,  65.00,  60.00, 0.00, 0.08, 0.25, 0.30,  2.1334, 1.0e-4},
            { PUT,   95.00, 100.00, 0.05, 0.10, 0.50, 0.20,  2.4648, 1.0e-4},
            { PUT,   19.00,  19.00, 0.10, 0.10, 0.75, 0.28,  1.7011, 1.0e-4},
            { CALL,  19.00,  19.00, 0.10, 0.10, 0.75, 0.28,  1.7011, 1.0e-4},
            { CALL,   1.60,   1.56, 0.08, 0.06, 0.50, 0.12,  0.0291, 1.0e-4},
            { PUT,   70.00,  75.00, 0.05, 0.10, 0.50, 0.35,  4.0870, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.15,  0.0205, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.15,  1.8734, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.15,  9.9413, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.25,  0.3150, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.25,  3.1217, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.25, 10.3556, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.35,  0.9474, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.35,  4.3693, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.35, 11.1381, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.15,  0.8069, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.15,  4.0232, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.15, 10.5769, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.25,  2.7026, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.25,  6.6997, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.25, 12.7857, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.35,  4.9329, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.35,  9.3679, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.35, 15.3086, 1.0e-4}
        };

        const int updateSize = 36;
        optionInputStruct update [] = {
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.15,  9.9210, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.15,  1.8734, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.15,  0.0408, 1.0e-4},
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.25, 10.2155, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.25,  3.1217, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.25,  0.4551, 1.0e-4},
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.10, 0.35, 10.8479, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.10, 0.35,  4.3693, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.10, 0.35,  1.2376, 1.0e-4},
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.15, 10.3192, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.15,  4.0232, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.15,  1.0646, 1.0e-4},
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.25, 12.2149, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.25,  6.6997, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.25,  3.2734, 1.0e-4},
            { PUT,  100.00,  90.00, 0.10, 0.10, 0.50, 0.35, 14.4452, 1.0e-4},
            { PUT,  100.00, 100.00, 0.10, 0.10, 0.50, 0.35,  9.3679, 1.0e-4},
            { PUT,  100.00, 110.00, 0.10, 0.10, 0.50, 0.35,  5.7963, 1.0e-4},
            { CALL,  40.00,  42.00, 0.08, 0.04, 0.75, 0.35,  5.0975, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.15,  0.0205, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.15,  1.8734, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.15,  9.9413, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.25,  0.3150, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.25,  3.1217, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.25, 10.3556, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.10, 0.35,  0.9474, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.10, 0.35,  4.3693, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.10, 0.35, 11.1381, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.15,  0.8069, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.15,  4.0232, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.15, 10.5769, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.25,  2.7026, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.25,  6.6997, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.25, 12.7857, 1.0e-4},
            { CALL, 100.00,  90.00, 0.10, 0.10, 0.50, 0.35,  4.9329, 1.0e-4},
            { CALL, 100.00, 100.00, 0.10, 0.10, 0.50, 0.35,  9.3679, 1.0e-4},
            { CALL, 100.00, 110.00, 0.10, 0.10, 0.50, 0.35, 15.3086, 1.0e-4}
        };

	int i;
        for (i = 0; i < NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC; i++)
        {
            if (i < beginSize)
            {
                values[i].type = begin[i].type;
                values[i].strike = begin[i].strike;
                values[i].spot = begin[i].spot;
                values[i].q = begin[i].q;
                values[i].r = begin[i].r;
                values[i].t = begin[i].t;
                values[i].vol = begin[i].vol;
                values[i].value = begin[i].value;
                values[i].tol = begin[i].tol;
            }
            else
            {
                int j = (i - beginSize) % updateSize;
                values[i].type = update[j].type;
                values[i].strike = update[j].strike;
                values[i].spot = update[j].spot;
                values[i].q = update[j].q;
                values[i].r = update[j].r;
                values[i].t = update[j].t;
                values[i].vol = update[j].vol;
                values[i].value = update[j].value;
                values[i].tol = update[j].tol;
            }
        }
    }

    // required for OpenACC/HMPP
    int n = NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC;

    printf("Number of options: %d\n", n);

    float* outputVals = (float*) malloc (NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC * sizeof (float));

#pragma hmpp options allocate, args[values;outputVals].size={n}
#pragma hmpp options advancedload, args[values], args[values].hostdata="values"
    
    struct timeval tv_accel_begin, tv_accel_end;
  
    printf ("Run on Accelerator\n");
    gettimeofday (&tv_accel_begin, NULL);
  
#pragma hmpp options callsite
    options (n, values, outputVals);

    gettimeofday (&tv_accel_end, NULL);

#pragma hmpp options delegatedstore, args[outputVals]

    printf ("Accelerator Time: %f ms\n", (((tv_accel_end.tv_sec - tv_accel_begin.tv_sec) * 1e6 + (tv_accel_end.tv_usec - tv_accel_begin.tv_usec)) / 1000.0f));
  
#pragma hmpp options release    

    
    float sumVals = 0.0f;
    int currVal;
    for (currVal = 0; currVal < NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC; currVal++)
    {
        sumVals += outputVals[currVal];
    }
    printf("Sum of output prices: %f\n", sumVals);
    printf("Output price at index %d: %f\n", NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC/2, outputVals[NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC/2]);

    free (outputVals);
    float* outputValsCpu = (float*) malloc (NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC * sizeof (float));
    
    struct timeval tv_cpu_begin, tv_cpu_end;
  
    printf ("Run on CPU\n");
    gettimeofday (&tv_cpu_begin, NULL);

    //process options on CPU
    options (n, values, outputValsCpu);

    gettimeofday (&tv_cpu_end, NULL);
    printf ("CPU Time: %f ms\n", (((tv_cpu_end.tv_sec - tv_cpu_begin.tv_sec) * 1e6 + (tv_cpu_end.tv_usec - tv_cpu_begin.tv_usec))  / 1000.0f));


    sumVals = 0.0f;
    for (currVal = 0; currVal < NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC; currVal++)
    {
        sumVals += outputValsCpu[currVal];
    }
    printf("Sum of output prices: %f\n", sumVals);
    printf("Output price at index %d: %f\n", NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC/2, outputValsCpu[NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC/2]);
    
    free (outputValsCpu);

    free (values);
}

