//blackScholesEngineHmppOpenCL.c
//Scott Grauer-Gray
//Main function to call computation for black-scholes using HMPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "blackScholesAnalyticEngine.c"

#define THREAD_BLOCK_SIZE 256
#define NUM_SAMPLES 439058

int
main (int argc, char** argv) 
{
  runBlackScholesAnalyticEngine ();
}
