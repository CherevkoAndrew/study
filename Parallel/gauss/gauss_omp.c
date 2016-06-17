#include <cfloat>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <float.h>

#ifndef NUM_THREADS
#define NUM_THREADS 2
#endif

double* gaussSolve(double** A, double* b, int n) {
	//find main
	 omp_set_num_threads(NUM_THREADS);
	double* x = new double[n];
	x = new double[n];
	double leadingValue;
	int* leadingPosition = new int[2];

	int** leadingPositionLocal = new int*[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		leadingPositionLocal[i] = new int[2];
	}

	int transpos[n];
	for (int i = 0; i < n; ++i)
	{
		transpos[i] = i;
	}
	double swap;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%f 	", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < n - 1; ++i)
	{
		leadingValue = DBL_MIN;

		#pragma omp parallel
		{
			double localMax = DBL_MIN;
			#pragma omp for
			for (int row = i; row < n; ++row)
			{
				for (int column = i; column < n; ++column)
				{
					if (fabs(A[row][column]) > fabs(localMax))
					{
						localMax = A[row][column];
						leadingPositionLocal[omp_get_thread_num()][0] = row;
						leadingPositionLocal[omp_get_thread_num()][1] = column;
						//printf("%f\n",localMax);
					}
				}
			}
		}

		for (int iterator = 0; iterator < NUM_THREADS; ++iterator)
		{
			if( A[leadingPositionLocal[iterator][0]][leadingPositionLocal[iterator][1]] > leadingValue){
				leadingValue = A[leadingPositionLocal[iterator][0]][leadingPositionLocal[iterator][1]];
				leadingPosition = leadingPositionLocal[iterator];
			}
		}

		if ( i != leadingPosition[0]) 
		{
			for (int item = 0; item < n; ++item)
			{
				swap = A[i][item];
				A[i][item] = A[leadingPosition[0]][item];
				A[leadingPosition[0]][item] = swap;
			}

			swap = b[i];
			b[i] = b[leadingPosition[0]];
			b[leadingPosition[0]] = swap;
		}

		if ( i != leadingPosition[1]) 
		{
			swap = transpos[i];
			transpos[i] = transpos[leadingPosition[1]];
			transpos[leadingPosition[1]] = swap;

			for (int item = 0; item < n; ++item)
			{
				swap = A[item][i];
				A[item][i] = A[item][leadingPosition[1]];
				A[item][leadingPosition[1]] = swap;
			}
		}

		#pragma omp parallel for schedule(dynamic)
			for (int row = i + 1; row < n; ++row)
			{
				for (int column = i+1; column < n; ++column)
				{
					A[row][column] -= A[row][i] * A[i][column] / A[i][i];
				}
				b[row] -= A[row][i] * b[i] / A[i][i];
				//printf("%d\n",omp_get_thread_num() );
			}
	}



	for (int i = 0; i < n; ++i)
	{
		x[transpos[i]] = b[i];
	}


	
	for (int row = n-2; row >=0 ; --row)
	{
		x[transpos[row+1]] /= A[row+1][row+1];
		for (int column = n-1; column > row; --column)
		{
			x[transpos[row]] -= A[row][column] * x[transpos[column]];
		}

		
	}

	x[transpos[0]] /= A[0][0];

	for (int i = 0; i < n; ++i)
	{
		printf("x[%d]=%f \n",i, x[i]);
	}
	printf("\n");
	return x;
}

int main(int argc, char const *argv[])
{
	srand(time(0));
	int n = 5;
	double** a = new double*[n];
	double** c =  new double*[n];
	double* b = new double[n];
	double* d = new double[n];
	double* e = new double[n];
	for (int i = 0; i < n; ++i)
	{
		a[i] = new double[n];
		c[i] = new double[n];
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			a[i][j] = rand()%10;
			c[i][j] = a[i][j];
		}
		b[i] = rand()%10;
		d[i] = b[i];
	}

	//delete [] b;
	e = gaussSolve(a,b,n);
	double eps;
	for (int i = 0; i < n; ++i)
	{
		eps = d[i];
		for (int j = 0; j < n; ++j)
		{
			eps -= c[i][j] * e[j];
		}
		printf("%f\n", eps);
	}

	
	for (int i = 0; i < n; ++i)
	{
		delete a[i];
		delete c[i];
	}
	delete [] a;
	delete [] b;
	delete [] c;
	delete [] d;
	delete [] e;
	return 0;
}