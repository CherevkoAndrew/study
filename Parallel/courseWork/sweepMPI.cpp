#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
//#include <windows.h>
#include <cmath>
#include <omp.h>
#include <stdio.h>
using namespace std;

MPI_Status status;

int nw = 5;

double* TridiagonalSolve ( double *a,  double *b, double *c, double *d, int n){
 1
	int p = n/2;
	double *x = new double[n];
	MPI_Send( a, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	MPI_Send( a, n, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);
	MPI_Send( b, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	MPI_Send( b, n, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);
	MPI_Send( c, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	MPI_Send( c, n, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);
	MPI_Send( d, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	MPI_Send( d, n, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);
	double alpha, beta, psi, eta;

	MPI_Recv(&alpha, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,&status);
	MPI_Recv(&beta, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,&status);
	MPI_Recv(&psi, 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD,&status);
	MPI_Recv(&eta, 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD,&status);

	x[p] = (beta - alpha * eta) / (1-alpha*psi);

	MPI_Send( &(x[p]), 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	MPI_Send( &(x[p]), 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);

	MPI_Recv(x, p, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,&status);
	
	MPI_Recv(&(x[p+1]), n - p -1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD,&status);
	
	return x;
}

int main(int argc, char *argv[]) {
	if ( MPI_Init(&argc, &argv)) 
	{ 
	cout << "Ошибка запуска, выполнение остановлено " << endl;
	} 


	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		double a[] = {0,1,8,1,2};
		double b[] = {2,3,3,1,3};
		double c[] = {1,1,5,2,0};
		double d[] = {4, 10, 45, 17, 23};
		double* x = TridiagonalSolve(a,b,c,d,5);
		for (int i = 0; i < 5; ++i)
		{
			cout<< x[i]<< ' ';
		} 	
		cout<<endl;
	} else if (world_rank == 1) {
			int p = nw/2;

			double *tridiagonalA = new double[nw];
			double *tridiagonalB = new double[nw];
			double *tridiagonalC = new double[nw];
			double *tridiagonalD = new double[nw];


			double *x = new double[p + 1];


					// tridiagonal solve
					MPI_Recv(tridiagonalA, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalB, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalC, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalD, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					
					tridiagonalC[0] /= tridiagonalB[0];
					tridiagonalD[0] /= tridiagonalB[0];

					

					for (unsigned int i = 1; i <= p; i++){
						double id = 1. / (tridiagonalB[i] - tridiagonalC[i-1] * tridiagonalA[i]);  
						tridiagonalC[i] *= id;	                        
						tridiagonalD[i] = (tridiagonalD[i] - tridiagonalD[i-1] * tridiagonalA[i]) * id;
						
					}		
					MPI_Send( &(tridiagonalC[p]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					MPI_Send( &(tridiagonalD[p]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					
					MPI_Recv(&(x[p]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					///
						for (int i = p-1; i >= 0; i--)
							x[i] = tridiagonalD[i] - tridiagonalC[i] * x[i + 1];
					///
					MPI_Send( x, p, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				
	} else if (world_rank == 2) {
		
			int p = nw/2;
			int n = nw;

			double *tridiagonalA = new double[nw];
			double *tridiagonalB = new double[nw];
			double *tridiagonalC = new double[nw];
			double *tridiagonalD = new double[nw];

			double *x = new double[n - p + 1];

			

			MPI_Recv(tridiagonalA, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
			MPI_Recv(tridiagonalB, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
			MPI_Recv(tridiagonalC, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
			MPI_Recv(tridiagonalD, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
			
			tridiagonalA[n-1] /= tridiagonalB[n-1];	
			tridiagonalD[n-1] /= tridiagonalB[n-1];
			//cout << tridiagonalA[n-1] << " " << tridiagonalD[n-1]<<endl;	
			for (unsigned int i = n-2; i > p; --i){
				double id = 1. / (tridiagonalB[i] - tridiagonalA[i+1] * tridiagonalC[i]);  
				tridiagonalA[i] *= id;	                         
				tridiagonalD[i] = (tridiagonalD[i] - tridiagonalD[i+1] * tridiagonalC[i]) * id;
				//cout << tridiagonalA[i] << " " << tridiagonalD[i]<<endl;
			}

			MPI_Send( &(tridiagonalA[p+1]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			MPI_Send( &(tridiagonalD[p+1]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

			MPI_Recv(x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

			for (int i = 1; i < n - p; i++){
				x[i] = tridiagonalD[i+p] - tridiagonalA[i+p] * x[i - 1];
				cout<<tridiagonalD[i+p]<< " "<< tridiagonalA[i+p]<< " "<<x[i-1]<<endl;
			}
			MPI_Send(&(x[1]), n-p-1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}