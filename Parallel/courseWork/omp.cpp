#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
//#include <windows.h>
#include <cmath>
#include <omp.h>
#include <stdio.h>
using namespace std;

double aa = 1.;
double A = 20.;
double B = 15.6;
double eps = 1.e-10;

double* TridiagonalSolve ( double *a,  double *b, double *c, double *d, int n){
 
 	/*double *x = new double[n];
	
	c[0] /= b[0];	
	d[0] /= b[0];	
	
	for (unsigned int i = 1; i < n; i++){
		double id = 1 / (b[i] - c[i-1] * a[i]);  
		c[i] *= id;	                         
		d[i] = (d[i] - d[i-1] * a[i]) * id;
	}
 
	
	x[n - 1] = d[n - 1];
	for (int i = n - 2; i >= 0; i--)
		x[i] = d[i] - c[i] * x[i + 1];
	return x;*/
	int p = n/2;
	double *x = new double[n];
	


	#pragma omp parallel sections
	{
		#pragma omp section
		{
			c[0] /= b[0];	
			d[0] /= b[0];
			for (unsigned int i = 1; i <= p; i++){
				double id = 1. / (b[i] - c[i-1] * a[i]);  
				c[i] *= id;	                        
				d[i] = (d[i] - d[i-1] * a[i]) * id;
			}
			//printf("%d\n", omp_get_thread_num());
		}

		#pragma omp section
		{
			a[n-1] /= b[n-1];	
			d[n-1] /= b[n-1];	
			for (unsigned int i = n-2; i > p; --i){
				double id = 1. / (b[i] - a[i+1] * c[i]);  
				a[i] *= id;	                         
				d[i] = (d[i] - d[i+1] * c[i]) * id;
			}
			//printf("%d\n", omp_get_thread_num());
		}
	}
	
	x[p] = (d[p] - c[p] * d[p+1]) / (1-c[p]*a[p+1]);
	
	/* Now back substitute. */
	//x[n - 1] = d[n - 1];
	for (int i = p-1; i >= 0; i--)
		x[i] = d[i] - c[i] * x[i + 1];

	for (int i = p+1; i < n; i++)
		x[i] = d[i] - a[i] * x[i - 1];
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
	int nw = 10;
	int nt = 10;
	double hStep = (bw - aw) / (nw - 1);
	double tStep = (bt - at) / (nt - 1);

	//cout <<tStep<< " "<<hStep<<endl;

	for (int i = 0; i < nt; ++i)
	{
		for (int j = 0; j < nw; ++j)
		{
	//		cout << omega(tStep * i , hStep * j)<<"  ";
		}
	//	cout <<endl;
	}
	//cout << endl;

	double **w = new double*[nt];
	for (int i = 0; i < nt; ++i)
	{
		w[i] = new double[nw];
	}
/////////////////////////////////////   omp  КРАЙОВІ УМОВИ
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
//////////////////////////////////////

	double *a = new double[nw];
	double *b = new double[nw];
	double *c = new double[nw];
	double	 *d = new double[nw];

	double *dw;

	//int k = 0;

	
	double e = 1;
	for (int k = 0; k < nt - 1; ++k)
	{
		e = 1;
		//cout << k << endl;
		for (int i = 0; i < nw; ++i)
		{
			w[k+1][i] = w[k][i];
		//	cout << w[k][i] << "  ";
		}
		//cout<< endl<<endl;
		while(e > eps)
		{
			///////////////// 
			#pragma omp parallel for
			for (int i = 1; i < nw - 1; ++i)
			{
				//printf("%d %d\n", k, omp_get_thread_num());
				a[i] = (aa * tStep / hStep / hStep) * ( pow(w[k+1][i], 1./2) - (-w[k+1][i-1] + w[k+1][i+1]) / pow(w[k+1][i], 1./2) / 4.);   //aa * tStep / hStep * ( pow(w[k+1][i], 1./2) / hStep - pow(w[k+1][i], -1./2) * (w[k+1][i+1] - w[k+1][i-1]) / 2.);
				b[i] = -1. + aa * tStep * ( -1./4 * pow(w[k+1][i], -3./2) * pow((w[k+1][i+1] - w[k+1][i-1]) / 2./ hStep ,2)  + 1./2 * pow(w[k+1][i], -1./2) * (w[k+1][i-1] - 2. * w[k+1][i] + w[k+1][i+1]) / hStep / hStep - 2. / hStep / hStep * pow(w[k+1][i], 1./2));
				c[i] = (aa * tStep / hStep / hStep) * ( pow(w[k+1][i], 1./2) + (-w[k+1][i-1] + w[k+1][i+1]) / pow(w[k+1][i], 1./2) / 4.); //aa * tStep / hStep * ( pow(w[k+1][i], 1./2) / hStep  + pow(w[k+1][i], -1./2) * (w[k+1][i+1] - w[k+1][i-1]) / 2. );

				d[i] = -(w[k][i] - w[k+1][i] + (aa * tStep / hStep / hStep) * ( pow( w[k+1][i+1] - w[k+1][i-1], 2.) / (8. * pow(w[k+1][i], 1./2)) + pow((w[k+1][i]), 1./2) * (w[k+1][i-1] - 2.* w[k+1][i] + w[k+1][i+1])));
			}
			b[0] = 1;
			b[nw-1] = 1;
			a[nw-1] = 0;
			c[0] = 0;

			d[0] = -(w[k+1][0] - pow(A, 4.) /  pow( 6.*aa*(B - (k+1)*tStep), 2.));
			d[nw-1] = -(w[k+1][nw-1] - pow( pow(1.-A, 2) / ( 6. * aa * (B - (k+1)*tStep)), 2.));
			///////////////////
			
			dw = TridiagonalSolve(a, b, c, d, nw);
			/////////// omp
			#pragma omp parallel for
			for (int i = 0; i < nw; ++i)
			{
				w[k+1][i] += dw[i];
			}
			////////// 

			e = 0.;
			#pragma omp parallel for
			for (int i = 0; i < nw; ++i)
			{
				e += fabs(dw[i]);
			}
			//cout<<e<<endl;
		}
	}
	///// omp
	double err = 0.;
	for (int i = 0; i < nt; ++i)
	{
		err = 0;
		for (int j = 0; j < nw; ++j)
		{
			err += (omega(i*tStep,j*hStep) - w[i][j]) / omega(i*tStep,j*hStep);
		}
		cout <<"exp = "<<err<<" "<<err / (nw)<<endl;
	}
	

		for (int i = 0; i < nt; ++i)
	{
		delete [] w[i];
	}
}

