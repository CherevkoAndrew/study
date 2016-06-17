#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
//#include <windows.h>
#include <math.h>
using namespace std;

double** create_rand_matrix(int n) {
	srand(time(NULL));
	double** matrix = new double*[n];
	//printf("%d\n",n );
	for (int i = 0; i < n; ++i)
	{
		matrix[i] = new double[n+1];
		for (int j = 0; j < n+1; ++j)
		{
			matrix[i][j] = (double)(rand()%10);
			printf("%f 	", matrix[i][j]);
		}
		printf("\n");
	}
	return matrix;
}

int main(int argc, char *argv[])
{
  
	int n = atoi(argv[1]);

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

		int row, col, outRow, outCol;
		int transpos[n];
		for (int i = 0; i < n; ++i)
		{
			transpos[i] = i;
		}
		
		double** a;
		double* lineBuf = new double[n+1];
		a = create_rand_matrix(n);
		for(int gauss_it = 0; gauss_it < n-1; ++gauss_it) {
			for (int i = 1; i < world_size; ++i)// отослать на каждый поток количество строк, который он должен принять
			{
				linesPerProc = (n-gauss_it)/(world_size-1) + (int)((n-gauss_it)%(world_size-1) >= i);
				MPI_Send(&linesPerProc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				//printf("%d  \n", linesPerProc);
			}
			for (int i = gauss_it; i < n; ++i) //отослать строки
			{
				MPI_Send(a[i], n+1, MPI_DOUBLE, (i-gauss_it) %(world_size-1) + 1, 0, MPI_COMM_WORLD);
			
			}
		
			//получить номера столбцов с максимумами
			MPI_Recv(&col, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,&status);
			row = gauss_it;
			//printf("%d %d %f\n", row, col,a[row][col]);
		
			for (int i = 1; i < n - gauss_it; ++i)
			{
				outRow = i+gauss_it;
				//printf("start\n");
				MPI_Recv(&outCol, 1, MPI_INT, i%(world_size-1) + 1, 0, MPI_COMM_WORLD,&status);
				//printf("%d %d %f\n", outRow, outCol,a[outRow][outCol]);
				if (fabs(a[outRow][outCol]) > fabs(a[row][col])) {
					row = outRow;
					col = outCol;
				}
			}
			//printf("%d %d %f\n", row, col,a[row][col]);

			if(gauss_it != col){
				for (int i = 0; i < n; ++i)
				{
					swap(a[i][gauss_it], a[i][col]);
				}
				swap(transpos[gauss_it], transpos[col]);
			}
			if(gauss_it != row)
				for (int i = 0; i < n+1; ++i)
				{
					swap(a[gauss_it][i], a[row][i]);
				}
			for (int i = 1; i < world_size; ++i)
			{
				linesPerProc = (n-gauss_it - 1)/(world_size-1) + (int)((n-gauss_it - 1)%(world_size-1) >= i);
				MPI_Send(&linesPerProc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(a[gauss_it], n+1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
			for (int i = gauss_it + 1; i < n; ++i)
			{
				MPI_Send(a[i], n+1, MPI_DOUBLE, (i-gauss_it-1) %(world_size-1) + 1, 0, MPI_COMM_WORLD);
			}
			for (int i = 0; i < n - gauss_it - 1; ++i)
			{
				//printf("ready\n");
				MPI_Recv(lineBuf, n+1, MPI_DOUBLE, i%(world_size-1) + 1, 0, MPI_COMM_WORLD,&status);
				//printf("ready\n");
				for (int j = 0; j < n+1; ++j)
				{
				//	printf("ready\n");
					a[gauss_it+1+i][j] = lineBuf[j];
				}
			}

		}

		for (int i = 1; i < world_size; ++i)// отослать на каждый поток количество строк, который он должен принять
		{
			linesPerProc = (n)/(world_size-1) + (int)((n)%(world_size-1) >= i);
			MPI_Send(&linesPerProc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			//printf("%d\n", linesPerProc);
		}
		int rowNum;
		double x[n];	
		for (int i = 0; i < n; ++i)
		{
			MPI_Send(a[n-i-1], n+1, MPI_DOUBLE, (i) %(world_size-1) + 1, 0, MPI_COMM_WORLD);
			rowNum = n-i-1;
			MPI_Send(&rowNum, 1, MPI_INT, (i) %(world_size-1) + 1, 0, MPI_COMM_WORLD);
			//printf("%d\n", i);
		}

		for (int i = 0; i < n; ++i)
		{
			MPI_Recv(&(x[n-i-1]), 1, MPI_DOUBLE, (i) %(world_size-1) + 1,0, MPI_COMM_WORLD, &status);
			for (int j = 1; j < world_size; ++j)
			{
				MPI_Send(&(x[n-i-1]), 1, MPI_DOUBLE, j,0, MPI_COMM_WORLD);				/* code */
			}
		}
		for (int i = 0; i < n; ++i)
		{
			//printf("%f  \n", x[i]);
		}
		double xUlt[n];
		for (int i = 0; i < n; ++i)
		{
			xUlt[transpos[i]] = x[i];
		}
		for (int i = 0; i < n; ++i)
		{
			printf("x[%d] = %f  \n",i, xUlt[i]);
		}
		double eps = a[0][n];
		for (int i = 0; i < n; ++i)
		{
			//printf("%f \n",a[0][i] );
			eps -= x[i] * a[0][i];
		}
		printf("eps = %f\n",eps );
		for (int i = 0; i < n; ++i)
		{
			delete[] a[i];
		}
		delete[] a;

	} else {
		double* line = new double[n+1];
		double* leadingLine = new double[n+1];
		int num;

		
		for (int gauss_it = 0; gauss_it < n-1; ++gauss_it)
		{
			/* code */
			MPI_Recv(&linesPerProc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			for (int j = 0; j < linesPerProc; ++j)
			{
			
				num = gauss_it;

				MPI_Recv(line, n+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

				for (int i = gauss_it; i < n; ++i)
				{
					if(fabs(line[i]) > fabs(line[num]))
						num = i;
					
				}
				//printf("%f %d\n", line[num], world_rank);
				MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);	
			}

			MPI_Recv(&linesPerProc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);


			MPI_Recv(leadingLine, n+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

			//printf("lpp:%d\n", linesPerProc );
			for (int j = 0; j < linesPerProc; ++j)
			{
			
				num = gauss_it;

				MPI_Recv(line, n+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

				for (int i = gauss_it+1; i < n+1; ++i)
				{
					line[i] -= line[gauss_it] * leadingLine[i] / leadingLine[gauss_it];					
				}
				//printf("%f %d\n", line[num], world_rank);
				MPI_Send(line, n+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);	
				//printf("ok\n");
			}




		}

			MPI_Recv(&linesPerProc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			//printf("%d\n",linesPerProc );
			
			double** backPassLines = new double*[linesPerProc];
			int rangs[linesPerProc];
			int received = 0;
			double x[n];
			double nextX;
			for (int i = 0; i < linesPerProc; ++i)
			{
				backPassLines[i] = new double[n+1];
				MPI_Recv(backPassLines[i], n+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
				MPI_Recv(&(rangs[i]), 1, MPI_INT, 0,0,MPI_COMM_WORLD, &status);
			}

			for (int localLine = 0; localLine < linesPerProc; ++localLine)
			{
				for (int column = n-received-1; column > rangs[localLine]; --column)//rangs - глобальный номер строки
				{
					MPI_Recv(x+column, 1, MPI_DOUBLE, 0,0,MPI_COMM_WORLD, &status);
					++received;
					//обновить строки
					for (int i = 0; i < linesPerProc; ++i)
					{
						backPassLines[i][n] -= x[column]*backPassLines[i][column];
					}
				}
				nextX = backPassLines[localLine][n] / backPassLines[localLine][rangs[localLine]];
				MPI_Send(&nextX, 1, MPI_DOUBLE, 0,0, MPI_COMM_WORLD);
			}
	}

	MPI_Finalize();
}

	
	