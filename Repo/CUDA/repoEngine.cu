//repoEngine.cu
//Scott Grauer-Gray
//Engine for running the repo (repurchase agreement) on the GPU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <sys/time.h>

#include "repoStructs.cuh"
#include "repoKernels.cu"
#include "repoKernelsCpu.cu"

#include <time.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define NUM_REPOS_RUN 1000000


int monthLengthCpu(int month, bool leapYear) 
{
	int MonthLength[] = {
			31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
    
	int MonthLeapLength[] = {
			31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
    
	return (leapYear? MonthLeapLength[month-1] : MonthLength[month-1]);
}


int monthOffsetCpu(int m, bool leapYear) 
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


int yearOffsetCpu(int y)
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


bool isLeapCpu(int y) 
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


repoDateStruct intializeDateCpu(int d, int m, int y) 
{
	repoDateStruct currDate;

	currDate.day = d;
	currDate.month = m;
	currDate.year = y;

	bool leap = isLeapCpu(y);
	int offset = monthOffsetCpu(m,leap);

	currDate.dateSerialNum = d + offset + yearOffsetCpu(y);

	return currDate;
}



void runRepoEngine() 
{
	//can run multiple times with different number of bonds by uncommenting these lines
	//int nReposArray[] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000};

	//for (int numTime=0; numTime < 14; numTime++)
	{

		int numRepos = NUM_REPOS_RUN;//nReposArray[numTime];	
		printf("\nNUM_REPOS: %d\n\n", numRepos);

		

		inArgsStruct inArgsHost;
		
		inArgsHost.discountCurve = (repoYieldTermStruct*)malloc(numRepos*sizeof(repoYieldTermStruct));
		inArgsHost.repoCurve = (repoYieldTermStruct*)malloc(numRepos*sizeof(repoYieldTermStruct));
		inArgsHost.settlementDate = (repoDateStruct*)malloc(numRepos*sizeof(repoDateStruct));
		inArgsHost.deliveryDate = (repoDateStruct*)malloc(numRepos*sizeof(repoDateStruct));
		inArgsHost.maturityDate = (repoDateStruct*)malloc(numRepos*sizeof(repoDateStruct));
		inArgsHost.repoDeliveryDate = (repoDateStruct*)malloc(numRepos*sizeof(repoDateStruct));
		inArgsHost.bondCleanPrice = (dataType*)malloc(numRepos*sizeof(dataType));
		inArgsHost.bond = (bondStruct*)malloc(numRepos*sizeof(bondStruct));
		inArgsHost.dummyStrike = (dataType*)malloc(numRepos*sizeof(dataType));
		
		srand ( time(NULL) );
	
		
		for (int numRepo = 0; numRepo < numRepos; numRepo++)
		{
			repoDateStruct repoSettlementDate = intializeDateCpu(rand() % 28 + 1, 3 - (rand() % 3), 2000);
    			repoDateStruct repoDeliveryDate = intializeDateCpu(rand() % 28 + 1, 9 + (rand() % 3), 2000);
    			dataType repoRate = 0.05 + ((float)rand()/(float)RAND_MAX - 0.5)*0.1;
    
    			//int repoSettlementDays = 0;
    			int repoCompounding = SIMPLE_INTEREST;
    			dataType repoCompoundFreq = 1;
    	
    			repoDateStruct bondIssueDate = intializeDateCpu(rand()%28 + 1, rand()%12 + 1, 1999 - (rand()%2));
    			//repoDateStruct bondDatedDate = bondIssueDate;
    			repoDateStruct bondMaturityDate = intializeDateCpu(rand()%28 + 1, rand()%12 + 1, 2001);
    
    			bondStruct bond;
    			bond.startDate = bondIssueDate;
    			bond.maturityDate = bondMaturityDate;
    			bond.rate = 0.08 + ((float)rand() / (float)RAND_MAX - 0.5) * 0.1;


			//dataType bondCoupon = bond.rate;
			dataType bondCouponFrequency = 2;
			
			//int bondSettlementDays = 0;

			dataType bondCleanPrice = 89.97693786;
			//dataType bondRedemption = 100.0;
			//dataType faceAmount = 100.0;




			repoYieldTermStruct bondCurve;

			bondCurve.refDate = repoSettlementDate;
			bondCurve.calDate = repoSettlementDate;
			bondCurve.forward = -0.1f; // dummy rate
			bondCurve.compounding = COMPOUNDED_INTEREST;
			bondCurve.frequency = bondCouponFrequency;
			bondCurve.dayCounter = USE_EXACT_DAY;
						bondCurve.refDate = repoSettlementDate;
			bondCurve.calDate = repoSettlementDate;
			bondCurve.compounding = COMPOUNDED_INTEREST;
			bondCurve.frequency = bondCouponFrequency;

		
			dataType dummyStrike = 91.5745;

			repoYieldTermStruct repoCurve;
			repoCurve.refDate = repoSettlementDate;
			repoCurve.calDate = repoSettlementDate;
			repoCurve.forward = repoRate;
			repoCurve.compounding = repoCompounding;
			repoCurve.frequency = repoCompoundFreq;
			repoCurve.dayCounter = USE_SERIAL_NUMS;


			inArgsHost.discountCurve[numRepo] = bondCurve;
			inArgsHost.repoCurve[numRepo] = repoCurve;
			inArgsHost.settlementDate[numRepo] = repoSettlementDate;
			inArgsHost.deliveryDate[numRepo] = repoDeliveryDate;
			inArgsHost.maturityDate[numRepo] = bondMaturityDate;
			inArgsHost.repoDeliveryDate[numRepo] = repoDeliveryDate;
			inArgsHost.bondCleanPrice[numRepo] = bondCleanPrice;
			inArgsHost.bond[numRepo] = bond;
			inArgsHost.dummyStrike[numRepo] = dummyStrike;
		}

		printf("Inputs for repo with index %d\n", numRepos/2);
		printf("Repo Settlement Date: %d-%d-%d\n", inArgsHost.settlementDate[numRepos/2].month, inArgsHost.settlementDate[numRepos/2].day, inArgsHost.settlementDate[numRepos/2].year);
		printf("Repo Delivery Date: %d-%d-%d\n", inArgsHost.deliveryDate[numRepos/2].month, inArgsHost.deliveryDate[numRepos/2].day, inArgsHost.deliveryDate[numRepos/2].year);
		printf("Bond Issue Date: %d-%d-%d\n", inArgsHost.bond[numRepos/2].startDate.month, inArgsHost.bond[numRepos/2].startDate.day, inArgsHost.bond[numRepos/2].startDate.year);
		printf("Bond Maturity Date: %d-%d-%d\n", inArgsHost.maturityDate[numRepos/2].month, inArgsHost.maturityDate[numRepos/2].day, inArgsHost.maturityDate[numRepos/2].year);
		printf("Repo rate: %f\n", inArgsHost.repoCurve[numRepos/2].forward);
		printf("Bond rate: %f\n", inArgsHost.bond[numRepos/2].rate);
		printf("Bond clean price: %f\n\n", inArgsHost.bondCleanPrice[numRepos/2]);

		resultsStruct resultsHost;
		resultsStruct resultsFromGpu;

		resultsHost.dirtyPrice = (dataType*)malloc(numRepos*sizeof(dataType));
		resultsHost.accruedAmountSettlement = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.accruedAmountDeliveryDate = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.cleanPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.forwardSpotIncome = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.underlyingBondFwd = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.repoNpv = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.repoCleanForwardPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.repoDirtyForwardPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.repoImpliedYield = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.marketRepoRate = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsHost.bondForwardVal = (dataType*)malloc(numRepos*sizeof(dataType));;

		resultsFromGpu.dirtyPrice = (dataType*)malloc(numRepos*sizeof(dataType));
		resultsFromGpu.accruedAmountSettlement = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.accruedAmountDeliveryDate = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.cleanPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.forwardSpotIncome = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.underlyingBondFwd = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.repoNpv = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.repoCleanForwardPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.repoDirtyForwardPrice = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.repoImpliedYield = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.marketRepoRate = (dataType*)malloc(numRepos*sizeof(dataType));;
		resultsFromGpu.bondForwardVal = (dataType*)malloc(numRepos*sizeof(dataType));;

		inArgsStruct inArgsGpu;
		resultsStruct resultsGpu;

		cudaMalloc(&(resultsGpu.dirtyPrice), numRepos*sizeof(dataType));
		cudaMalloc(&(resultsGpu.accruedAmountSettlement), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.accruedAmountDeliveryDate), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.cleanPrice), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.forwardSpotIncome), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.underlyingBondFwd), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.repoNpv), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.repoCleanForwardPrice), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.repoDirtyForwardPrice), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.repoImpliedYield), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.marketRepoRate), numRepos*sizeof(dataType));;
		cudaMalloc(&(resultsGpu.bondForwardVal), numRepos*sizeof(dataType));;

		cudaMalloc(&(inArgsGpu.discountCurve), numRepos*sizeof(repoYieldTermStruct));;
		cudaMalloc(&(inArgsGpu.repoCurve), numRepos*sizeof(repoYieldTermStruct));;
		cudaMalloc(&(inArgsGpu.settlementDate), numRepos*sizeof(repoDateStruct));;
		cudaMalloc(&(inArgsGpu.deliveryDate), numRepos*sizeof(repoDateStruct));;
		cudaMalloc(&(inArgsGpu.maturityDate), numRepos*sizeof(repoDateStruct));;
		cudaMalloc(&(inArgsGpu.repoDeliveryDate), numRepos*sizeof(repoDateStruct));;
		cudaMalloc(&(inArgsGpu.bondCleanPrice), numRepos*sizeof(dataType));;
		cudaMalloc(&(inArgsGpu.bond), numRepos*sizeof(bondStruct));;
		cudaMalloc(&(inArgsGpu.dummyStrike), numRepos*sizeof(dataType));;


		dim3 blockDim(128, 1);
		dim3 gridDim((size_t)ceil((dataType)numRepos / (dataType)blockDim.x), 1);


		cudaMemcpy((inArgsGpu.discountCurve), inArgsHost.discountCurve, numRepos*sizeof(repoYieldTermStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.repoCurve), inArgsHost.repoCurve, numRepos*sizeof(repoYieldTermStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.settlementDate), inArgsHost.settlementDate, numRepos*sizeof(repoDateStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.deliveryDate), inArgsHost.deliveryDate, numRepos*sizeof(repoDateStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.maturityDate), inArgsHost.maturityDate, numRepos*sizeof(repoDateStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.repoDeliveryDate), inArgsHost.repoDeliveryDate, numRepos*sizeof(repoDateStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.bondCleanPrice), inArgsHost.bondCleanPrice, numRepos*sizeof(dataType), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.bond), inArgsHost.bond, numRepos*sizeof(bondStruct), cudaMemcpyHostToDevice);;
		cudaMemcpy((inArgsGpu.dummyStrike), inArgsHost.dummyStrike, numRepos*sizeof(dataType), cudaMemcpyHostToDevice);;
		
		long seconds, useconds;    
		float mtimeGpu;
		float mtimeCpu;

		struct timeval start;
		gettimeofday(&start, NULL);

	
    

		getRepoResultsGpu <<< gridDim, blockDim >>> (inArgsGpu, resultsGpu, numRepos);

		cudaThreadSynchronize();
		struct timeval end;
		gettimeofday(&end, NULL);
	
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		mtimeGpu = ((seconds) * 1000 + ((float)useconds)/1000.0) + 0.5f;

		printf("Run on GPU\n");
		printf("Processing time on GPU: %f (ms)  \n\n", mtimeGpu);

		//copy data back to CPU
		cudaMemcpy(resultsFromGpu.dirtyPrice, (resultsGpu.dirtyPrice), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);
		cudaMemcpy(resultsFromGpu.accruedAmountSettlement, (resultsGpu.accruedAmountSettlement), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.accruedAmountDeliveryDate, (resultsGpu.accruedAmountDeliveryDate), numRepos*sizeof(dataType),cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.cleanPrice, (resultsGpu.cleanPrice), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.forwardSpotIncome, (resultsGpu.forwardSpotIncome), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.underlyingBondFwd, (resultsGpu.underlyingBondFwd), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.repoNpv, (resultsGpu.repoNpv), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.repoCleanForwardPrice, (resultsGpu.repoCleanForwardPrice), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.repoDirtyForwardPrice, (resultsGpu.repoDirtyForwardPrice), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.repoImpliedYield, (resultsGpu.repoImpliedYield), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.marketRepoRate, (resultsGpu.marketRepoRate), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaMemcpy(resultsFromGpu.bondForwardVal, (resultsGpu.bondForwardVal), numRepos*sizeof(dataType), cudaMemcpyDeviceToHost);;
		cudaThreadSynchronize();


		double totPrice = 0.0;
		for (int numRepo = 0; numRepo < numRepos; numRepo++)
		{
			totPrice += resultsFromGpu.dirtyPrice[numRepo];
		}
		printf("Sum of repo dirty price on GPU: %f\n\n", totPrice);
		printf("Computed info on GPU for Repo at index %d\n", numRepos/2);
		printf("Dirty Price: %f\n", resultsFromGpu.dirtyPrice[numRepos/2]);
		printf("Accrued Amount: %f\n", resultsFromGpu.accruedAmountSettlement[numRepos/2]);
		printf("Accrued Amount at delivery: %f\n", resultsFromGpu.accruedAmountDeliveryDate[numRepos/2]);
		printf("Clean Price: %f\n", resultsFromGpu.cleanPrice[numRepos/2]);
		printf("Forward Spot Income: %f\n", resultsFromGpu.forwardSpotIncome[numRepos/2]);
		printf("Underlying Forward Income: %f\n", resultsFromGpu.underlyingBondFwd[numRepos/2]);
		printf("Repo NPV: %f\n", resultsFromGpu.repoNpv[numRepos/2]);
		printf("Repo Clean Forward Price: %f\n", resultsFromGpu.repoCleanForwardPrice[numRepos/2]);
		printf("Repo Dirty Forward Price: %f\n", resultsFromGpu.repoDirtyForwardPrice[numRepos/2]);
		printf("Repo Implied Yield: %f\n", resultsFromGpu.repoImpliedYield[numRepos/2]);
		printf("Market Repo Rate: %f\n", resultsFromGpu.marketRepoRate[numRepos/2]);
		printf("Bond Forward Val: %f\n\n", resultsFromGpu.bondForwardVal[numRepos/2]);


		gettimeofday(&start, NULL);
		getRepoResultsGpuCpu(inArgsHost, resultsHost, numRepos);
		gettimeofday(&end, NULL);

		totPrice = 0.0;
		for (int numRepo = 0; numRepo < numRepos; numRepo++)
		{
			totPrice += resultsHost.dirtyPrice[numRepo];
		}

		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		mtimeCpu = ((seconds) * 1000 + ((float)useconds)/1000.0) + 0.5f;

		printf("Run on CPU\n");
		printf("Processing time on CPU: %f (ms)  \n\n", mtimeCpu);

		printf("Sum of repo dirty price on CPU: %f\n\n", totPrice);
		printf("Computed info on CPU for Repo at index %d\n", numRepos/2);
		printf("Dirty Price: %f\n", resultsHost.dirtyPrice[numRepos/2]);
		printf("Accrued Amount: %f\n", resultsHost.accruedAmountSettlement[numRepos/2]);
		printf("Accrued Amount at delivery: %f\n", resultsHost.accruedAmountDeliveryDate[numRepos/2]);
		printf("Clean Price: %f\n", resultsHost.cleanPrice[numRepos/2]);
		printf("Forward Spot Income: %f\n", resultsHost.forwardSpotIncome[numRepos/2]);
		printf("Underlying Forward Income: %f\n", resultsHost.underlyingBondFwd[numRepos/2]);
		printf("Repo NPV: %f\n", resultsHost.repoNpv[numRepos/2]);
		printf("Repo Clean Forward Price: %f\n", resultsHost.repoCleanForwardPrice[numRepos/2]);
		printf("Repo Dirty Forward Price: %f\n", resultsHost.repoDirtyForwardPrice[numRepos/2]);
		printf("Repo Implied Yield: %f\n", resultsHost.repoImpliedYield[numRepos/2]);
		printf("Market Repo Rate: %f\n", resultsHost.marketRepoRate[numRepos/2]);
		printf("Bond Forward Val: %f\n\n", resultsHost.bondForwardVal[numRepos/2]);

		printf("Speedup using GPU: %f\n", mtimeCpu/mtimeGpu);



		cudaFree(resultsGpu.dirtyPrice);
		cudaFree(resultsGpu.accruedAmountSettlement);;
		cudaFree(resultsGpu.accruedAmountDeliveryDate);;
		cudaFree(resultsGpu.cleanPrice);;
		cudaFree(resultsGpu.forwardSpotIncome);;
		cudaFree(resultsGpu.underlyingBondFwd);;
		cudaFree(resultsGpu.repoNpv);;
		cudaFree(resultsGpu.repoCleanForwardPrice);;
		cudaFree(resultsGpu.repoDirtyForwardPrice);;
		cudaFree(resultsGpu.repoImpliedYield);;
		cudaFree(resultsGpu.marketRepoRate);;
		cudaFree(resultsGpu.bondForwardVal);;

		cudaFree(inArgsGpu.discountCurve);
		cudaFree(inArgsGpu.repoCurve);
		cudaFree(inArgsGpu.settlementDate);
		cudaFree(inArgsGpu.deliveryDate);
		cudaFree(inArgsGpu.maturityDate);
		cudaFree(inArgsGpu.repoDeliveryDate);
		cudaFree(inArgsGpu.bondCleanPrice);;
		cudaFree(inArgsGpu.bond);
		cudaFree(inArgsGpu.dummyStrike);;


		free(resultsHost.dirtyPrice);
		free(resultsHost.accruedAmountSettlement);;
		free(resultsHost.accruedAmountDeliveryDate);;
		free(resultsHost.cleanPrice);;
		free(resultsHost.forwardSpotIncome);;
		free(resultsHost.underlyingBondFwd);;
		free(resultsHost.repoNpv);;
		free(resultsHost.repoCleanForwardPrice);;
		free(resultsHost.repoDirtyForwardPrice);;
		free(resultsHost.repoImpliedYield);;
		free(resultsHost.marketRepoRate);;
		free(resultsHost.bondForwardVal);;

		free(resultsFromGpu.dirtyPrice);
		free(resultsFromGpu.accruedAmountSettlement);;
		free(resultsFromGpu.accruedAmountDeliveryDate);;
		free(resultsFromGpu.cleanPrice);;
		free(resultsFromGpu.forwardSpotIncome);;
		free(resultsFromGpu.underlyingBondFwd);;
		free(resultsFromGpu.repoNpv);;
		free(resultsFromGpu.repoCleanForwardPrice);;
		free(resultsFromGpu.repoDirtyForwardPrice);;
		free(resultsFromGpu.repoImpliedYield);;
		free(resultsFromGpu.marketRepoRate);;
		free(resultsFromGpu.bondForwardVal);

		free(inArgsHost.discountCurve);
		free(inArgsHost.repoCurve);
		free(inArgsHost.settlementDate);
		free(inArgsHost.deliveryDate);
		free(inArgsHost.maturityDate);
		free(inArgsHost.repoDeliveryDate);
		free(inArgsHost.bondCleanPrice);
		free(inArgsHost.bond);
		free(inArgsHost.dummyStrike);

	}
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
	runRepoEngine();
	char c;
	c = getchar();
	printf("%c\n", c);
}

