//monteCarloEngineHmppOpenACC.c
//Scott Grauer-Gray
//Main function for running monte-carlo using OpenACC

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "monteCarloEngine.c"


int
main( int argc, char** argv) 
{
  srand (time (NULL));
  
  runMonteCarlo();
  
}


