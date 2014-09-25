//bondsKernelsGpu.cu
//Scott Grauer-Gray
//Bonds kernels to run on the GPU w/ CUDA

#include "bondsKernelsGpu.cuh"


__device__ int monthLengthKernelGpu(int month, bool leapYear) 
{
	int MonthLength[12];
	MonthLength[0]=31;
	MonthLength[1]=28;
	MonthLength[2]=31;
	MonthLength[3]=30;
	MonthLength[4]=31;
	MonthLength[5]=30;
	MonthLength[6]=31;
	MonthLength[7]=31;
	MonthLength[8]=30;
	MonthLength[9]=31;
	MonthLength[10]=30;
	MonthLength[11]=31;

	int MonthLeapLength[12];
	MonthLeapLength[0]=31;
	MonthLeapLength[1]=29;
	MonthLeapLength[2]=31;
	MonthLeapLength[3]=30;
	MonthLeapLength[4]=31;
	MonthLeapLength[5]=30;
	MonthLeapLength[6]=31;
	MonthLeapLength[7]=31;
	MonthLeapLength[8]=30;
	MonthLeapLength[9]=31;
	MonthLeapLength[10]=30;
	MonthLeapLength[11]=31;

	return (leapYear? MonthLeapLength[month-1] : MonthLength[month-1]);
}


__device__ int monthOffsetKernelGpu(int m, bool leapYear) 
{
	int MonthOffset[13];
	MonthOffset[0]=0;
	MonthOffset[1]=31;
	MonthOffset[2]=59;
	MonthOffset[3]=90;
	MonthOffset[4]=120;
	MonthOffset[5]=151;
	MonthOffset[6]=181;
	MonthOffset[7]=212;
	MonthOffset[8]=243;
	MonthOffset[9]=273;
	MonthOffset[10]=304;
	MonthOffset[11]=334;
	MonthOffset[12]=365;

	int MonthLeapOffset[13];
	MonthLeapOffset[0]=0;
	MonthLeapOffset[1]=31;
	MonthLeapOffset[2]=60;
	MonthLeapOffset[3]=91;
	MonthLeapOffset[4]=121;
	MonthLeapOffset[5]=152;
	MonthLeapOffset[6]=182;
	MonthLeapOffset[7]=213;
	MonthLeapOffset[8]=244;
	MonthLeapOffset[9]=274;
	MonthLeapOffset[10]=305;
	MonthLeapOffset[11]=335;
	MonthLeapOffset[12]=366;

	return (leapYear? MonthLeapOffset[m-1] : MonthOffset[m-1]);
}


__device__ int yearOffsetKernelGpu(int y)
{

	int YearOffset[121];
	YearOffset[0] = 0;;
	YearOffset[1] = 366;;
	YearOffset[2] = 731;
	YearOffset[3] = 1096;
	YearOffset[4] = 1461;
	YearOffset[5] = 1827;
	YearOffset[6] = 2192;
	YearOffset[7] = 2557;
	YearOffset[8] = 2922;
	YearOffset[9] = 3288;
	YearOffset[10] = 3653;
	YearOffset[11] = 4018;
	YearOffset[12] = 4383;
	YearOffset[13] = 4749;
	YearOffset[14] = 5114;
	YearOffset[15] = 5479;
	YearOffset[16] = 5844;
	YearOffset[17] = 6210;
	YearOffset[18] = 6575;
	YearOffset[19] = 6940;
	YearOffset[20] = 7305;
	YearOffset[21] = 7671;


	YearOffset[22] = 8036;
	YearOffset[23] = 8401;
	YearOffset[24] = 8766;
	YearOffset[25] = 9132;
	YearOffset[26] = 9497;
	YearOffset[27] = 9862;
	YearOffset[28] = 10227;
	YearOffset[29] = 10593;
	YearOffset[30] = 10958;
	YearOffset[31] = 11323;
	YearOffset[32] = 11688;
	YearOffset[33] = 12054;

	YearOffset[34] = 12419;
	YearOffset[35] = 12784;
	YearOffset[36] = 13149;
	YearOffset[37] = 13515;
	YearOffset[38] = 13880;
	YearOffset[39] = 14245;
	YearOffset[40] = 14610;
	YearOffset[41] = 14976;
	YearOffset[42] = 15341;
	YearOffset[43] = 15706;
	YearOffset[44] = 16071;
	YearOffset[45] = 16437;

	YearOffset[46] = 16802;
	YearOffset[47] = 17167;
	YearOffset[48] = 17532;
	YearOffset[49] = 17898;
	YearOffset[50] = 18263;
	YearOffset[51] = 18628;
	YearOffset[52] = 18993;
	YearOffset[53] = 19359;
	YearOffset[54] = 19724;
	YearOffset[55] = 20089;
	YearOffset[56] = 20454;
	YearOffset[57] = 20820;
	   
	YearOffset[58] = 21185;
	YearOffset[59] = 21550;
	YearOffset[60] = 21915;
	YearOffset[61] = 22281;
	YearOffset[62] = 22646;
	YearOffset[63] = 23011;
	YearOffset[64] = 23376;
	YearOffset[65] = 23742;
	YearOffset[66] = 24107;
	YearOffset[67] = 24472;
	YearOffset[68] = 24837;
	YearOffset[69] = 25203;

	YearOffset[70] = 25568;
	YearOffset[71] = 25933;
	YearOffset[72] = 26298;
	YearOffset[73] = 26664;
	YearOffset[74] = 27029;
	YearOffset[75] = 27394;
	YearOffset[76] = 27759;
	YearOffset[77] = 28125;
	YearOffset[78] = 28490;
	YearOffset[79] = 28855;
	YearOffset[80] = 29220;
	YearOffset[81] = 29586;
	   
	YearOffset[82] = 29951;
	YearOffset[83] = 30316;
	YearOffset[84] = 30681;
	YearOffset[85] = 31047;
	YearOffset[86] = 31412;
	YearOffset[87] = 31777;
	YearOffset[88] = 32142;
	YearOffset[89] = 32508;
	YearOffset[90] = 32873;
	YearOffset[91] = 33238;
	YearOffset[92] = 33603;
	YearOffset[93] = 33969;

	YearOffset[94] = 34334;
	YearOffset[95] = 34699;
	YearOffset[96] = 35064;
	YearOffset[97] = 35430;
	YearOffset[98] = 35795;
	YearOffset[99] = 36160;
	YearOffset[100] = 36525;
	YearOffset[101] = 36891;
	YearOffset[102] = 37256;
	YearOffset[103] = 37621;
	YearOffset[104] = 37986;
	YearOffset[105] = 38352;

	YearOffset[106] = 38717;
	YearOffset[107] = 39082;
	YearOffset[108] = 39447;
	YearOffset[109] = 39813;
	YearOffset[110] = 40178;
	YearOffset[111] = 40543;
	YearOffset[112] = 40908;
	YearOffset[113] = 41274;
	YearOffset[114] = 41639;
	YearOffset[115] = 42004;
	YearOffset[116] = 42369;
	YearOffset[117] = 42735;
	YearOffset[118] = 43100;
	YearOffset[119] = 42735;
	YearOffset[120] = 43830;
  

        return YearOffset[y-1900];
}


__device__ bool isLeapKernelGpu(int y) 
{
        bool YearIsLeap[121];

	YearIsLeap[0] = 1;;
	YearIsLeap[1] = 0;;
	YearIsLeap[2] = 0;
	YearIsLeap[3] = 0;//1096;
	YearIsLeap[4] = 1;//1461;
	YearIsLeap[5] = 0;//1827;
	YearIsLeap[6] = 0;//2192;
	YearIsLeap[7] = 0;//2557;
	YearIsLeap[8] = 1;//2922;
	YearIsLeap[9] = 0;//3288;
	YearIsLeap[10] = 0;//3653;
	YearIsLeap[11] = 0;//4018;
	YearIsLeap[12] = 1;//4383;
	YearIsLeap[13] = 0;//4749;
	YearIsLeap[14] = 0;//5114;
	YearIsLeap[15] = 0;//5479;
	YearIsLeap[16] = 1;//5844;
	YearIsLeap[17] = 0;//6210;
	YearIsLeap[18] = 0;//6575;
	YearIsLeap[19] = 0;//6940;
	YearIsLeap[20] = 1;//7305;
	YearIsLeap[21] = 0;//7671;


	YearIsLeap[22] = 0;//8036;
	YearIsLeap[23] = 0;//8401;
	YearIsLeap[24] = 1;//8766;
	YearIsLeap[25] = 0;//9132;
	YearIsLeap[26] = 0;//9497;
	YearIsLeap[27] = 0;//9862;
	YearIsLeap[28] = 1;//10227;
	YearIsLeap[29] = 0;//10593;
	YearIsLeap[30] = 0;//10958;
	YearIsLeap[31] = 0;//11323;
	YearIsLeap[32] = 1;//11688;
	YearIsLeap[33] = 0;//12054;

	YearIsLeap[34] = 0;//12419;
	YearIsLeap[35] = 0;//12784;
	YearIsLeap[36] = 1;//13149;
	YearIsLeap[37] = 0;//13515;
	YearIsLeap[38] = 0;//13880;
	YearIsLeap[39] = 0;//14245;
	YearIsLeap[40] = 1;//14610;
	YearIsLeap[41] = 0;//14976;
	YearIsLeap[42] = 0;//15341;
	YearIsLeap[43] = 0;//15706;
	YearIsLeap[44] = 1;//16071;
	YearIsLeap[45] = 0;//16437;

	YearIsLeap[46] = 0;//16802;
	YearIsLeap[47] = 0;//17167;
	YearIsLeap[48] = 1;//17532;
	YearIsLeap[49] = 0;//17898;
	YearIsLeap[50] = 0;//18263;
	YearIsLeap[51] = 0;//18628;
	YearIsLeap[52] = 1;//18993;
	YearIsLeap[53] = 0;//19359;
	YearIsLeap[54] = 0;//19724;
	YearIsLeap[55] = 0;//20089;
	YearIsLeap[56] = 1;//20454;
	YearIsLeap[57] = 0;//20820;
	   
	YearIsLeap[58] = 0;//21185;
	YearIsLeap[59] = 0;//21550;
	YearIsLeap[60] = 1;//21915;
	YearIsLeap[61] = 0;//22281;
	YearIsLeap[62] = 0;//22646;
	YearIsLeap[63] = 0;//23011;
	YearIsLeap[64] = 1;//23376;
	YearIsLeap[65] = 0;//23742;
	YearIsLeap[66] = 0;//24107;
	YearIsLeap[67] = 0;//24472;
	YearIsLeap[68] = 1;//24837;
	YearIsLeap[69] = 0;//25203;

	YearIsLeap[70] = 0;//25568;
	YearIsLeap[71] = 0;//25933;
	YearIsLeap[72] = 1;//26298;
	YearIsLeap[73] = 0;//26664;
	YearIsLeap[74] = 0;//27029;
	YearIsLeap[75] = 0;//27394;
	YearIsLeap[76] = 1;//27759;
	YearIsLeap[77] = 0;//28125;
	YearIsLeap[78] = 0;//28490;
	YearIsLeap[79] = 0;//28855;
	YearIsLeap[80] = 1;//29220;
	YearIsLeap[81] = 0;//29586;
	   
	YearIsLeap[82] = 0;//29951;
	YearIsLeap[83] = 0;//30316;
	YearIsLeap[84] = 1;//30681;
	YearIsLeap[85] = 0;//31047;
	YearIsLeap[86] = 0;//31412;
	YearIsLeap[87] = 0;//31777;
	YearIsLeap[88] = 1;//32142;
	YearIsLeap[89] = 0;//32508;
	YearIsLeap[90] = 0;//32873;
	YearIsLeap[91] = 0;//33238;
	YearIsLeap[92] = 1;//33603;
	YearIsLeap[93] = 0;//33969;

	YearIsLeap[94] = 0;//34334;
	YearIsLeap[95] = 0;//34699;
	YearIsLeap[96] = 1;//35064;
	YearIsLeap[97] = 0;//35430;
	YearIsLeap[98] = 0;//35795;
	YearIsLeap[99] = 0;//36160;
	YearIsLeap[100] =1;// 36525;
	YearIsLeap[101] =0;// 36891;
	YearIsLeap[102] =0;// 37256;
	YearIsLeap[103] =0;// 37621;
	YearIsLeap[104] =1;// 37986;
	YearIsLeap[105] =0;// 38352;

	YearIsLeap[106] = 0;//38717;
	YearIsLeap[107] = 0;//39082;
	YearIsLeap[108] = 1;//39447;
	YearIsLeap[109] = 0;//39813;
	YearIsLeap[110] = 0;//40178;
	YearIsLeap[111] = 0;//40543;
	YearIsLeap[112] = 1;//40908;
	YearIsLeap[113] = 0;//41274;
	YearIsLeap[114] = 0;//41639;
	YearIsLeap[115] = 0;//42004;
	YearIsLeap[116] = 1;//42369;
	YearIsLeap[117] = 0;//42735;
	YearIsLeap[118] = 0;//43100;
	YearIsLeap[119] = 0;//42735;
	YearIsLeap[120] = 1;//43830;


	return YearIsLeap[y-1900];
}


__device__ bondsDateStruct intializeDateKernelGpu(int d, int m, int y) 
{
	bondsDateStruct currDate;

	currDate.day = d;
	currDate.month = m;
	currDate.year = y;

	bool leap = isLeapKernelGpu(y);
	int offset = monthOffsetKernelGpu(m,leap);

	currDate.dateSerialNum = d + offset + yearOffsetKernelGpu(y);

	return currDate;
}


__device__ dataType yearFractionGpu(bondsDateStruct d1,
					bondsDateStruct d2, int dayCounter)
{
	return dayCountGpu(d1, d2, dayCounter) / 360.0; 
}


__device__ int dayCountGpu(bondsDateStruct d1, bondsDateStruct d2, int dayCounter) 
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


__device__ dataType couponNotionalGpu()
{
	return 100.0;
}

__device__ dataType bondNotionalGpu()
{
	return 100.0;
}


__device__ dataType fixedRateCouponNominalGpu()
{
	return 100.0;
}

__device__ bool eventHasOccurredGpu(bondsDateStruct currDate, bondsDateStruct eventDate)
{
         return eventDate.dateSerialNum > currDate.dateSerialNum;
}


__device__ bool cashFlowHasOccurredGpu(bondsDateStruct refDate, bondsDateStruct eventDate)
{
        return eventHasOccurredGpu(refDate, eventDate);
}



__device__ bondsDateStruct advanceDateGpu(bondsDateStruct date, int numMonthsAdvance) 
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

	int length = monthLengthKernelGpu(m, isLeapKernelGpu(y));
	if (d > length)
		d = length;

	bondsDateStruct newDate = intializeDateKernelGpu(d, m, y);

	return newDate;
}

__device__ int getNumCashFlowsGpu(inArgsStruct inArgs, int bondNum)
{
	int numCashFlows = 0;
	
	//bondsDateStruct endDate = inArgs.bond[bondNum].maturityDate;
	bondsDateStruct currCashflowDate = inArgs.bond[bondNum].maturityDate;

	while (currCashflowDate.dateSerialNum > inArgs.bond[bondNum].startDate.dateSerialNum)
	{
		numCashFlows++;
		currCashflowDate = advanceDateGpu(currCashflowDate, -6); 
	}

	return numCashFlows+1;
}





__device__ dataType getDirtyPriceGpu(inArgsStruct inArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalGpu();
	return discountingBondEngineCalculateSettlementValueGpu(inArgs, bondNum, cashFlows, numLegs) * 100.0 / currentNotional;
}


__device__ dataType getAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs)
{
	return bondAccruedAmountGpu(inArgs, date, bondNum, cashFlows, numLegs);
}


__device__ dataType discountingBondEngineCalculateSettlementValueGpu(inArgsStruct inArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs)  
{

	bondsDateStruct currDate = inArgs.currDate[bondNum];
		
	if (currDate.dateSerialNum < inArgs.bond[bondNum].startDate.dateSerialNum)
	{
		currDate = inArgs.bond[bondNum].startDate;
	}



	return cashFlowsNpvGpu(cashFlows,
                               inArgs.discountCurve[bondNum],
                               false,
                               currDate,
                               currDate, 
			       numLegs);
}


__device__ dataType bondAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalGpu();
	if (currentNotional == 0.0)
		return 0.0;
		
	return bondFunctionsAccruedAmountGpu(inArgs, date, bondNum, cashFlows, numLegs);
}


__device__ dataType bondFunctionsAccruedAmountGpu(inArgsStruct inArgs, bondsDateStruct date, int bondNum, cashFlowsStruct cashFlows, int numLegs) 
{
        return cashFlowsAccruedAmountGpu(cashFlows,
                                        false, date, numLegs, inArgs, bondNum) *
            100.0 / bondNotionalGpu();
}







__device__ dataType cashFlowsAccruedAmountGpu(cashFlowsStruct cashFlows,
                                  bool includecurrDateFlows,
                                  bondsDateStruct currDate,
				  int numLegs, inArgsStruct inArgs, int bondNum) 
{
	int legComputeNum = cashFlowsNextCashFlowNumGpu(cashFlows,
                            currDate, numLegs); 

        dataType result = 0.0;

	int i;

        for (i = legComputeNum; i < (numLegs); ++i)
	{
                result += fixedRateCouponAccruedAmountGpu(cashFlows, i, currDate, inArgs, bondNum);
        }

        return result;
}


__device__ dataType fixedRateCouponAccruedAmountGpu(cashFlowsStruct cashFlows, int numLeg, bondsDateStruct d, inArgsStruct inArgs, int bondNum) 
{
	if (d.dateSerialNum <= cashFlows.legs[numLeg].accrualStartDate.dateSerialNum || d.dateSerialNum > inArgs.maturityDate[bondNum].dateSerialNum) 
	{
		return 0.0;
	}
	else
	{
		bondsDateStruct endDate = cashFlows.legs[numLeg].accrualEndDate;
		if (d.dateSerialNum < cashFlows.legs[numLeg].accrualEndDate.dateSerialNum)
		{
			endDate = d;
		}

		return fixedRateCouponNominalGpu()*(interestRateCompoundFactorGpu(cashFlows.intRate, cashFlows.legs[numLeg].accrualStartDate, endDate, cashFlows.dayCounter) - 1.0);
	}
}

	



__device__ dataType cashFlowsNpvGpu(cashFlowsStruct cashFlows,
                        bondsYieldTermStruct discountCurve,
                        bool includecurrDateFlows,
                        bondsDateStruct currDate,
                        bondsDateStruct npvDate,
						int numLegs) 
{
       npvDate = currDate;

        dataType totalNPV = 0.0;

	int i;

        for (i=0; i<numLegs; ++i) {

	

            if (!(cashFlowHasOccurredGpu(cashFlows.legs[i].paymentDate, currDate)))
                totalNPV += fixedRateCouponAmountGpu(cashFlows, i) *
                            bondsYieldTermStructureDiscountGpu(discountCurve, cashFlows.legs[i].paymentDate);
        }

	return totalNPV/bondsYieldTermStructureDiscountGpu(discountCurve, npvDate);
}




__device__ dataType bondsYieldTermStructureDiscountGpu(bondsYieldTermStruct ytStruct, bondsDateStruct t)
{
	ytStruct.intRate.rate = ytStruct.forward;
	ytStruct.intRate.freq = ytStruct.frequency;
	ytStruct.intRate.comp = ytStruct.compounding;
	return flatForwardDiscountImplGpu(ytStruct.intRate, yearFractionGpu(ytStruct.refDate, t, ytStruct.dayCounter));
}


__device__ dataType flatForwardDiscountImplGpu(intRateStruct intRate, dataType t) 
{
	return interestRateDiscountFactorGpu(intRate, t);
}





__device__ dataType interestRateDiscountFactorGpu(intRateStruct intRate, dataType t) 
{
	return 1.0/interestRateCompoundFactorGpuTwoArgs(intRate, t);
}


__device__ dataType interestRateCompoundFactorGpuTwoArgs(intRateStruct intRate, dataType t) 
{
	//switch (intRate.comp) 
	{
          if (intRate.comp == SIMPLE_INTEREST)
            return 1.0 + intRate.rate*t;
          else if (intRate.comp == COMPOUNDED_INTEREST)
            return pow(1.0f+intRate.rate/intRate.freq, intRate.freq*t);
          else if (intRate.comp == CONTINUOUS_INTEREST)
            return exp(intRate.rate*t);
	}

	return 0.0f;
}


__device__ dataType fixedRateCouponAmountGpu(cashFlowsStruct cashFlows, int numLeg) 
{
	if (cashFlows.legs[numLeg].amount == COMPUTE_AMOUNT)
	{
		return fixedRateCouponNominalGpu()*(interestRateCompoundFactorGpu(cashFlows.intRate, cashFlows.legs[numLeg].accrualStartDate,
                                              cashFlows.legs[numLeg].accrualEndDate, cashFlows.dayCounter) - 1.0);
	}
	else
	{
		return cashFlows.legs[numLeg].amount;
	}
}

__device__ dataType interestRateCompoundFactorGpu(intRateStruct intRate, bondsDateStruct d1,
				                           bondsDateStruct d2, int dayCounter)
{
	dataType t = yearFractionGpu(d1, d2, dayCounter);
	return interestRateCompoundFactorGpuTwoArgs(intRate, t);
}



__device__ dataType interestRateImpliedRateGpu(dataType compound,                                        
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


__device__ dataType getMarketRepoRateGpu(bondsDateStruct d,
                                   int comp,
                                   dataType freq,
								   bondsDateStruct referenceDate,
								   inArgsStruct inArgs, int bondNum)
{
	dataType compound = 1.0/bondsYieldTermStructureDiscountGpu(inArgs.repoCurve[bondNum], d);
    	return interestRateImpliedRateGpu(compound,
                                   comp, freq,
                                   yearFractionGpu(referenceDate, d, inArgs.repoCurve[bondNum].dayCounter));
}


__device__ couponStruct cashFlowsNextCashFlowGpu(cashFlowsStruct cashFlows,
                            bondsDateStruct currDate,
							int numLegs) 
{
	int i;
        for (i = 0; i < numLegs; ++i) 
	{


            	if ( ! (cashFlowHasOccurredGpu(cashFlows.legs[i].paymentDate, currDate) ))
                	return cashFlows.legs[i];
        }
        return cashFlows.legs[numLegs-1];
}


__device__ int cashFlowsNextCashFlowNumGpu(cashFlowsStruct cashFlows,
                            bondsDateStruct currDate,
							int numLegs) 
{
	int i;
	
        for (i = 0; i < numLegs; ++i) 
	{

        	if ( ! (cashFlowHasOccurredGpu(cashFlows.legs[i].paymentDate, currDate) ))
                	return i;
        }
        
	return (numLegs-1);
}


__device__ dataType getBondYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     bondsDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
		     inArgsStruct currInArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs)
{
	dataType currentNotional = bondNotionalGpu();

	if (currentNotional == 0.0)
		return 0.0;

	if (currInArgs.bond[bondNum].startDate.dateSerialNum > settlement.dateSerialNum)
	{
		settlement = currInArgs.bond[bondNum].startDate;
	}

	return getBondFunctionsYieldGpu(cleanPrice, dc, comp, freq,
								settlement, accuracy, maxEvaluations,
								currInArgs, bondNum, cashFlows, numLegs);
}


__device__ dataType getBondFunctionsYieldGpu(dataType cleanPrice,
                     int dc,
                     int comp,
                     dataType freq,
                     bondsDateStruct settlement,
                     dataType accuracy,
                     int maxEvaluations,
					 inArgsStruct currInArgs, int bondNum, cashFlowsStruct cashFlows, int numLegs)
{
        dataType dirtyPrice = cleanPrice + bondFunctionsAccruedAmountGpu(currInArgs, settlement, bondNum, cashFlows, numLegs); 
        dirtyPrice /= 100.0 / bondNotionalGpu();

        return getCashFlowsYieldGpu(cashFlows, dirtyPrice,
                                dc, comp, freq,
                                false, settlement, settlement, numLegs,
                                accuracy, maxEvaluations, 0.05f);
}


__device__ dataType getCashFlowsYieldGpu(cashFlowsStruct leg,
                          dataType npv,
                          int dayCounter,
                          int compounding,
                          dataType frequency,
                          bool includecurrDateFlows,
                          bondsDateStruct currDate,
                          bondsDateStruct npvDate,
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
	objFunction.includecurrDateFlows = includecurrDateFlows;
	objFunction.currDate = currDate;
	objFunction.npvDate = npvDate;

	return solverSolveGpu(solver, objFunction, accuracy, guess, guess/10.0, leg, numLegs);
}


__device__ dataType solverSolveGpu(solverStruct solver,
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
	solver.fxMax_ = fOpGpu(f, solver.root_, cashFlows, numLegs);

	// monotonically crescent bias, as in optionValue(volatility)
	if (closeGpu(solver.fxMax_,0.0))
	{
		return solver.root_;
	}
	else if (closeGpu(solver.fxMax_, 0.0)) 
	{
		solver.xMin_ = /*enforceBounds*/(solver.root_ - step);
		solver.fxMin_ = fOpGpu(f, solver.xMin_, cashFlows, numLegs);
		solver.xMax_ = solver.root_;
	} 
	else 
	{
		solver.xMin_ = solver.root_;
		solver.fxMin_ = solver.fxMax_;
		solver.xMax_ = /*enforceBounds*/(solver.root_+step);
		solver.fxMax_ = fOpGpu(f, solver.xMax_, cashFlows, numLegs);
	}

	solver.evaluationNumber_ = 2;
	while (solver.evaluationNumber_ <= solver.maxEvaluations_) 
	{
		if (solver.fxMin_*solver.fxMax_ <= 0.0) 
		{
			if (closeGpu(solver.fxMin_, 0.0))
				return solver.xMin_;
			if (closeGpu(solver.fxMax_, 0.0))
				return solver.xMax_;
			solver.root_ = (solver.xMax_+solver.xMin_)/2.0;
			return solveImplGpu(solver, f, accuracy, cashFlows, numLegs);
		}
		if (fabs(solver.fxMin_) < fabs(solver.fxMax_)) 
		{
			solver.xMin_ = /*enforceBounds*/(solver.xMin_+growthFactor*(solver.xMin_ - solver.xMax_));
			solver.fxMin_= fOpGpu(f, solver.xMin_, cashFlows, numLegs);
		} 
		else if (fabs(solver.fxMin_) > fabs(solver.fxMax_)) 
		{
			solver.xMax_ = /*enforceBounds*/(solver.xMax_+growthFactor*(solver.xMax_ - solver.xMin_));
			solver.fxMax_= fOpGpu(f, solver.xMax_, cashFlows, numLegs);
		} 
		else if (flipflop == -1) 
		{
			solver.xMin_ = /*enforceBounds*/(solver.xMin_+growthFactor*(solver.xMin_ - solver.xMax_));
			solver.fxMin_= fOpGpu(f, solver.xMin_, cashFlows, numLegs);
			solver.evaluationNumber_++;
			flipflop = 1;
		} 
		else if (flipflop == 1) 
		{
			solver.xMax_ = /*enforceBounds*/(solver.xMax_+growthFactor*(solver.xMax_ - solver.xMin_));
			solver.fxMax_= fOpGpu(f, solver.xMax_, cashFlows, numLegs);
			flipflop = -1;
		}
		solver.evaluationNumber_++;
	}

	return 0.0f;
}


__device__ dataType cashFlowsNpvYieldGpu(cashFlowsStruct cashFlows,
                        intRateStruct y,
                        bool includecurrDateFlows,
                        bondsDateStruct currDate,
                        bondsDateStruct npvDate,
						int numLegs) 
{
        dataType npv = 0.0;
        dataType discount = 1.0;
        bondsDateStruct lastDate;
	bool first = true;

	int i;
        for (i=0; i<numLegs; ++i) 
	{
		

            if (cashFlowHasOccurredGpu(cashFlows.legs[i].paymentDate, currDate))
                continue;

            bondsDateStruct couponDate = cashFlows.legs[i].paymentDate;
            dataType amount = fixedRateCouponAmountGpu(cashFlows, i);
            if (first) 
			{
				first = false;
		if (i > 0) {
                    lastDate = advanceDateGpu(cashFlows.legs[i].paymentDate, -1*6); 
                } else {
                        lastDate = cashFlows.legs[i].accrualStartDate;
		}
                discount *= interestRateDiscountFactorGpu(y, yearFractionGpu(npvDate, couponDate, y.dayCounter));
            } 
			else  
			{
                discount *= interestRateDiscountFactorGpu(y, yearFractionGpu(lastDate, couponDate, y.dayCounter));
            }

            lastDate = couponDate;

            npv += amount * discount;
        }

        return npv;
}

__device__ dataType fOpGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs)
{
	intRateStruct yield;
	
	yield.rate = y;
	yield.comp = f.comp;
	yield.freq = f.freq;
	yield.dayCounter = f.dayCounter;

	dataType NPV = cashFlowsNpvYieldGpu(cashFlows,
                        yield,
                        false,
                        f.currDate,
                        f.npvDate, numLegs);

                
	return (f.npv - NPV);
}



__device__ dataType fDerivativeGpu(irrFinderStruct f, dataType y, cashFlowsStruct cashFlows, int numLegs)
{
	intRateStruct yield;
	yield.rate = y;
	yield.dayCounter = f.dayCounter;
	yield.comp = f.comp;
	yield.freq = f.freq;


    	return modifiedDurationGpu(cashFlows, yield,
                                        f.includecurrDateFlows,
                                        f.currDate, f.npvDate, numLegs);
}


__device__ bool closeGpu(dataType x, dataType y)
{
	return closeGpuThreeArgs(x,y,42);
}


__device__ bool closeGpuThreeArgs(dataType x, dataType y, int n)
{
	dataType diff = fabs(x-y);
	dataType tolerance = n*QL_EPSILON_GPU;

	return diff <= tolerance*fabs(x) &&
               diff <= tolerance*fabs(y);
}


__device__ dataType enforceBoundsGpu(dataType x)
{
	/*if Gpu(lowerBoundEnforced_ && x < lowerBound_)
		return lowerBound_;
	if (upperBoundEnforced_ && x > upperBound_)
		return upperBound_;*/
	return x;
}


__device__ dataType solveImplGpu(solverStruct solver, irrFinderStruct f,
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

	froot = fOpGpu(f, solver.root_, cashFlows, numLegs);
	dfroot = fDerivativeGpu(f, solver.root_, cashFlows, numLegs);

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
		froot = fOpGpu(f, solver.root_, cashFlows, numLegs);
		dfroot = fDerivativeGpu(f, solver.root_, cashFlows, numLegs);
		++solver.evaluationNumber_;
		if (froot < 0.0)
			xl=solver.root_;
		else
			xh=solver.root_;
	}

	return solver.root_;
}




__device__ dataType modifiedDurationGpu(cashFlowsStruct cashFlows,
								intRateStruct y,
								bool includecurrDateFlows,
								bondsDateStruct currDate,
								bondsDateStruct npvDate,
								int numLegs)
{
	dataType P = 0.0;
	dataType dPdy = 0.0;
	dataType r = y.rate;
	dataType N = y.freq;
	int dc = y.dayCounter;

	int i;
	for (i=0; i<numLegs; ++i) 
	{
		

		if (!cashFlowHasOccurredGpu(cashFlows.legs[i].paymentDate, currDate)) 
		{
			dataType t = yearFractionGpu(npvDate,
									cashFlows.legs[i].paymentDate, dc);
			dataType c = fixedRateCouponAmountGpu(cashFlows, i);  
			dataType B = interestRateDiscountFactorGpu(y, t); 

			P += c * B;
			//switch (y.comp) 
			{
				if (y.comp == SIMPLE_INTEREST)
					dPdy -= c * B*B * t;
				if (y.comp == COMPOUNDED_INTEREST)
					dPdy -= c * t * B/(1+r/N);
				if (y.comp == CONTINUOUS_INTEREST)
					dPdy -= c * B * t;
				if (y.comp == SIMPLE_THEN_COMPOUNDED_INTEREST)
				{
					if (t<=1.0/N)
						dPdy -= c * B*B * t;
					else
						dPdy -= c * t * B/(1+r/N);
				}
			}
		}
	}

	if (P == 0.0) // no cashflows
	{
		return 0.0;
	}
	return (-1*dPdy)/P; // reverse derivative sign
}


__global__ void getBondsResultsGpu(inArgsStruct inArgs,
			resultsStruct results,
			int n)
{
	int bondNum = blockIdx.x*blockDim.x + threadIdx.x;
	if (bondNum < n)
	{
		int numLegs;

		int numCashFlows = 0;
	
		//bondsDateStruct endDate = inArgs.bond[bondNum].maturityDate;
		bondsDateStruct currCashflowDate = inArgs.bond[bondNum].maturityDate;

		while (currCashflowDate.dateSerialNum > inArgs.bond[bondNum].startDate.dateSerialNum)
		{
			numCashFlows++;
			currCashflowDate = advanceDateGpu(currCashflowDate, -6); 
		}

		numLegs = numCashFlows+1;

 


     		cashFlowsStruct cashFlows; 
		couponStruct cashLegs[9];
		cashFlows.legs = cashLegs;

		cashFlows.intRate.dayCounter = USE_EXACT_DAY;
		cashFlows.intRate.rate  = inArgs.bond[bondNum].rate;
		cashFlows.intRate.freq  = ANNUAL_FREQ;
		cashFlows.intRate.comp  = SIMPLE_INTEREST;
		cashFlows.dayCounter  = USE_EXACT_DAY;
		cashFlows.nominal  = 100.0;

		//bondsDateStruct currPaymentDate;
		bondsDateStruct currStartDate = advanceDateGpu(inArgs.bond[bondNum].maturityDate, (numLegs - 1)*-6);;
		bondsDateStruct currEndDate = advanceDateGpu(currStartDate, 6); 

		int cashFlowNum;
		for (cashFlowNum = 0; cashFlowNum < numLegs-1; cashFlowNum++)
		{
			cashFlows.legs[cashFlowNum].paymentDate = currEndDate;
			
			
			cashFlows.legs[cashFlowNum].accrualStartDate  = currStartDate;
			cashFlows.legs[cashFlowNum].accrualEndDate  = currEndDate;
			
			cashFlows.legs[cashFlowNum].amount = COMPUTE_AMOUNT;

			currStartDate = currEndDate;
			currEndDate = advanceDateGpu(currEndDate, 6); 
		}

		cashFlows.legs[numLegs-1].paymentDate  = inArgs.bond[bondNum].maturityDate;
		cashFlows.legs[numLegs-1].accrualStartDate = inArgs.currDate[bondNum];
		cashFlows.legs[numLegs-1].accrualEndDate  = inArgs.currDate[bondNum];
		cashFlows.legs[numLegs-1].amount = 100.0;

		results.bondForwardVal[bondNum] = getBondYieldGpu(inArgs.bondCleanPrice[bondNum],
                     USE_EXACT_DAY,
                     COMPOUNDED_INTEREST,
                     2.0,
                     inArgs.currDate[bondNum],
                     ACCURACY,
                     100,
		     inArgs, bondNum, cashFlows, numLegs);
		inArgs.discountCurve[bondNum].forward = results.bondForwardVal[bondNum];
		results.dirtyPrice[bondNum] = getDirtyPriceGpu(inArgs, bondNum, cashFlows, numLegs);
		results.accruedAmountCurrDate[bondNum] = getAccruedAmountGpu(inArgs, inArgs.currDate[bondNum], bondNum, cashFlows, numLegs);
		results.cleanPrice[bondNum] = results.dirtyPrice[bondNum] - results.accruedAmountCurrDate[bondNum];
	}
}

