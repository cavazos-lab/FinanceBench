//monteCarloEngineHmppCuda.c
//Scott Grauer-Gray
//Main function to drive monte-carlo on the gpu using HMPP

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "monteCarloEngine.c"


////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
  srand (time (NULL));
  
  runMonteCarlo();
  
}


