FinanceBench
------------

* Scott Grauer-Gray <grauerg@udel.edu>
* Will Killian <killian@udel.edu>
* Robert Searles <rsearles@udel.edu>
* John Cavazos <cavazos@udel.edu>

This contains codes for Black-Scholes, Monte-Carlo, Bonds, and Repo financial applications which can be run on the CPU and GPU.  All the codes are described in the following paper and any work that uses these codes should cite this paper:

"Accelerating Financial Applications on the GPU" which was presented at the Sixth Workshop on General Purpose Processing Using GPUs (GPGPU 6)


README-parameters.txt contains information on the parameters in each application.

Directions (tested for Linux-based systems w/ gcc/g++ compiler, NVIDIA GPU, and CAPS Compiler for HMPP/OpenACC):

For CUDA/OpenCL codes (CUDA available for each application, OpenCL available for Black-Scholes
and Monte-Carlo):

1. Set the PATH and LD_LIBRARY_PATH environment variables to point to the appropriate locations for CUDA/OpenCL.
2. Navigate to CUDA/OpenCL folder in target application.
3. Run Makefile (by using "make" command).
4. Application executable should be create and can be run.

For HMPP/OpenACC Codes:

1. Set the PATH and LD_LIBRARY_PATH environment variables to point to the appropriate locations for CUDA/OpenCL.
2. Set the environment variables needed for the HMPP/OpenACC compilation environment.
3. Navigate to HMPP/OpenACC folder in target application.
4. Run Makefile (by using "make" command).
5. Application executable should be create and can be run.

For CPU/OpenMP Codes (assuming using g++ compiler):

1. Run Makefile (by using "make" command).
2. Application executable should be create and can be run.
