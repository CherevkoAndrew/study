#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
#include <cmath>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
using namespace std;

double aa = 1.;
double A = 6.;
double B = 4;
double eps = 1.e-12;
MPI_Status status;

double* TridiagonalSolve ( double *a,  double *b, double *c, double *d, int n){
 
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

double omega( double t, double x) {
	return pow(pow(x - A, 2.) / (6. * aa * (B - t)) , 2.);
}

int main(int argc, char *argv[]) {
	double aw = 0;
	double bw = 1;
	double at = 0;
	double bt = 1.;
	int nw = 100;
	int nt = 100;
	double hStep = (bw - aw) / (nw - 1);
	double tStep = (bt - at) / (nt - 1);
	double newtonDE = 1;// погрешность ньютона

	MPI_Init(&argc, &argv);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		double **w = new double*[nt];
		for (int i = 0; i < nt; ++i)
		{
			w[i] = new double[nw];
		}

		#pragma omp parallel for
		for (int i = 0; i < nw; ++i)
		{
			w[0][i] = pow(pow((double)i*hStep - A, 2) / (6.*aa*B),2);
		}

		#pragma omp parallel for
		for (int i = 0; i < nt; ++i)
		{
			w[i][0] = pow(A, 4) / pow(6.*aa*(B - tStep*i), 2);
			w[i][nw - 1] = pow(1.-A, 4) / pow(6.*aa*(B - tStep*i), 2);
		}

		double *a = new double[nw];
		double *b = new double[nw];
		double *c = new double[nw];
		double *d = new double[nw];

		double *dw;
		
		for (int k = 0; k < nt - 1; ++k)
		{
			newtonDE = 1;
			for (int i = 0; i < nw; ++i)
			{
				w[k+1][i] = w[k][i];
			}

			while(newtonDE > eps)
			{
				MPI_Send( w[k], nw, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
				for (int i = 1; i < 5; ++i)
				{
					MPI_Send( w[k+1], nw, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				}
				MPI_Recv(d, nw, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,&status);
				MPI_Recv(a, nw, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD,&status);
				MPI_Recv(b, nw, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD,&status);
				MPI_Recv(c, nw, MPI_DOUBLE, 4, 0, MPI_COMM_WORLD,&status);
				b[0] = 1;
				b[nw-1] = 1;
				a[nw-1] = 0;
				c[0] = 0;				

				dw = TridiagonalSolve(a, b, c, d, nw);

				#pragma omp parallel for
				for (int i = 0; i < nw; ++i)
				{
					w[k+1][i] += dw[i];
				}

				newtonDE = 0.;
				#pragma omp parallel for
				for (int i = 0; i < nw; ++i)
				{
					newtonDE += fabs(dw[i]);
				}
				for (int i = 1; i < 5; ++i)
				{
					MPI_Send( &newtonDE, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				}
			}
		}
		double errAvg = 0.;
		double errMin = 10.;
		double errMax = 0.;
		double deltaAvg = 0.;
		double deltaMin = 10.;
		double deltaMax = 0.;

		for (int i = 0; i < nt; ++i)
		{
			for (int j = 0; j < nw; ++j)
			{
				if ( i % 10 == 0 && j % 10 == 0) cout <<setw(10)<< w[i][j] << " "; 
				deltaAvg += fabs((omega(i*tStep,j*hStep) - w[i][j]));
				errAvg += fabs((omega(i*tStep,j*hStep) - w[i][j]) / omega(i*tStep,j*hStep));
				if (fabs(omega(i*tStep,j*hStep) - w[i][j]) > deltaMax) {
					deltaMax = fabs((omega(i*tStep,j*hStep) - w[i][j]));
					errMax = fabs((omega(tStep*i,hStep*j) - w[i][j]) / omega(tStep*i,hStep*j));
				} else if ((omega(i*tStep,j*hStep) - w[i][j]) < deltaMin) {
					deltaMin = fabs((omega(i*tStep,j*hStep) - w[i][j]));
					errMin = fabs((omega(i*tStep,j*hStep) - w[i][j]) / omega(i*tStep,j*hStep));
				}
			}
			if (i % 10 == 0) cout << endl; 
		}

		int size = nt * nw;
		cout << errMin << " " << deltaMin  << endl;
		cout << errAvg /size << " " << deltaAvg / size << endl;
		cout << errMax << " " << deltaMax << endl;
		ofstream myfile;
  			myfile.open ("func.dat");
  			for (int i = 0; i < nt; ++i)
  			{
  				for (int j = 0; j < nw; ++j)
  				{
  					myfile << tStep * i << '\t' << hStep * j << '\t' << w[i][j] << endl;
  				}
  			}
	} else {
		if (world_rank == 1){
			double *d = new double[nw];
			double *wk = new double[nw];
			double *wNext = new double[nw];

			int p = nw/2;

			double *tridiagonalA = new double[nw];
			double *tridiagonalB = new double[nw];
			double *tridiagonalC = new double[nw];
			double *tridiagonalD = new double[nw];

			double *x = new double[p + 1];

			for (int k = 0; k < nt - 1; ++k )
			{
				newtonDE = 1;
				while(newtonDE > eps) {
					MPI_Recv(wk, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(wNext, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);

					for (int i = 1; i < nw - 1; ++i)
					{
						d[i] = -(wk[i] - wNext[i] + (aa * tStep / hStep / hStep) * ( pow( wNext[i+1] - wNext[i-1], 2.) / (8. * pow(wNext[i], 1./2)) + pow((wNext[i]), 1./2) * (wNext[i-1] - 2.* wNext[i] + wNext[i+1])));		
					}
					d[0] = -(wNext[0] - pow(A, 4.) /  pow( 6.*aa*(B - (k+1)*tStep), 2.));
					d[nw-1] = -(wNext[nw-1] - pow( pow(1.-A, 2) / ( 6. * aa * (B - (k+1)*tStep)), 2.));
					MPI_Send( d, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

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
						for (int i = p-1; i >= 0; i--)
							x[i] = tridiagonalD[i] - tridiagonalC[i] * x[i + 1];
					MPI_Send( x, p, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					MPI_Recv(&newtonDE, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
				}
			}
		} else if(world_rank == 2) {
			double *a = new double[nw];
			double *wNext = new double[nw];

			int p = nw/2;
			int n = nw;

			double *tridiagonalA = new double[nw];
			double *tridiagonalB = new double[nw];
			double *tridiagonalC = new double[nw];
			double *tridiagonalD = new double[nw];

			double *x = new double[n - p + 1];

			for (int k = 0; k < nt - 1; ++k )
			{
				newtonDE = 1;
				while(newtonDE > eps) {
					MPI_Recv(wNext, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					for (int i = 1; i < nw - 1; ++i)
					{
						a[i] = (aa * tStep / hStep / hStep) * ( pow(wNext[i], 1./2) - (-wNext[i-1] + wNext[i+1]) / pow(wNext[i], 1./2) / 4.);
					}

					MPI_Send( a, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

					MPI_Recv(tridiagonalA, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalB, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalC, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					MPI_Recv(tridiagonalD, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);

					tridiagonalA[n-1] /= tridiagonalB[n-1];	
					tridiagonalD[n-1] /= tridiagonalB[n-1];
					for (unsigned int i = n-2; i > p; --i){
						double id = 1. / (tridiagonalB[i] - tridiagonalA[i+1] * tridiagonalC[i]);  
						tridiagonalA[i] *= id;	                         
						tridiagonalD[i] = (tridiagonalD[i] - tridiagonalD[i+1] * tridiagonalC[i]) * id;
					}

					MPI_Send( &(tridiagonalA[p+1]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					MPI_Send( &(tridiagonalD[p+1]), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

					MPI_Recv(x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

					for (int i = 1; i < n - p; i++){
						x[i] = tridiagonalD[i+p] - tridiagonalA[i+p] * x[i - 1];
					}
					MPI_Send(&(x[1]), n-p-1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

					MPI_Recv(&newtonDE, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
				}
			}
		} else if(world_rank == 3) {
			double *b = new double[nw];
			double *wNext = new double[nw];

			for (int k = 0; k < nt - 1; ++k )
			{
				newtonDE = 1;
				while(newtonDE > eps) {
					MPI_Recv(wNext, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);

					for (int i = 1; i < nw - 1; ++i)
					{
						b[i] = -1. + aa * tStep * ( -1./4 * pow(wNext[i], -3./2) * pow((wNext[i+1] - wNext[i-1]) / 2./ hStep ,2)  + 1./2 * pow(wNext[i], -1./2) * (wNext[i-1] - 2. * wNext[i] + wNext[i+1]) / hStep / hStep - 2. / hStep / hStep * pow(wNext[i], 1./2));
					}
					MPI_Send( b, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					MPI_Recv(&newtonDE, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
				}
			}
		} else if(world_rank == 4) {
			double *c = new double[nw];
			double *wNext = new double[nw];

			for (int k = 0; k < nt - 1; ++k )
			{
				newtonDE = 1;
				while(newtonDE > eps) {
					MPI_Recv(wNext, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
					for (int i = 1; i < nw - 1; ++i)
					{
						c[i] = (aa * tStep / hStep / hStep) * ( pow(wNext[i], 1./2) + (-wNext[i-1] + wNext[i+1]) / pow(wNext[i], 1./2) / 4.);
					}
					MPI_Send( c, nw, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
					MPI_Recv(&newtonDE, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,&status);
				}
			}
		}
	}
	MPI_Finalize();
}