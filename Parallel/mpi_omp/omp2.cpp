#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
int main()
{
const int SIZE= 1000;
srand(time(NULL));
//int sumSec=0;
int arr[SIZE];
int max =0;
int min = 10000000;
int parmax =0;
int parmin = 10000000;
for(int i=0;i<SIZE; i++) {
	arr[i]=rand()%1000000;
//	sumSec+=arr[i];
	if(arr[i]>max){
		max=arr[i];
	}
	if(arr[i] < min) {
		min = arr[i];
	}
}
//int sumPar=0;
int parMax[4];
int parMin[4];

#pragma omp parallel
{
	int localMax = 0;
	int localMin = 10000000;
	#pragma omp for 
	for(int i=0;i<SIZE; i++){
		if( arr[i]>localMax )
			localMax=arr[i];
		if(arr[i] < localMin)
			localMin = arr[i];
	}
	parMax[omp_get_thread_num()] = localMax;
	parMin[omp_get_thread_num()] = localMin;

}
	//printf("%d    %d\n" , sumSec , sumPar );
	printf("  %d  %d    %d	%d	%d\n", max  , parMax[0] , parMax[1], parMax[2] , parMax[3] );
	printf("  %d  %d    %d	%d	%d\n", min  , parMin[0] , parMin[1], parMin[2] , parMin[3] );
	for (int index = 0; index < 4; ++index)
	{
		if(parMax[index] > parmax)
			parmax = parMax[index];
		if(parMin[index] < parmin)
			parmin = parMin[index];
	}
	printf("%d	%d\n", parmax, parmin);
}