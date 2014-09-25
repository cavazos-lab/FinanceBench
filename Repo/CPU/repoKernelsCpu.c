//repoKernelsCpu.cu
//Scott Grauer-Gray
//July 6, 2012
//Repo kernels on the CPU

#include "repoKernelsCpu.h"


int monthLengthKernelCpu(int month, bool leapYear) 
{
	int MonthLength[] = {
			31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
    
	int MonthLeapLength[] = {
			31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
    
	return (leapYear? MonthLeapLength[month-1] : MonthLength[month-1]);
}


int monthOffsetKernelCpu(int m, bool leapYear) 
{
	int MonthOffset[] = {
			0,  31,  59,  90, 120, 151,   // Jan - Jun
			181, 212, 243, 273, 304, 334,   // Jun - Dec
			365     // used in dayOfMonth to bracket day
	};
    
	int MonthLeapOffset[] = {
			0,  31,  60,  91, 121, 152,   // Jan - Jun
			182, 213, 244, 274, 305, 335,   // Jun - Dec
			366     // used in dayOfMonth to bracket day
		};

	return (leapYear? MonthLeapOffset[m-1] : MonthOffset[m-1]);
}


int yearOffsetKernelCpu(int y)
{
        // the list of all December 31st in the preceding year
        // e.g. for 1901 yearOffset[1] is 366, that is, December 31 1900
        int YearOffset[] = {
            // 1900-1909
                0,  366,  731, 1096, 1461, 1827, 2192, 2557, 2922, 3288,
            // 1910-1919
             3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940,
            // 1920-1929
             7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862,10227,10593,
            // 1930-1939
            10958,11323,11688,12054,12419,12784,13149,13515,13880,14245,
            // 1940-1949
            14610,14976,15341,15706,16071,16437,16802,17167,17532,17898,
            // 1950-1959
            18263,18628,18993,19359,19724,20089,20454,20820,21185,21550,
            // 1960-1969
            21915,22281,22646,23011,23376,23742,24107,24472,24837,25203,
            // 1970-1979
            25568,25933,26298,26664,27029,27394,27759,28125,28490,28855,
            // 1980-1989
            29220,29586,29951,30316,30681,31047,31412,31777,32142,32508,
            // 1990-1999
            32873,33238,33603,33969,34334,34699,35064,35430,35795,36160,
            // 2000-2009
            36525,36891,37256,37621,37986,38352,38717,39082,39447,39813,
            // 2010-2019
            40178,40543,40908,41274,41639,42004,42369,42735,43100,43465,
            // 2020-2029
            43830,44196,44561,44926,45291,45657,46022,46387,46752,47118,
            // 2030-2039
            47483,47848,48213,48579,48944,49309,49674,50040,50405,50770,
            // 2040-2049
            51135,51501,51866,52231,52596,52962,53327,53692,54057,54423,
            // 2050-2059
            54788,55153,55518,55884,56249,56614,56979,57345,57710,58075,
            // 2060-2069
            58440,58806,59171,59536,59901,60267,60632,60997,61362,61728,
            // 2070-2079
            62093,62458,62823,63189,63554,63919,64284,64650,65015,65380,
            // 2080-2089
            65745,66111,66476,66841,67206,67572,67937,68302,68667,69033,
            // 2090-2099
            69398,69763,70128,70494,70859,71224,71589,71955,72320,72685,
            // 2100-2109
            73050,73415,73780,74145,74510,74876,75241,75606,75971,76337,
            // 2110-2119
            76702,77067,77432,77798,78163,78528,78893,79259,79624,79989,
            // 2120-2129
            80354,80720,81085,81450,81815,82181,82546,82911,83276,83642,
            // 2130-2139
            84007,84372,84737,85103,85468,85833,86198,86564,86929,87294,
            // 2140-2149
            87659,88025,88390,88755,89120,89486,89851,90216,90581,90947,
            // 2150-2159
            91312,91677,92042,92408,92773,93138,93503,93869,94234,94599,
            // 2160-2169
            94964,95330,95695,96060,96425,96791,97156,97521,97886,98252,
            // 2170-2179
            98617,98982,99347,99713,100078,100443,100808,101174,101539,101904,
            // 2180-2189
            102269,102635,103000,103365,103730,104096,104461,104826,105191,105557,
            // 2190-2199
            105922,106287,106652,107018,107383,107748,108113,108479,108844,109209,
            // 2200
            109574
        };

        return YearOffset[y-1900];
}


bool isLeapKernelCpu(int y) 
{
        bool YearIsLeap[] = {
            // 1900 is leap in agreement with Excel's bug
            // 1900 is out of valid date range anyway
            // 1900-1909
             true,false,false,false, true,false,false,false, true,false,
            // 1910-1919
            false,false, true,false,false,false, true,false,false,false,
            // 1920-1929
             true,false,false,false, true,false,false,false, true,false,
            // 1930-1939
            false,false, true,false,false,false, true,false,false,false,
            // 1940-1949
             true,false,false,false, true,false,false,false, true,false,
            // 1950-1959
            false,false, true,false,false,false, true,false,false,false,
            // 1960-1969
             true,false,false,false, true,false,false,false, true,false,
            // 1970-1979
            false,false, true,false,false,false, true,false,false,false,
            // 1980-1989
             true,false,false,false, true,false,false,false, true,false,
            // 1990-1999
            false,false, true,false,false,false, true,false,false,false,
            // 2000-2009
             true,false,false,false, true,false,false,false, true,false,
            // 2010-2019
            false,false, true,false,false,false, true,false,false,false,
            // 2020-2029
             true,false,false,false, true,false,false,false, true,false,
            // 2030-2039
            false,false, true,false,false,false, true,false,false,false,
            // 2040-2049
             true,false,false,false, true,false,false,false, true,false,
            // 2050-2059
            false,false, true,false,false,false, true,false,false,false,
            // 2060-2069
             true,false,false,false, true,false,false,false, true,false,
            // 2070-2079
            false,false, true,false,false,false, true,false,false,false,
            // 2080-2089
             true,false,false,false, true,false,false,false, true,false,
            // 2090-2099
            false,false, true,false,false,false, true,false,false,false,
            // 2100-2109
            false,false,false,false, true,false,false,false, true,false,
            // 2110-2119
            false,false, true,false,false,false, true,false,false,false,
            // 2120-2129
             true,false,false,false, true,false,false,false, true,false,
            // 2130-2139
            false,false, true,false,false,false, true,false,false,false,
            // 2140-2149
             true,false,false,false, true,false,false,false, true,false,
            // 2150-2159
            false,false, true,false,false,false, true,false,false,false,
            // 2160-2169
             true,false,false,false, true,false,false,false, true,false,
            // 2170-2179
            false,false, true,false,false,false, true,false,false,false,
            // 2180-2189
             true,false,false,false, true,false,false,false, true,false,
            // 2190-2199
            false,false, true,false,false,false, true,false,false,false,
            // 2200
            false
        };

	return YearIsLeap[y-1900];
}


repoDateStruct intializeDateKernelCpu(int d, int m, int y) 
{
	repoDateStruct currDate;

	currDate.day = d;
	currDate.month = m;
	currDate.year = y;

	bool leap = isLeapKernelCpu(y);
	int offset = monthOffsetKernelCpu(m,leap);

	currDate.dateSerialNum = d + offset + yearOffsetKernelCpu(y);

	return currDate;
}


dataType yearFractionCpu(repoDateStruct d1,
					repoDateStruct d2, int dayCounter)
{
	return dayCountCpu(d1, d2, dayCounter) / 360.0; 
}


int dayCountCpu(repoDateStruct d1, repoDateStruct d2, int dayCounter) 
{
	if (dayCounter == USE_EXACT_DAY)
	{
		int dd1 = d1.day, dd2 = d2.day;
		int mm1 = d1.month, mm2 = d2.month;
		int yy1 = d1.year, yy2 = d2.year;

		if (dd2 == 31 && dd1 < 30) 
		{ 
			dd2 = 1; mm2++; 
		}
        
		return 360*(yy2-yy1) + 30*(mm2-mm1-1) + MAX(0, 30-dd1) + MIN(30, dd2);
	}

	else
	{
		return (d2.dateSerialNum - d1.dateSerialNum);
	}
}


dataType couponNotionalCpu()
{
	return 100.0;
}


dataType bondNotionalCpu()
{
	return 100.0;
}


dataType fixedRateCouponNominalCpu()
{
	return 100.0;
}

bool eventHasOccurredCpu(repoDateStruct currDate, repoDateStruct eventDate)
{
         return eventDate.dateSerialNum > currDate.dateSerialNum;
}


bool cashFlowHasOccurredCpu(repoDateStruct refDate, repoDateStruct eventDate)
{
        return eventHasOccurredCpu(refDate, eventDate);
}



repoDateStruct advanceDateCpu(repoDateStruct date, int numMonthsAdvance) 
{
	int d = date.day;
	int m = date.month+numMonthsAdvance;
	int y = date.year;

	while (m > 12) 
	{
		m -= 12;
		y += 1;
	}

	while (m < 1) 
	{
		m += 12;
		y -= 1;
	}

	int length = monthLengthKernelCpu(m, isLeapKernelCpu(y));
	if (d > length)
		d = length;

	repoDateStruct newDate = intializeDateKernelCpu(d, m, y);

	return newDate;
}

int getNumCashFlowsCpu(inArgsStruct inArgs, int repoNum)
{
	int numCashFlows = 0;
	
	//repoDateStruct endDate = inArgs.bond[repoNum].maturityDate;
	repoDateStruct currCashflowDate = inArgs.bond[repoNum].maturityDate;

	while (currCashflowDate.dateSerialNum > inArgs.bond[repoNum].startDate.dateSerialNum)
	{
		numCashFlows++;
		currCashflowDate = advanceDateCpu(currCashflowDate, -6); 
	}

	return numCashFlows+1;
}


void getRepoResultsGpuCpu(inArgsStruct inArgs, resultsStruct results, int totNumRuns)
{
	for (int repoNum=0; repoNum < totNumRuns; repoNum++)
	{
		int numLegs = getNumCashFlowsCpu(inArgs, repoNum);
		cashFlowsStruct cashFlows;  
		cashFlows.legs = (couponStruct*)malloc(numLegs*sizeof(couponStruct));

		cashFlows.intRate.dayCounter = USE_EXACT_DAY;
		cashFlows.intRate.rate = inArgs.bond[repoNum].rate;
		cashFlows.intRate.freq = ANNUAL_FREQ;
		cashFlows.intRate.comp = SIMPLE_INTEREST;
		cashFlows.dayCounter = USE_EXACT_DAY;
		cashFlows.nominal = 100.0;

		//repoDateStruct currPaymentDate;
		repoDateStruct currStartDate = advanceDateCpu(inArgs.bond[repoNum].maturityDate, (numLegs - 1)*-6);
		repoDateStruct currEndDate = advanceDateCpu(currStartDate, 6); 

		for (int cashFlowNum = 0; cashFlowNum < numLegs-1; cashFlowNum++)
		{
			cashFlows.legs[cashFlowNum].paymentDate = currEndDate;
			
			cashFlows.legs[cashFlowNum].accrualStartDate = currStartDate;
			cashFlows.legs[cashFlowNum].accrualEndDate = currEndDate;
			
			cashFlows.legs[cashFlowNum].amount = COMPUTE_AMOUNT;

			currStartDate = currEndDate;
			currEndDate = advanceDateCpu(currEndDate, 6); 
		}

		cashFlows.legs[numLegs-1].paymentDate = inArgs.bond[repoNum].maturityDate;
		cashFlows.legs[numLegs-1].accrualStartDate = inArgs.settlementDate[repoNum];
		cashFlows.legs[numLegs-1].accrualEndDate = inArgs.settlementDate[repoNum];
		cashFlows.legs[numLegs-1].amount = 100.0;


		results.bondForwardVal[repoNum] = getBondYieldCpu(inArgs.bondCleanPrice[repoNum],
                     USE_EXACT_DAY,
                     COMPOUNDED_INTEREST,
                     2.0,
                     inArgs.settlementDate[repoNum],
                     ACCURACY,
                     100,
		     inArgs, repoNum, cashFlows, numLegs);
		inArgs.discountCurve[repoNum].forward = results.bondForwardVal[repoNum];
		results.dirtyPrice[repoNum] = getDirtyPriceCpu(inArgs, repoNum, cashFlows, numLegs);
		results.accruedAmountSettlement[repoNum] = getAccruedAmountCpu(inArgs, inArgs.settlementDate[repoNum], repoNum, cashFlows, numLegs);
		results.accruedAmountDeliveryDate[repoNum] = getAccruedAmountCpu(inArgs, inArgs.deliveryDate[repoNum], repoNum, cashFlows, numLegs);
		results.cleanPrice[repoNum] = results.dirtyPrice[repoNum] - results.accruedAmountSettlement[repoNum];
		results.forwardSpotIncome[repoNum] = fixedRateBondForwardSpotIncomeCpu(inArgs, repoNum, cashFlows, numLegs);
		results.underlyingBondFwd[repoNum] = results.forwardSpotIncome[repoNum] / repoYieldTermStructureDiscountCpu(inArgs.repoCurve[repoNum], inArgs.repoDeliveryDate[repoNum]);
		dataType forwardVal = (results.dirtyPrice[repoNum] - results.forwardSpotIncome[repoNum]) / repoYieldTermStructureDiscountCpu(inArgs.repoCurve[repoNum], inArgs.repoDeliveryDate[repoNum]);
		
		results.repoNpv[repoNum] = (forwardVal - inArgs.dummyStrike[repoNum]) * repoYieldTermStructureDiscountCpu(inArgs.repoCurve[repoNum], inArgs.repoDeliveryDate[repoNum]);
		results.repoCleanForwardPrice[repoNum] = forwardVal - getAccruedAmountCpu(inArgs, inArgs.repoDeliveryDate[repoNum], repoNum, cashFlows, numLegs);
		results.repoDirtyForwardPrice[repoNum] = forwardVal;
		results.repoImpliedYield[repoNum] = getImpliedYieldCpu(inArgs, inArgs.dummyStrike[repoNum], results.dirtyPrice[repoNum], results.forwardSpotIncome[repoNum], repoNum);
		results.marketRepoRate[repoNum] = getMarketRepoRateCpu(inArgs.repoDeliveryDate[repoNum],
                                   SIMPLE_INTEREST,
                                   1.0,
								   inArgs.settlementDate[repoNum],
								   inArgs, repoNum);

		
		free(cashFlows.legs);
	}
}


dataType getDirtyPriceCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalCpu();
	return discountingBondEngineCalculateSettlementValueCpu(inArgs, repoNum, cashFlows, numLegs) * 100.0 / currentNotional;
}


dataType getAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
	return bondAccruedAmountCpu(inArgs, date, repoNum, cashFlows, numLegs);
}


dataType discountingBondEngineCalculateSettlementValueCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs)  
{
	repoDateStruct settlementDate = inArgs.settlementDate[repoNum];
		
	if (settlementDate.dateSerialNum < inArgs.bond[repoNum].startDate.dateSerialNum)
	{
		settlementDate = inArgs.bond[repoNum].startDate;
	}


	// a bond's cashflow on settlement date is never taken into account
	return cashFlowsNpvCpu(cashFlows,
                               inArgs.discountCurve[repoNum],
                               false,
                               settlementDate,
                               settlementDate, 
			       numLegs);
}


dataType bondAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalCpu();
	if (currentNotional == 0.0)
		return 0.0;
		
	return bondFunctionsAccruedAmountCpu(inArgs, date, repoNum, cashFlows, numLegs);
}


dataType bondFunctionsAccruedAmountCpu(inArgsStruct inArgs, repoDateStruct date, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
        return cashFlowsAccruedAmountCpu(cashFlows,
                                        false, date, numLegs, inArgs, repoNum) *
            100.0 / bondNotionalCpu();
}




dataType cashFlowsAccruedAmountCpu(cashFlowsStruct cashFlows,
                                  bool includeSettlementDateFlows,
                                  repoDateStruct settlementDate,
								  int numLegs, inArgsStruct inArgs, int repoNum) 
{
	int legComputeNum = cashFlowsNextCashFlowNumCpu(cashFlows,
                            settlementDate, numLegs); 

        dataType result = 0.0;


        for (int i = legComputeNum; i < (numLegs); ++i)
	{
                result += fixedRateCouponAccruedAmountCpu(cashFlows, i, settlementDate, inArgs, repoNum);
        }

        return result;
}


dataType fixedRateCouponAccruedAmountCpu(cashFlowsStruct cashFlows, int numLeg, repoDateStruct d, inArgsStruct inArgs, int repoNum) 
{
	if (d.dateSerialNum <= cashFlows.legs[numLeg].accrualStartDate.dateSerialNum || d.dateSerialNum > inArgs.maturityDate[repoNum].dateSerialNum) 
	{
		return 0.0;
	}
	else
	{
		repoDateStruct endDate = cashFlows.legs[numLeg].accrualEndDate;
		if (d.dateSerialNum < cashFlows.legs[numLeg].accrualEndDate.dateSerialNum)
		{
			endDate = d;
		}

		return fixedRateCouponNominalCpu()*(interestRateCompoundFactorCpu(cashFlows.intRate, cashFlows.legs[numLeg].accrualStartDate, endDate, cashFlows.dayCounter) - 1.0);
	}
}


dataType cashFlowsNpvCpu(cashFlowsStruct cashFlows,
                        repoYieldTermStruct discountCurve,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
						int numLegs) 
{
    	npvDate = settlementDate;

      	dataType totalNPV = 0.0;

        for (int i=0; i<numLegs; ++i) {

	

            if (!(cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlementDate)))
                totalNPV += fixedRateCouponAmountCpu(cashFlows, i) *
                            repoYieldTermStructureDiscountCpu(discountCurve, cashFlows.legs[i].paymentDate);
        }

	return totalNPV/repoYieldTermStructureDiscountCpu(discountCurve, npvDate);
}




dataType repoYieldTermStructureDiscountCpu(repoYieldTermStruct ytStruct, repoDateStruct t)
{
	ytStruct.intRate.rate = ytStruct.forward;
	ytStruct.intRate.freq = ytStruct.frequency;
	ytStruct.intRate.comp = ytStruct.compounding;
	return flatForwardDiscountImplCpu(ytStruct.intRate, yearFractionCpu(ytStruct.refDate, t, ytStruct.dayCounter));
}


dataType flatForwardDiscountImplCpu(intRateStruct intRate, dataType t) 
{
	return interestRateDiscountFactorCpu(intRate, t);
}



dataType interestRateDiscountFactorCpu(intRateStruct intRate, dataType t) 
{
	return 1.0/interestRateCompoundFactorCpu(intRate, t);
}


dataType interestRateCompoundFactorCpu(intRateStruct intRate, dataType t) 
{
	switch (intRate.comp) 
	{
          case SIMPLE_INTEREST:
            return 1.0 + intRate.rate*t;
          case COMPOUNDED_INTEREST:
            return pow(1.0f+intRate.rate/intRate.freq, intRate.freq*t);
          case CONTINUOUS_INTEREST:
            return exp(intRate.rate*t);
          //case SimpleThenCompounded:
          //  if (t<=1.0/Real(freq_))
          //      return 1.0 + intRate.rate*t;
          //  else
          //      return pow(1.0+r_/freq_, freq_*t);
	}

	return 0.0f;
}


dataType fixedRateCouponAmountCpu(cashFlowsStruct cashFlows, int numLeg) 
{
	if (cashFlows.legs[numLeg].amount == COMPUTE_AMOUNT)
	{
		return fixedRateCouponNominalCpu()*(interestRateCompoundFactorCpu(cashFlows.intRate, 
			cashFlows.legs[numLeg].accrualStartDate, cashFlows.legs[numLeg].accrualEndDate, 
			cashFlows.dayCounter) - 1.0);
	}
	else
	{
		return cashFlows.legs[numLeg].amount;
	}
}

dataType interestRateCompoundFactorCpu(intRateStruct intRate, repoDateStruct d1,
				                           repoDateStruct d2, int dayCounter)
{
	dataType t = yearFractionCpu(d1, d2, dayCounter);
	return interestRateCompoundFactorCpu(intRate, t);
}



dataType fixedRateBondForwardSpotIncomeCpu(inArgsStruct inArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs) 
{
        dataType income = 0.0;
        repoDateStruct settlement = inArgs.settlementDate[repoNum];
	repoDateStruct maturityDate = inArgs.deliveryDate[repoNum];
	repoYieldTermStruct repoCurve = inArgs.repoCurve[repoNum];

     
        for (int i = 0; i < numLegs; ++i) 
	{	
            	if (!cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlement))
		{
                	if (cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, maturityDate)) 
			{
                    		income += fixedRateCouponAmountCpu(cashFlows, i) *
					  repoYieldTermStructureDiscountCpu(repoCurve, cashFlows.legs[i].paymentDate);
                	} 
			else 
			{
                    		break;
                	}
            	}
        }

        return income;
}


dataType getImpliedYieldCpu(inArgsStruct inArgs, dataType forwardValue, dataType underlyingSpotValue, dataType spotIncomeIncDiscCurve, int repoNum)
{
	dataType t = yearFractionCpu(inArgs.settlementDate[repoNum], inArgs.repoDeliveryDate[repoNum], inArgs.repoCurve[repoNum].dayCounter) ;
        dataType compoundingFactor = forwardValue/
            			     (underlyingSpotValue-spotIncomeIncDiscCurve) ;
        return interestRateImpliedRateCpu(compoundingFactor,                                        
                              SIMPLE_INTEREST,
                              1.0,
                              t);
}


dataType interestRateImpliedRateCpu(dataType compound,                                        
                              int comp,
                              dataType freq,
                              dataType t) 
{
        dataType r = 0.0f;
        if (compound==1.0) 
	{    
            	r = 0.0;
        } 
	else 
	{    
            	switch (comp)
		{
              		case SIMPLE_INTEREST:
                		r = (compound - 1.0)/t;
                		break;
              		case COMPOUNDED_INTEREST:
                		r = (pow((dataType)compound, 1.0f/((freq)*t))-1.0f)*(freq);
                		break;
		}
        }

        return r;
}


dataType getMarketRepoRateCpu(repoDateStruct d,
                              int comp,
                              dataType freq,
			      repoDateStruct referenceDate,
			      inArgsStruct inArgs, int repoNum)
{
	dataType compound = 1.0/repoYieldTermStructureDiscountCpu(inArgs.repoCurve[repoNum], d);
    	return interestRateImpliedRateCpu(compound,
                                   	  comp, freq,
                                   	  yearFractionCpu(referenceDate, d, inArgs.repoCurve[repoNum].dayCounter));
}


couponStruct cashFlowsNextCashFlowCpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs) 
{
        for (int i = 0; i < numLegs; ++i) 
	{
        	if ( ! (cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlementDate) ))
                	return cashFlows.legs[i];
        }
        return cashFlows.legs[numLegs-1];
}


int cashFlowsNextCashFlowNumCpu(cashFlowsStruct cashFlows,
                            repoDateStruct settlementDate,
							int numLegs) 
{
        for (int i = 0; i < numLegs; ++i) 
	{
        	if ( ! (cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlementDate) ))
                	return i;
        }
        
	return (numLegs-1);
}


dataType getBondYieldCpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
		     inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalCpu();

	if (currentNotional == 0.0)
		return 0.0;

	if (currInArgs.bond[repoNum].startDate.dateSerialNum > settlement.dateSerialNum)
	{
		settlement = currInArgs.bond[repoNum].startDate;
	}

	return getBondFunctionsYieldCpu(cleanPrice, dc, comp, freq,
								settlement, accuracy, maxEvaluations,
								currInArgs, repoNum, cashFlows, numLegs);
}


dataType getBondFunctionsYieldCpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     repoDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
		     inArgsStruct currInArgs, int repoNum, cashFlowsStruct cashFlows, int numLegs)
{
        dataType dirtyPrice = cleanPrice + bondFunctionsAccruedAmountCpu(currInArgs, settlement, repoNum, cashFlows, numLegs); 
        dirtyPrice /= 100.0 / bondNotionalCpu();

        return getCashFlowsYieldCpu(cashFlows, dirtyPrice,
                                dc, comp, freq,
                                false, settlement, settlement, numLegs,
                                accuracy, maxEvaluations);
}


dataType getCashFlowsYieldCpu(cashFlowsStruct leg,
                          dataType npv,
                          int dayCounter,
                          int compounding,
                          dataType frequency,
                          bool includeSettlementDateFlows,
                          repoDateStruct settlementDate,
                          repoDateStruct npvDate,
						  int numLegs,
                          dataType accuracy,
                          int maxIterations,
                          dataType guess)
{
	//Brent solver;
	solverStruct solver;
	solver.maxEvaluations_ = maxIterations;
	irrFinderStruct objFunction;

	objFunction.npv = npv;
	objFunction.dayCounter = dayCounter;
	objFunction.comp = compounding;
	objFunction.freq = frequency;
	objFunction.includeSettlementDateFlows = includeSettlementDateFlows;
	objFunction.settlementDate = settlementDate;
	objFunction.npvDate = npvDate;

	return solverSolveCpu(solver, objFunction, accuracy, guess, guess/10.0, leg, numLegs);
}


dataType solverSolveCpu(solverStruct solver,
					irrFinderStruct f,
					dataType accuracy,
					dataType guess,
					dataType step,
					cashFlowsStruct cashFlows,
					int numLegs)
{
	// check whether we really want to use epsilon
	accuracy = MAX(accuracy, QL_EPSILON_GPU);

	dataType growthFactor = 1.6;
	int flipflop = -1;

	solver.root_ = guess;
	solver.fxMax_ = fOpCpu(f, solver.root_, cashFlows, numLegs);

	// monotonically crescent bias, as in optionValue(volatility)
	if (closeCpu(solver.fxMax_,0.0))
	{
		return solver.root_;
	}
	else if (closeCpu(solver.fxMax_, 0.0)) 
	{
		solver.xMin_ = /*enforceBounds*/(solver.root_ - step);
		solver.fxMin_ = fOpCpu(f, solver.xMin_, cashFlows, numLegs);
		solver.xMax_ = solver.root_;
	} 
	else 
	{
		solver.xMin_ = solver.root_;
		solver.fxMin_ = solver.fxMax_;
		solver.xMax_ = /*enforceBounds*/(solver.root_+step);
		solver.fxMax_ = fOpCpu(f, solver.xMax_, cashFlows, numLegs);
	}

	solver.evaluationNumber_ = 2;
	while (solver.evaluationNumber_ <= solver.maxEvaluations_) 
	{
		if (solver.fxMin_*solver.fxMax_ <= 0.0) 
		{
			if (closeCpu(solver.fxMin_, 0.0))
				return solver.xMin_;
			if (closeCpu(solver.fxMax_, 0.0))
				return solver.xMax_;
			solver.root_ = (solver.xMax_+solver.xMin_)/2.0;
			return solveImplCpu(solver, f, accuracy, cashFlows, numLegs);
		}
		if (fabs(solver.fxMin_) < fabs(solver.fxMax_)) 
		{
			solver.xMin_ = /*enforceBounds*/(solver.xMin_+growthFactor*(solver.xMin_ - solver.xMax_));
			solver.fxMin_= fOpCpu(f, solver.xMin_, cashFlows, numLegs);
		} 
		else if (fabs(solver.fxMin_) > fabs(solver.fxMax_)) 
		{
			solver.xMax_ = /*enforceBounds*/(solver.xMax_+growthFactor*(solver.xMax_ - solver.xMin_));
			solver.fxMax_= fOpCpu(f, solver.xMax_, cashFlows, numLegs);
		} 
		else if (flipflop == -1) 
		{
			solver.xMin_ = /*enforceBounds*/(solver.xMin_+growthFactor*(solver.xMin_ - solver.xMax_));
			solver.fxMin_= fOpCpu(f, solver.xMin_, cashFlows, numLegs);
			solver.evaluationNumber_++;
			flipflop = 1;
		} 
		else if (flipflop == 1) 
		{
			solver.xMax_ = /*enforceBounds*/(solver.xMax_+growthFactor*(solver.xMax_ - solver.xMin_));
			solver.fxMax_= fOpCpu(f, solver.xMax_, cashFlows, numLegs);
			flipflop = -1;
		}
		solver.evaluationNumber_++;
	}

	return 0.0f;
}


dataType cashFlowsNpvYieldCpu(cashFlowsStruct cashFlows,
                        intRateStruct y,
                        bool includeSettlementDateFlows,
                        repoDateStruct settlementDate,
                        repoDateStruct npvDate,
						int numLegs) 
{
       	dataType npv = 0.0;
        dataType discount = 1.0;
        repoDateStruct lastDate;
	bool first = true;
		
        for (int i=0; i<numLegs; ++i) 
	{
            	if (cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlementDate))
            	    	continue;

            	repoDateStruct couponDate = cashFlows.legs[i].paymentDate;
            	dataType amount = fixedRateCouponAmountCpu(cashFlows, i);
            	if (first) 
		{
			first = false;
			
			if (i > 0) 
			{
                    		lastDate = advanceDateCpu(cashFlows.legs[i].paymentDate, -1*6);
                	} 
			else 
			{
                        	lastDate = cashFlows.legs[i].accrualStartDate;
			}

                	discount *= interestRateDiscountFactorCpu(y, yearFractionCpu(npvDate, couponDate, y.dayCounter));                             
         	} 
		else  
		{
                	discount *= interestRateDiscountFactorCpu(y, yearFractionCpu(lastDate, couponDate, y.dayCounter));
            	}

            	lastDate = couponDate;

            	npv += amount * discount;
        }

        return npv;
}


dataType fOpCpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs)
{
	intRateStruct yield;
	
	yield.rate = y;
	yield.comp = f.comp;
	yield.freq = f.freq;
	yield.dayCounter = f.dayCounter;

	dataType NPV = cashFlowsNpvYieldCpu(cashFlows,
                        yield,
                        false,
                        f.settlementDate,
                        f.npvDate, numLegs);
             
	return (f.npv - NPV);
}



dataType fDerivativeCpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs)
{
	intRateStruct yield;
	yield.rate = y;
	yield.dayCounter = f.dayCounter;
	yield.comp = f.comp;
	yield.freq = f.freq;

    	return modifiedDurationCpu(cashFlows, yield,
                                        f.includeSettlementDateFlows,
                                        f.settlementDate, f.npvDate, numLegs);
}


bool closeCpu(dataType x, dataType y)
{
	return closeCpu(x,y,42);
}


bool closeCpu(dataType x, dataType y, int n)
{
	dataType diff = fabs(x-y);
	dataType tolerance = n*QL_EPSILON_GPU;

	return diff <= tolerance*fabs(x) &&
               diff <= tolerance*fabs(y);
}


dataType enforceBoundsCpu(dataType x)
{
	/*if Cpu(lowerBoundEnforced_ && x < lowerBound_)
		return lowerBound_;
	if (upperBoundEnforced_ && x > upperBound_)
		return upperBound_;*/
	return x;
}


dataType solveImplCpu(solverStruct solver, irrFinderStruct f,
				dataType xAccuracy, cashFlowsStruct cashFlows, int numLegs)
{
	dataType froot, dfroot, dx, dxold;
	dataType xh, xl;

	// Orient the search so that f(xl) < 0
	if (solver.fxMin_ < 0.0) 
	{
		xl = solver.xMin_;
		xh = solver.xMax_;
	} 
	else 
	{
		xh = solver.xMin_;
		xl = solver.xMax_;
	}

	// the "stepsize before last"
	dxold = solver.xMax_ - solver.xMin_;
	// it was dxold=std::fabs(xMax_-xMin_); in Numerical Recipes
	// here (xMax_-xMin_ > 0) is verified in the constructor

	// and the last step
	dx = dxold;

	froot = fOpCpu(f, solver.root_, cashFlows, numLegs);
	dfroot = fDerivativeCpu(f, solver.root_, cashFlows, numLegs);

	++solver.evaluationNumber_;

	while (solver.evaluationNumber_<=solver.maxEvaluations_) 
	{
		// Bisect if (out of range || not decreasing fast enough)
		if ((((solver.root_-xh)*dfroot-froot)*
			((solver.root_-xl)*dfroot-froot) > 0.0)
			|| (fabs(2.0*froot) > fabs(dxold*dfroot))) 
		{
			dxold = dx;
			dx = (xh-xl)/2.0;
			solver.root_=xl+dx;
		} 
		else 
		{
			dxold = dx;
			dx = froot/dfroot;
			solver.root_ -= dx;
		}
	
		// Convergence criterion
		if (fabs(dx) < xAccuracy)
			return solver.root_;
		froot = fOpCpu(f, solver.root_, cashFlows, numLegs);
		dfroot = fDerivativeCpu(f, solver.root_, cashFlows, numLegs);
		++solver.evaluationNumber_;
		if (froot < 0.0)
			xl=solver.root_;
		else
			xh=solver.root_;
	}

	return solver.root_;
}





dataType modifiedDurationCpu(cashFlowsStruct cashFlows,
								intRateStruct y,
								bool includeSettlementDateFlows,
								repoDateStruct settlementDate,
								repoDateStruct npvDate,
								int numLegs)
{
	dataType P = 0.0;
	dataType dPdy = 0.0;
	dataType r = y.rate;
	dataType N = y.freq;
	int dc = y.dayCounter;

	for (int i=0; i<numLegs; ++i) 
	{
		if (!cashFlowHasOccurredCpu(cashFlows.legs[i].paymentDate, settlementDate)) 
		{
			dataType t = yearFractionCpu(npvDate,
						     cashFlows.legs[i].paymentDate, dc);
			dataType c = fixedRateCouponAmountCpu(cashFlows, i);  
			dataType B = interestRateDiscountFactorCpu(y, t); 

			P += c * B;
			switch (y.comp) 
			{
				case SIMPLE_INTEREST:
					dPdy -= c * B*B * t;
					break;
				case COMPOUNDED_INTEREST:
					dPdy -= c * t * B/(1+r/N);
					break;
				case CONTINUOUS_INTEREST:
					dPdy -= c * B * t;
					break;
				case SIMPLE_THEN_COMPOUNDED_INTEREST:
					if (t<=1.0/N)
						dPdy -= c * B*B * t;
					else
						dPdy -= c * t * B/(1+r/N);
					break;
			}
		}
	}

	if (P == 0.0) // no cashflows
	{
		return 0.0;
	}
	return (-1*dPdy)/P; // reverse derivative sign
}

