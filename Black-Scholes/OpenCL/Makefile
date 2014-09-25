INCLUDE=-I/usr/local/cuda-5.0/include
#LIBPATH=-L${OpenCL_SDK}/OpenCL/common/lib -L${OpenCL_SDK}/shared/lib
LIB=-lOpenCL -lm
all:
	g++ -O3 ${INCLUDE} ${LIBPATH} ${LIB} blackScholesAnalyticEngine.c -o blackScholesAnalyticEngine.exe

clean:
	rm -f *~ *.exe
