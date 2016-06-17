// writers_readers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h> 


#define NUM_OF_WRITERS 3
#define NUM_OF_READERS 3
#define NUM_OF_ITER 10
#define PERIOD 200

CRITICAL_SECTION readOpen;
CRITICAL_SECTION writeOpen;
CRITICAL_SECTION readTry;
CRITICAL_SECTION resource;

int writeCount = 0;
int readCount = 0;
int w_last = 0;
int r_last = 0;
int resource_t = 0;

typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef struct Queue {
	Node* head;
	Node* tail;
	//int size;
	//int maxSize;
} Queue;


void push(Queue* q, int data) {
	//if (q->size > 10) return;
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
   // q->size++;
    //printf("push tmp%ld h%ld t%ld\n",(long)tmp,(long)q->head,(long)q->tail);
    if(!(q->head)){
    	q->head = tmp;
    	q->tail = tmp;
    	q->tail->next = NULL;
    	q->head->next = NULL;
    	//printf("push new -h%ld -t%ld\n",(long)q->head,(long)q->tail);
    	//q->size++;
    	//return;
    } else {
	    q->tail->next = tmp;
	    q->tail = tmp;
	    q->tail->next = NULL;
	    //printf("push DONE -h%ld -t%ld\n",(long)q->head,(long)q->tail);
	}
}

int pop(Queue* q) {
	if(!(q->head)) return -1;
    Node* prev = NULL;
  //  q->size--;
    int val;
    //prev = q->head;
    //printf("pop h%ld\n",(long)q->head);
    val = q->head->value;
    //printf("free h%ld\n",(long)q->head);
    
    prev = q->head;
    q->head = q->head->next;
    free(prev);
   
    if(!(q->head)) q->tail = NULL;
    //printf("h%ld t%ld free done \n",(long)q->head,(long)q->tail);
    
    return val;
}


DWORD WINAPI writer( LPVOID lpParam ) {
	//Queue* resource_t = (Queue*)lpParam;
	int id = ++w_last;
	for(int i = 0; i < NUM_OF_ITER; ++i) {

		Sleep(rand()%PERIOD);
		
		EnterCriticalSection(&writeOpen);
			writeCount++;
			if(writeCount == 1) {
				printf("writer %d : waiting for readers exit\n", id);
				EnterCriticalSection(&readTry);
				printf("writer %d : readers blocked\n", id);
			}
		LeaveCriticalSection(&writeOpen);

		printf("writer %d : waiting for resource\n", id);
		EnterCriticalSection(&resource);
		printf("writer %d : resource blocked\n", id);
		//WRITING
		Sleep(rand()%PERIOD);
		resource_t++;
		printf("writer %d : %d\n", id,resource_t);
		//printf("writing\n");
		LeaveCriticalSection(&resource);
		printf("writer %d : resource unblocked\n", id);

		EnterCriticalSection(&writeOpen);
			writeCount--;
			if(!writeCount){
				LeaveCriticalSection(&readTry);
				printf("writer %d : last writer\n", id);
			}
		LeaveCriticalSection(&writeOpen);
	}
	return NULL;
}

DWORD WINAPI reader( LPVOID lpParam ) {
	//Queue* resource_t = (Queue*)lpParam;
	int id = ++r_last;
	for(int i = 0; i < NUM_OF_ITER; ++i) {
		Sleep(rand()%PERIOD);
		printf("reader %d : wait if blocked by writer\n", id);
		EnterCriticalSection(&readTry);
			EnterCriticalSection(&readOpen);
				readCount++;
				if(readCount == 1) {
					printf("reader %d : block resource\n", id);
					EnterCriticalSection(&resource);
				}
			LeaveCriticalSection(&readOpen);
		LeaveCriticalSection(&readTry);
		//READING
		printf("reader %d : reading\n",id);
		Sleep(rand()%PERIOD);
		printf("reader %d : %d\n",id, resource_t);
		EnterCriticalSection(&readOpen);
			readCount--;
			if(!readCount) {
				printf("reader %d : last reader////////////////\n", id);
				LeaveCriticalSection(&resource);
				
			}
		LeaveCriticalSection(&readOpen);
	}
	return NULL;
}

//int readCount = 0;
//int writeCount = 0;


int main(void)
{
	InitializeCriticalSection(&readOpen);
	InitializeCriticalSection(&writeOpen);
	InitializeCriticalSection(&readTry);
	InitializeCriticalSection(&resource);

	srand((unsigned) time(NULL));

	HANDLE writers[ NUM_OF_WRITERS ];
	HANDLE readers[ NUM_OF_READERS ];

	printf("////////////////////////////////\n");

	Queue* q = (Queue*) malloc(sizeof(Queue));

	for(int i = 0; i < NUM_OF_WRITERS; ++i){
		writers[i] = CreateThread( 
                     NULL,       // default security attributes
                     0,          // default stack size
					 (LPTHREAD_START_ROUTINE) writer, 
                     NULL,       // no thread function arguments
                     0,          // default creation flags
                     NULL);
	}

	for(int i = 0; i < NUM_OF_READERS; ++i){
		readers[i] = CreateThread( 
                     NULL,       // default security attributes
                     0,          // default stack size
					 (LPTHREAD_START_ROUTINE) reader, 
                     NULL,       // no thread function arguments
                     0,          // default creation flags
                     NULL);
	}
	while(WaitForMultipleObjects(NUM_OF_READERS, readers,TRUE,0));
	while(WaitForMultipleObjects(NUM_OF_WRITERS, writers,TRUE,0));
	//system("pause");
	DeleteCriticalSection(&readOpen);
	DeleteCriticalSection(&writeOpen);
	DeleteCriticalSection(&readTry);
	DeleteCriticalSection(&resource);
	system("pause");
	return 0;
}

