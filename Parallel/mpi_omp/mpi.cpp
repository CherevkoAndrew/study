#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
//#include <windows.h>
using namespace std;

float* create_rand_nums(int size);

float compute_avg(float* array, int size);

int main(int argc, char *argv[])
{
  if ( MPI_Init(&argc, &argv)) 
  { 
    cout << "Ошибка запуска, выполнение остановлено " << endl;
   // MPI_Abort(MPI_COMM_WORLD, rc);
  } 
  int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

int num_elements_per_proc = 100;//atoi(argv[1]);
  srand(time(NULL));

  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

// Create a buffer that will hold a subset of the random numbers
float *sub_rand_nums = new float [num_elements_per_proc];
cout<<"OK\n";

MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, 
	sub_rand_nums, num_elements_per_proc, MPI_FLOAT, 
	0, MPI_COMM_WORLD);
cout<<"OK\n";
float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);

float *sub_avgs = 0;
if (world_rank == 0) {
	sub_avgs = new float [world_size];
}
MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0,
           MPI_COMM_WORLD);

if (world_rank == 0) {
  	float avg = compute_avg(sub_avgs, world_size);
  	cout<<avg<<"\n";
}

MPI_Finalize();
return 0;
}

float* create_rand_nums(int size) {
	srand(time(NULL));
	float* result = new float[size];
	for (int i = 0; i < size; ++i)
	{
		result[i] = rand();
	}
	return result;
}

float compute_avg(float* array, int size) {
	float result = 0;
	for (int i = 0; i < size; ++i)
	{
		result += array[i];
	}
	return result / size;
}