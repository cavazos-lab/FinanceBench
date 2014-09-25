//blackScholesAnalyticEngineKernelsCpu.cu
//Scott Grauer-Gray
//Kernels for running black scholes using the analytic engine

//declarations for the kernels
#include "blackScholesAnalyticEngineKernels.h"

//needed for the constants in the error function
#include "ErrorFunctConstants.h"

//device kernel to retrieve the compound factor in interestRate
float interestRateCompoundFactor (float t, const yieldTermStruct currYieldTermStruct)
{
  return (exp ((currYieldTermStruct.forward) * t));
}

//device kernel to retrieve the discount factor in interestRate
float interestRateDiscountFactor (float t, const yieldTermStruct currYieldTermStruct)
{
  return 1.0f / interestRateCompoundFactor (t, currYieldTermStruct);
}

//device function to get the variance of the black volatility function
float getBlackVolBlackVar (const blackVolStruct volTS)
{
  float vol = volTS.volatility;
  return vol * vol * volTS.timeYearFraction;
}

//device function to get the discount on a dividend yield
float getDiscountOnDividendYield (float yearFraction, const yieldTermStruct dividendYieldTermStruct)
{
  float intDiscountFactor = interestRateDiscountFactor (yearFraction, dividendYieldTermStruct);
  return intDiscountFactor;
}


//device function to get the discount on the risk free rate
float getDiscountOnRiskFreeRate (float yearFraction, const yieldTermStruct riskFreeRateYieldTermStruct)
{
  return interestRateDiscountFactor (yearFraction, riskFreeRateYieldTermStruct);
}

//device kernel to run the error function
float errorFunct (const normalDistStruct normDist, float x)
{
  float R,S,P,Q,s,y,z,r, ax;
  
  ax = fabs (x);
  
  if (ax < 0.84375) 
    {      
      if (ax < 3.7252902984e-09) 
	{ 
	  if (ax < DBL_MIN*16)
	    {
	      return 0.125 * (8.0 * x + (ERROR_FUNCT_efx8) * x);  /*avoid underflow */
	    }
	  return x + (ERROR_FUNCT_efx) * x;
        }
      z = x*x;
      r = ERROR_FUNCT_pp0 + z * (ERROR_FUNCT_pp1 + z * (ERROR_FUNCT_pp2 + z * (ERROR_FUNCT_pp3 + z * ERROR_FUNCT_pp4)));
      s = ERROR_FUNCT_one + z * (ERROR_FUNCT_qq1 + z * (ERROR_FUNCT_qq2 + z * (ERROR_FUNCT_qq3 + z * (ERROR_FUNCT_qq4 + z * ERROR_FUNCT_qq5))));
      y = r/s;
      return x + x*y;
    }
  if(ax <1.25) 
    {      
      s = ax-ERROR_FUNCT_one;
      P = ERROR_FUNCT_pa0 + s * (ERROR_FUNCT_pa1 + s * (ERROR_FUNCT_pa2 + s * (ERROR_FUNCT_pa3 + s * (ERROR_FUNCT_pa4 + s * (ERROR_FUNCT_pa5 + s * ERROR_FUNCT_pa6)))));
      Q = ERROR_FUNCT_one + s * (ERROR_FUNCT_qa1 + s * (ERROR_FUNCT_qa2 + s * (ERROR_FUNCT_qa3 + s * (ERROR_FUNCT_qa4 + s * (ERROR_FUNCT_qa5 + s * ERROR_FUNCT_qa6)))));
      if (x >= 0)
	{
	  return ERROR_FUNCT_erx + P / Q;
	}
      else
	{
	  return -1 * ERROR_FUNCT_erx - P / Q;
	}
    }
  if (ax >= 6) 
    {      
      if(x>=0) 
	return ERROR_FUNCT_one-ERROR_FUNCT_tiny; 
      else 
	return ERROR_FUNCT_tiny-ERROR_FUNCT_one;
    }
  
  /* Starts to lose accuracy when ax~5 */
  s = ERROR_FUNCT_one/(ax*ax);
  
  if(ax < 2.85714285714285)
    {
      /* |x| < 1/0.35 */
      R = ERROR_FUNCT_ra0+s*(ERROR_FUNCT_ra1+s*(ERROR_FUNCT_ra2+s*(ERROR_FUNCT_ra3+s*(ERROR_FUNCT_ra4+s*(ERROR_FUNCT_ra5+s*(ERROR_FUNCT_ra6+s*ERROR_FUNCT_ra7))))));
      S = ERROR_FUNCT_one+s*(ERROR_FUNCT_sa1+s*(ERROR_FUNCT_sa2+s*(ERROR_FUNCT_sa3+s*(ERROR_FUNCT_sa4+s*(ERROR_FUNCT_sa5+s*(ERROR_FUNCT_sa6+s*(ERROR_FUNCT_sa7+s*ERROR_FUNCT_sa8)))))));
    }
  else
    {
      /* |x| >= 1/0.35 */
      R=ERROR_FUNCT_rb0+s*(ERROR_FUNCT_rb1+s*(ERROR_FUNCT_rb2+s*(ERROR_FUNCT_rb3+s*(ERROR_FUNCT_rb4+s*(ERROR_FUNCT_rb5+s*ERROR_FUNCT_rb6)))));
      S=ERROR_FUNCT_one+s*(ERROR_FUNCT_sb1+s*(ERROR_FUNCT_sb2+s*(ERROR_FUNCT_sb3+s*(ERROR_FUNCT_sb4+s*(ERROR_FUNCT_sb5+s*(ERROR_FUNCT_sb6+s*ERROR_FUNCT_sb7))))));
    }
  
  r = exp (-ax * ax - 0.5625 + R / S);
  if (x >= 0) 
    return ERROR_FUNCT_one - r / ax; 
  else 
    return r / ax-ERROR_FUNCT_one;
}

//device kernel to run the operator function in cumulative normal distribution
float cumNormDistOp (const normalDistStruct normDist, float z)
{
  z = (z - normDist.average) / normDist.sigma;
  float result = 0.5 * (1.0 + errorFunct(normDist, z * M_SQRT_2 ) );
  return result;
}


//device kernel to run the gaussian function in the normal distribution
float gaussianFunctNormDist (const normalDistStruct normDist, float x)
{
  float deltax = x - normDist.average;
  float exponent = -(deltax * deltax) / normDist.denominator;
  // debian alpha had some strange problem in the very-low range
  return (exponent <= -690.0) // exp(x) < 1.0e-300 anyway
    ? 0.0
    : normDist.normalizationFactor * exp (exponent);
}


//device kernel to retrieve the derivative in a cumulative normal distribution
float cumNormDistDeriv (const normalDistStruct normDist, float x)
{
  float xn = (x - normDist.average) / normDist.sigma;
  return gaussianFunctNormDist (normDist, xn) / normDist.sigma;
}



//device function to initialize variable in the black calculator
void
initBlackCalcVars (blackCalcStruct* blackCalculator, payoffStruct payoff)
{
  blackCalculator->d1 = log(blackCalculator->forward / blackCalculator->strike)/blackCalculator->stdDev + 0.5*blackCalculator->stdDev;
  blackCalculator->d2 = blackCalculator->d1 - blackCalculator->stdDev;

  //initialize the cumulative normal distribution structure
  normalDistStruct currCumNormDist;
  currCumNormDist.average = 0.0f;
  currCumNormDist.sigma = 1.0f;
  currCumNormDist.normalizationFactor = M_SQRT_2 * M_1_SQRTPI/currCumNormDist.sigma;
  currCumNormDist.derNormalizationFactor = currCumNormDist.sigma * currCumNormDist.sigma;
  currCumNormDist.denominator = 2.0 * currCumNormDist.derNormalizationFactor;
             
  blackCalculator->cum_d1 = cumNormDistOp (currCumNormDist, blackCalculator->d1);
  blackCalculator->cum_d2 = cumNormDistOp (currCumNormDist, blackCalculator->d2);
  blackCalculator->n_d1 = cumNormDistDeriv (currCumNormDist, blackCalculator->d1);
  blackCalculator->n_d2 = cumNormDistDeriv (currCumNormDist, blackCalculator->d2);

  blackCalculator->x = payoff.strike;
  blackCalculator->DxDstrike = 1.0;

  // the following one will probably disappear as soon as
  // super-share will be properly handled
  blackCalculator->DxDs = 0.0;
  
  // this part is always executed.
  // in case of plain-vanilla payoffs, it is also the only part
  // which is executed.
  if (payoff.type == CALL)
    {
      blackCalculator->alpha     =         blackCalculator->cum_d1; //  N(d1)
      blackCalculator->DalphaDd1 =         blackCalculator->n_d1;   //  n(d1)
      blackCalculator->beta      = -1.0f * blackCalculator->cum_d2; // -N(d2)
      blackCalculator->DbetaDd2  = -1.0f * blackCalculator->n_d2;   // -n(d2)
    }
  else if (payoff.type == PUT)
    {
      blackCalculator->alpha     = -1.0 + blackCalculator->cum_d1;  // -N(-d1)
      blackCalculator->DalphaDd1 =        blackCalculator->n_d1;    //  n( d1)
      blackCalculator->beta      =  1.0 - blackCalculator->cum_d2;  //  N(-d2)
      blackCalculator->DbetaDd2  = -1.0f * blackCalculator->n_d2;   // -n( d2)
    }
}


//device function to initialize the black calculator
blackCalcStruct initBlackCalculator (const payoffStruct payoff, float forwardPrice, float stdDev, float riskFreeDiscount)
{
  blackCalcStruct blackCalc;
  blackCalc.strike = payoff.strike;
  blackCalc.forward = forwardPrice;
  blackCalc.stdDev = stdDev;
  blackCalc.discount = riskFreeDiscount;
  blackCalc.variance = stdDev * stdDev;
  
  initBlackCalcVars (&blackCalc, payoff);
  
  return blackCalc;
}


//device function to retrieve the output resulting value
float getResultVal (blackCalcStruct blackCalculator)
{
  return blackCalculator.discount * (blackCalculator.forward * blackCalculator.alpha + blackCalculator.x * blackCalculator.beta);
}


//global function to retrieve the output value for an option
void getOutValOption (optionInputStruct* options, float* outputVals, int optionNum)
{
  //check if within current options
  if (optionNum < NUM_SAMPLES_BLACK_SCHOLES_ANALYTIC)
    {
      optionInputStruct threadOption = options [optionNum];
      
      payoffStruct currPayoff;
      currPayoff.type = threadOption.type;
      currPayoff.strike = threadOption.strike;
      
      yieldTermStruct qTS;
      qTS.timeYearFraction = threadOption.t;
      qTS.forward = threadOption.q;
      
      yieldTermStruct rTS;
      rTS.timeYearFraction = threadOption.t;
      rTS.forward = threadOption.r;
      
      blackVolStruct volTS;
      volTS.timeYearFraction = threadOption.t;
      volTS.volatility = threadOption.vol;
      
      blackScholesMertStruct stochProcess;
      stochProcess.x0 = threadOption.spot;
      stochProcess.dividendTS = qTS;
      stochProcess.riskFreeTS = rTS;
      stochProcess.blackVolTS = volTS;
      
      optionStruct currOption;
      currOption.payoff = currPayoff;
      currOption.yearFractionTime = threadOption.t;
      currOption.pricingEngine = stochProcess; 
      
      float variance = getBlackVolBlackVar (currOption.pricingEngine.blackVolTS);
      float dividendDiscount = getDiscountOnDividendYield (currOption.yearFractionTime, currOption.pricingEngine.dividendTS);
      float riskFreeDiscount = getDiscountOnRiskFreeRate (currOption.yearFractionTime, currOption.pricingEngine.riskFreeTS);
      float spot = currOption.pricingEngine.x0;
      
      float forwardPrice = spot * dividendDiscount / riskFreeDiscount;
      
      //declare the blackCalcStruct      
      //initialize the calculator
      blackCalcStruct blackCalc = initBlackCalculator (currOption.payoff, forwardPrice, sqrt(variance), riskFreeDiscount);
      
      //retrieve the results values
      float resultVal = getResultVal (blackCalc);
      
      //write the resulting value to global memory
      outputVals [optionNum] = resultVal;
    }
}

