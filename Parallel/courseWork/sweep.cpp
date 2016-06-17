#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
//#include <windows.h>
#include <cmath>
using namespace std;

double* TridiagonalSolve ( double *a,  double *b, double *c, double *d, int n, int p){
 
 	double *x = new double[n];
	
	c[0] /= b[0];	
	d[0] /= b[0];	
	
	for (unsigned int i = 1; i <= p; i++){
		double id = 1. / (b[i] - c[i-1] * a[i]);  
		c[i] *= id;	                        
		d[i] = (d[i] - d[i-1] * a[i]) * id;
	}

	
	a[n-1] /= b[n-1];	
	d[n-1] /= b[n-1];	
	for (unsigned int i = n-2; i > p; --i){
		double id = 1. / (b[i] - a[i+1] * c[i]);  
		a[i] *= id;	                         
		d[i] = (d[i] - d[i+1] * c[i]) * id;
		cout<< a[i]<<" "<< d[i]<<endl;
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


int main(int argc, char *argv[]) {
/*	double x = 1.;
	for (int i = 0; i < 30; ++i)
	{
		x -= (x-555)*(x-555) / (2*(x-555));
		cout << x <<endl;
	}
*/
double a[] = {0,1,8,1,2};
double b[] = {2,3,3,1,3};
double c[] = {1,1,5,2,0};
double d[] = {4, 10, 45, 17, 23};
double* x = TridiagonalSolve(a,b,c,d,5,2);
for (int i = 0; i < 5; ++i)
 	{
 		cout<< x[i]<< ' ';
 	} 	
 	cout<<endl;
}