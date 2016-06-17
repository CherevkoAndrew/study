#include "mpi.h"
#include <iostream>
#include <float.h>
#include <stdlib.h>
using namespace std
;
typedef struct 
{
	float min, max;
} bounds;

void mergeBounds( void *in, void *inout, int *len, MPI_Datatype *dptr)
{

	cout<<((bounds*)in)->min<<" "<<((bounds*)in)->max<<" 	process :"<<in<<"\n";
		if ( ((bounds*)inout)->min > ((bounds*)in)->min )
			((bounds*)inout)->min = ((bounds*)in)->min;
		if ( ((bounds*)inout)->max < ((bounds*)in)->max )
			((bounds*)inout)->max = ((bounds*)in)->max;
}

bounds* compute_bounds( float* array, int size) {
	bounds* result = new bounds;
	result->min = FLT_MAX;
	result->max = FLT_MIN;

	for (int i = 0; i < size; ++i)
	{
		if (array[i] < result->min) 
			result->min = array[i];
		if (array[i] > result->max)
			result->max = array[i];
	}

	return result;
}

float* create_rand_nums(int size) {
	srand(time(NULL));
	float* result = new float[size];
	for (int i = 0; i < size; ++i)
	{
		result[i] = rand() % 1000000;
	}
	return result;
}


int main(int argc, char *argv[])
{
	
	int num_elements_per_proc = atoi(argv[1]);;

	if ( MPI_Init(&argc, &argv)) 
	{ 
		cout << "Ошибка запуска, выполнение остановлено " << endl;
	} 

	MPI_Op myOp; 
	MPI_Datatype btype; 
	MPI_Type_contiguous( 2, MPI_FLOAT, &btype ); 
	MPI_Type_commit( &btype ); 
	MPI_Op_create( mergeBounds, true, &myOp ); 
	
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	float* rand_nums;
	if (world_rank == 0) {
		rand_nums = create_rand_nums(num_elements_per_proc * world_size);
	}
	
	bounds* localBounds;// = new bounds;
	float* sub_rand_nums = new float[num_elements_per_proc];
 
	MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, 
		sub_rand_nums, num_elements_per_proc, MPI_FLOAT, 
		0, MPI_COMM_WORLD);

	localBounds = compute_bounds(sub_rand_nums, num_elements_per_proc);

	bounds* globalBounds = new bounds;
	globalBounds->min = FLT_MAX;
	globalBounds->max = FLT_MIN;

	MPI_Reduce(localBounds, globalBounds, 1, btype,
											myOp, 0, MPI_COMM_WORLD);
 // cout<<"100\n";
	if (world_rank == 0) {
	printf("Total min = %f, max = %f\n", globalBounds->min, globalBounds->max);
	}


	MPI_Finalize();
	return 0;
}