#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
//#include <windows.h>
#include <math.h>
using namespace std;

#define MAX_I 10000

int simple(int* vector, int size) {
	int result = MAX_I * 3;
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = i+1; j < size; ++j)
		{
			if ( result > abs(vector[i] - vector[j]))
					result = abs(vector[i] - vector[j]);
		}
	}
	return result;
}

int main(int argc, char *argv[])
{
  
	int n = atoi(argv[1]);
	int *vector = new int[n];

	if ( MPI_Init(&argc, &argv)) 
	{ 
	cout << "Ошибка запуска, выполнение остановлено " << endl;
	} 

	MPI_Status status;

	int linesPerProc;

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		srand(time(0));
		for (int i = 0; i < n; ++i)
		{
			vector[i] = rand() % (2 * MAX_I) - MAX_I;
			printf("%d ", vector[i]);;
		}

		printf("\n\nsimple : %d\n", simple(vector, n));	
		for (int pid = 1; pid < world_size; ++pid)
		{
			MPI_Send(vector, n, MPI_INT, pid, 0, MPI_COMM_WORLD);
		}

		int result = MAX_I * 3;
		int tmp;
		for (int pid = 1; pid < world_size; ++pid)
		{
			MPI_Recv(&tmp, 1, MPI_INT, pid, 0, MPI_COMM_WORLD,&status);
			if ( tmp < result)
				result = tmp;
		}
		printf("final result = %d\n", result );

	} else {
		MPI_Recv(vector, n, MPI_INT, 0, 0, MPI_COMM_WORLD,&status);

		int result = MAX_I * 3;
		for ( int index = world_rank - 1; index < n - 1; index += (world_size-1)) {
			for ( int nextRight = index + 1; nextRight < n; ++nextRight) {
				//printf("%d %d %d\n",world_rank, index, nextRight );
				if ( result > abs(vector[index] - vector[nextRight]))
					result = abs(vector[index] - vector[nextRight]);
			}
		}
		printf("pid : %d  - %d\n", world_rank, result);

		MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}