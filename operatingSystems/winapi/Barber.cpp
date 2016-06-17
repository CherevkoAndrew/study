
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define SEATS 5
#define PERIOD 100
#define MAX_CONS 20

HANDLE barberReady;//1x sem
HANDLE accessWRSeats;//1x sem
HANDLE custReady;//SEATS x sem 
HANDLE initCust;//1x sem
int numberOfFreeWRSeats;
int consNum = 0;
int maxCons = MAX_CONS;

bool barberSleeping = true;

HANDLE barberTh;
HANDLE customersTh[MAX_CONS];

DWORD WINAPI barber( LPVOID lpParam )
{
	for(int i = 0; i < maxCons;){
		barberSleeping = true;

		printf("barber is sleeping\n");
		while(WaitForSingleObject( //ждем пока клиент разбудит
			custReady,   // handle to semaphore
			0L) != WAIT_OBJECT_0);
		barberSleeping = false;
		/*ReleaseSemaphore( 
			barberSleeping,  // handle to semaphore
            1,            // increase count by one
            NULL);
			*/
		ReleaseSemaphore( 
			barberReady,  // handle to semaphore
            1,            // increase count by one
            NULL);
		++i;
		printf("haircut\n");
		Sleep(rand()%PERIOD);
		//lock barber ready
		while(WaitForSingleObject( //сиотрим на стулья
				accessWRSeats,   // handle to semaphore
				0L) != WAIT_OBJECT_0);
		while(numberOfFreeWRSeats != SEATS) {
			while(WaitForSingleObject( //забираем клиента из зала
			custReady,   // handle to semaphore
			0L) != WAIT_OBJECT_0);
			++numberOfFreeWRSeats;//освобождаем стул
			printf("barber ready\n");
			ReleaseSemaphore( 
				barberReady,  // handle to semaphore
				1,            // increase count by one
				NULL);
			
			ReleaseSemaphore( 
				accessWRSeats,  // handle to semaphore
				1,            // increase count by one
				NULL);
			++i;
			
			Sleep(rand()%PERIOD);
			
			while(WaitForSingleObject( //сиотрим на стулья
				accessWRSeats,   // handle to semaphore
				0L) != WAIT_OBJECT_0);

		}
		ReleaseSemaphore( 
				accessWRSeats,  // handle to semaphore
				1,            // increase count by one
				NULL);
	}
	return 0;
}

DWORD WINAPI customer( LPVOID lpParam )
{
	while(WaitForSingleObject( //ждем пока клиент разбудит
		initCust,   // handle to semaphore
			0L) != WAIT_OBJECT_0);
	int id = consNum++;
	ReleaseSemaphore( 
		initCust,  // handle to semaphore
				1,            // increase count by one
				NULL);

	if (barberSleeping) {
		printf("customer %d : wake up barber\n",id);
		ReleaseSemaphore( 
				custReady,  // handle to semaphore
				1,            // increase count by one
				NULL);
	} else {////////
		
		while(WaitForSingleObject( 
			accessWRSeats,   // handle to semaphore
				0L) != WAIT_OBJECT_0);
		if ( numberOfFreeWRSeats > 0) {
			numberOfFreeWRSeats--;
			printf("customer %d : waiting.-%d seats left\n",id, numberOfFreeWRSeats);
			ReleaseSemaphore( 
				custReady,  // handle to semaphore
				1,            // increase count by one
				NULL);
			ReleaseSemaphore( 
				accessWRSeats,  // handle to semaphore
				1,            // increase count by one
				NULL);

			while(WaitForSingleObject( 
				barberReady,   // handle to semaphore
				0L) != WAIT_OBJECT_0);
			printf("customer %d : haircut\n",id);
		} else {
			ReleaseSemaphore( 
				accessWRSeats,  // handle to semaphore
				1,            // increase count by one
				NULL);
			printf("customer %d : Leave without a haircut\n",id);
		}
	}
	return 0;
}

int main(void) {
	srand (time(NULL));

	barberReady = CreateSemaphore( 
        NULL,           // default security attributes
        0,  // initial count
        1,  // maximum count
        NULL);

	accessWRSeats = CreateSemaphore( 
        NULL,           // default security attributes
        1,  // initial count
        1,  // maximum count
        NULL);

	custReady = CreateSemaphore( 
        NULL,           // default security attributes
        0,  // initial count
		SEATS,  // maximum count
        NULL);

	initCust =  CreateSemaphore( 
        NULL,           // default security attributes
        1,  // initial count
        1,  // maximum count
        NULL);

	numberOfFreeWRSeats = SEATS;
	int numberOfCustomers = 0;

	CreateThread( 
                     NULL,       // default security attributes
                     0,          // default stack size
					 (LPTHREAD_START_ROUTINE) barber, 
                     NULL,       // no thread function arguments
                     0,          // default creation flags
                     NULL);
	Sleep(50);
	for(int i = 0; i < maxCons;++i){
		customersTh[i] = CreateThread( 
                     NULL,       // default security attributes
                     0,          // default stack size
					 (LPTHREAD_START_ROUTINE) customer, 
                     NULL,       // no thread function arguments
                     0,          // default creation flags
                     NULL);
		Sleep(rand()%PERIOD/2);
	}

	_getch();
	TerminateThread(barberTh,NULL);
	CloseHandle(barberTh);
	CloseHandle(barberReady);
	CloseHandle(custReady);
	CloseHandle(accessWRSeats);
	for (int i = 0; i < maxCons; ++i) {
		CloseHandle(customersTh[i]);
	}
	CloseHandle(barberTh);
	system("pause");
	return 0;
}