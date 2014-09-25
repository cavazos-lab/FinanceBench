//blackScholesAnalyticEngineKernelsCpu.cu
//Scott Grauer-Gray
//Kernels for running black scholes using the analytic engine

#ifndef BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CU
#define BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CU

//declarations for the kernels
#include "blackScholesAnalyticEngineKernelsCpu.h"

#include "blackScholesAnalyticEngineStructs.h"


//needed for the constants in the error function
#include "errorFunctConsts.h"


//device kernel to retrieve the compound factor in interestRate
dataType interestRateCompoundFactorCpu(dataType t, yieldTermStruct currYieldTermStruct)
{
	return (exp((currYieldTermStruct.forward)*t));
}


//device kernel to retrieve the discount factor in interestRate
dataType interestRateDiscountFactorCpu(dataType t, yieldTermStruct currYieldTermStruct)
{
	return 1.0f / interestRateCompoundFactorCpu(t, currYieldTermStruct);
}


//device function to get the variance of the black volatility function
dataType getBlackVolBlackVarCpu(blackVolStruct volTS)
{
	dataType vol = volTS.volatility;
    return vol*vol*volTS.timeYearFraction;
}


//device function to get the discount on a dividend yield
dataType getDiscountOnDividendYieldCpu(dataType yearFraction, yieldTermStruct dividendYieldTermStruct)
{
	dataType intDiscountFactor = interestRateDiscountFactorCpu(yearFraction, dividendYieldTermStruct);
	return intDiscountFactor;
}


//device function to get the discount on the risk free rate
dataType getDiscountOnRiskFreeRateCpu(dataType yearFraction, yieldTermStruct riskFreeRateYieldTermStruct)
{
	return interestRateDiscountFactorCpu(yearFraction, riskFreeRateYieldTermStruct);
}


//device kernel to run the error function
dataType errorFunctCpu(normalDistStruct normDist, dataType x)
{
	dataType R,S,P,Q,s,y,z,r, ax;

    ax = fabs(x);

    if(ax < 0.84375) 
	{      
        if(ax < 3.7252902984e-09) 
		{ 
                if (ax < DBL_MIN*16)
                    return 0.125*(8.0*x+ (ERROR_FUNCT_efx8)*x);  /*avoid underflow */
	            return x + (ERROR_FUNCT_efx)*x;
        }
		z = x*x;
        r = ERROR_FUNCT_pp0+z*(ERROR_FUNCT_pp1+z*(ERROR_FUNCT_pp2+z*(ERROR_FUNCT_pp3+z*ERROR_FUNCT_pp4)));
        s = ERROR_FUNCT_one+z*(ERROR_FUNCT_qq1+z*(ERROR_FUNCT_qq2+z*(ERROR_FUNCT_qq3+z*(ERROR_FUNCT_qq4+z*ERROR_FUNCT_qq5))));
        y = r/s;
        return x + x*y;
    }
    if(ax <1.25) 
	{      
        s = ax-ERROR_FUNCT_one;
        P = ERROR_FUNCT_pa0+s*(ERROR_FUNCT_pa1+s*(ERROR_FUNCT_pa2+s*(ERROR_FUNCT_pa3+s*(ERROR_FUNCT_pa4+s*(ERROR_FUNCT_pa5+s*ERROR_FUNCT_pa6)))));
        Q = ERROR_FUNCT_one+s*(ERROR_FUNCT_qa1+s*(ERROR_FUNCT_qa2+s*(ERROR_FUNCT_qa3+s*(ERROR_FUNCT_qa4+s*(ERROR_FUNCT_qa5+s*ERROR_FUNCT_qa6)))));
        if(x>=0) return ERROR_FUNCT_erx + P/Q; else return -1*ERROR_FUNCT_erx - P/Q;
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

    if(ax < 2.85714285714285) { /* |x| < 1/0.35 */
        R = ERROR_FUNCT_ra0+s*(ERROR_FUNCT_ra1+s*(ERROR_FUNCT_ra2+s*(ERROR_FUNCT_ra3+s*(ERROR_FUNCT_ra4+s*(ERROR_FUNCT_ra5+s*(ERROR_FUNCT_ra6+s*ERROR_FUNCT_ra7))))));
        S = ERROR_FUNCT_one+s*(ERROR_FUNCT_sa1+s*(ERROR_FUNCT_sa2+s*(ERROR_FUNCT_sa3+s*(ERROR_FUNCT_sa4+s*(ERROR_FUNCT_sa5+s*(ERROR_FUNCT_sa6+s*(ERROR_FUNCT_sa7+s*ERROR_FUNCT_sa8)))))));
    } else {    /* |x| >= 1/0.35 */
        R=ERROR_FUNCT_rb0+s*(ERROR_FUNCT_rb1+s*(ERROR_FUNCT_rb2+s*(ERROR_FUNCT_rb3+s*(ERROR_FUNCT_rb4+s*(ERROR_FUNCT_rb5+s*ERROR_FUNCT_rb6)))));
        S=ERROR_FUNCT_one+s*(ERROR_FUNCT_sb1+s*(ERROR_FUNCT_sb2+s*(ERROR_FUNCT_sb3+s*(ERROR_FUNCT_sb4+s*(ERROR_FUNCT_sb5+s*(ERROR_FUNCT_sb6+s*ERROR_FUNCT_sb7))))));
    }

    r = exp( -ax*ax-0.5625 +R/S);
    if(x>=0) 
		return ERROR_FUNCT_one-r/ax; 
	else 
		return r/ax-ERROR_FUNCT_one;
}



//device kernel to run the operator function in cumulative normal distribution
dataType cumNormDistOpCpu(normalDistStruct normDist, dataType z)
{
	z = (z - normDist.average) / normDist.sigma;
    dataType result = 0.5 * ( 1.0 + errorFunctCpu(normDist, z*M_SQRT_2 ) );
	return result;
}


//device kernel to run the gaussian function in the normal distribution
dataType gaussianFunctNormDistCpu(normalDistStruct normDist, dataType x)
{
	dataType deltax = x - normDist.average;
	dataType exponent = -(deltax*deltax)/normDist.denominator;
    // debian alpha had some strange problem in the very-low range
    return exponent <= -690.0 ? 0.0 :  // exp(x) < 1.0e-300 anyway
            normDist.normalizationFactor * exp(exponent);
}


//device kernel to retrieve the derivative in a cumulative normal distribution
dataType cumNormDistDerivCpu(normalDistStruct normDist, dataType x)
{
	dataType xn = (x - normDist.average) / normDist.sigma;
    return gaussianFunctNormDistCpu(normDist, xn) / normDist.sigma;
}


//device function to initialize the cumulative normal distribution structure
void initCumNormDistCpu(normalDistStruct& currCumNormDist)
{
	currCumNormDist.average = 0.0f;
	currCumNormDist.sigma = 1.0f;
	currCumNormDist.normalizationFactor = M_SQRT_2*M_1_SQRTPI/currCumNormDist.sigma;
    currCumNormDist.derNormalizationFactor = currCumNormDist.sigma*currCumNormDist.sigma;
    currCumNormDist.denominator = 2.0*currCumNormDist.derNormalizationFactor;
}


//device function to initialize variable in the black calculator
void initBlackCalcVarsCpu(blackCalcStruct& blackCalculator, payoffStruct payoff)
{
    blackCalculator.d1 = log(blackCalculator.forward / blackCalculator.strike)/blackCalculator.stdDev + 0.5*blackCalculator.stdDev;
    blackCalculator.d2 = blackCalculator.d1 - blackCalculator.stdDev;

	//initialize the cumulative normal distribution structure
	normalDistStruct currCumNormDist;// = new normalDistStruct;//(normalDistStruct*)malloc(sizeof(normalDistStruct));
	initCumNormDistCpu(currCumNormDist);
                
	blackCalculator.cum_d1 = cumNormDistOpCpu(currCumNormDist, blackCalculator.d1);
	blackCalculator.cum_d2 = cumNormDistOpCpu(currCumNormDist, blackCalculator.d2);
	blackCalculator.n_d1 = cumNormDistDerivCpu(currCumNormDist, blackCalculator.d1);
	blackCalculator.n_d2 = cumNormDistDerivCpu(currCumNormDist, blackCalculator.d2);

	//delete currCumNormDist;

	blackCalculator.x = payoff.strike;
	blackCalculator.DxDstrike = 1.0;

	// the following one will probably disappear as soon as
	// super-share will be properly handled
	blackCalculator.DxDs = 0.0;

	// this part is always executed.
	// in case of plain-vanilla payoffs, it is also the only part
	// which is executed.
	switch (payoff.type) 
	{
		case CALL:
			blackCalculator.alpha     =  blackCalculator.cum_d1;//  N(d1)
            blackCalculator.DalphaDd1 =    blackCalculator.n_d1;//  n(d1)
            blackCalculator.beta      = -1.0f*blackCalculator.cum_d2;// -N(d2)
            blackCalculator.DbetaDd2  = -1.0f*blackCalculator.n_d2;// -n(d2)
            break;
        case PUT:
            blackCalculator.alpha     = -1.0+blackCalculator.cum_d1;// -N(-d1)
            blackCalculator.DalphaDd1 =        blackCalculator.n_d1;//  n( d1)
            blackCalculator.beta      =  1.0-blackCalculator.cum_d2;//  N(-d2)
            blackCalculator.DbetaDd2  =     -1.0f* blackCalculator.n_d2;// -n( d2)
            break;
	}
}


//device function to initialize the black calculator
void initBlackCalculatorCpu(blackCalcStruct& blackCalc, payoffStruct payoff, dataType forwardPrice, dataType stdDev, dataType riskFreeDiscount)
{
	blackCalc.strike = payoff.strike;
	blackCalc.forward = forwardPrice;
	blackCalc.stdDev = stdDev;
	blackCalc.discount = riskFreeDiscount;
	blackCalc.variance = stdDev * stdDev;

	initBlackCalcVarsCpu(blackCalc, payoff);
}


//device function to retrieve the output resulting value
dataType getResultValCpu(blackCalcStruct blackCalculator)
{
	dataType result = blackCalculator.discount * (blackCalculator.forward * 
					blackCalculator.alpha + blackCalculator.x * blackCalculator.beta);
	return result;
}


//global function to retrieve the output value for an option
void getOutValOptionCpu(optionInputStruct* options, dataType* outputVals, int optionNum, int numVals)
{
	//check if within current options
	if (optionNum < numVals)
	{
		optionInputStruct threadOption = options[optionNum];

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

		dataType variance = getBlackVolBlackVarCpu(currOption.pricingEngine.blackVolTS);
		dataType dividendDiscount = getDiscountOnDividendYieldCpu(currOption.yearFractionTime, currOption.pricingEngine.dividendTS);
		dataType riskFreeDiscount = getDiscountOnRiskFreeRateCpu(currOption.yearFractionTime, currOption.pricingEngine.riskFreeTS);
		dataType spot = currOption.pricingEngine.x0;

		dataType forwardPrice = spot * dividendDiscount / riskFreeDiscount;

		//declare the blackCalcStruct
		blackCalcStruct blackCalc;

		//initialize the calculator
		initBlackCalculatorCpu(blackCalc, currOption.payoff, forwardPrice, sqrt(variance), riskFreeDiscount);

		//retrieve the results values
		dataType resultVal = getResultValCpu(blackCalc);

		//write the resulting value to global memory
		outputVals[optionNum] = resultVal;
	}
}

#endif //BLACK_SCHOLES_ANALYTIC_ENGINE_KERNELS_CPU_CU
