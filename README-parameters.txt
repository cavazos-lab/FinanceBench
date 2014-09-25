README-parameters



Black-Scholes:

Parameters in sample code:
        Cycles through a set of 37 different options, with 19 CALL options and 18 PUT options
        Each option has hard-coded values for the strike, spot, q, r, t, and vol values, as well as the expected output value and tolerances when checking for an error
        5,000,000 total samples are run in parallel in the sample code




Monte-Carlo:

Parameters in sample code:
        The parameters for this code are in the monteCarloConstants.cuh file and can be adjusted there.
        The sample code is run with 400,000 samples in parallel.




Bonds:

Parameters in sample code:

Bond issue date and bond dated date:
Day: random integer value between 1 and 28
Month: random integer value between 1 and 12
Year: value of 1998 or 1999, set randomly

Bond maturity date:
Day: random integer value between 1 and 28
Month: random integer value between 1 and 12
Year: value of 2000 or 2001, set randomly

Bond rate: 
Random float value between 0.03 and 0.13

Other parameters are common in all bonds, see code for parameters

1000000 bonds (with parameters given above varying across bonds) are run in the sample code



Repo:

Parameters in sample code:

Repo Settlement Date: 
Day: random integer value between 1 and 28
Month: value of 1, 2, or 3, set randomly
Year: value of 1999 or 2000, set randomly

Repo Delivery Date: 
Day: random integer value between 1 and 28
Month: value of 9, 10, or 11, set randomly
Year: value of 2000 or 2001, set randomly

Bond issue date and bond dated date:
Day: random integer value between 1 and 28
Month: random integer value between 1 and 12
Year: value of 1997 or 1998, set randomly

Bond maturity date:
Day: random integer value between 1 and 28
Month: random integer value between 1 and 12
Year: value of 2002 or 2003, set randomly

Bond rate: 
Random float value between 0.03 and 0.13

Other parameters are common in all repos, see code for parameters

1000000 repos (with parameters given above varying across repos) are run in the sample code

