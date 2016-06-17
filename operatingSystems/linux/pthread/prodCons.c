#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#define NUM_OF_PROD 2
#define NUM_OF_CONS 2
#define NUM_OF_IT 30
#define PERIOD 50
#define MAX_SIZE 5
pthread_mutex_t prod_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cons_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t size_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t console = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t lcons_mutex = PTHREAD_MUTEX_INITIALIZER;
// for signals
pthread_mutex_t last = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prod_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cons_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t head_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t tail_ready = PTHREAD_COND_INITIALIZER;
//int cust_active = 0;
//int prod_active = 0;
int info = 0;
int p_num = 0;
int c_num = 0;
int size = 0;

int prod_it = 0;
int cons_it = 0;

bool pr_wait = false;
bool co_wait = false;


typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef struct Queue {
	Node* head;
	Node* tail;
	//int size;
	int maxSize;
} Queue;

//add to quen
void push(Queue* q, int data) {
	//if (q->size > 10) return;
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
   // q->size++;
    //printf("push tmp%ld h%ld t%ld\n",(long)tmp,(long)q->head,(long)q->tail);
    if(!(q->head)){
    	q->head = tmp;
    	// q->head->next = NULL;
    	//printf("push new -h%ld -t%ld\n",(long)q->head,(long)q->tail);
    	//q->size++;
    	//return;
    } else {
	    q->tail->next = tmp;
	}
	    q->tail = tmp;
	    q->tail->next = NULL;
	    //printf("push DONE -h%ld -t%ld\n",(long)q->head,(long)q->tail);
}

//take from quen
int pop(Queue* q) {
	if(!(q->head)) return -1;
    Node* prev = NULL;
  //  q->size--;
    int val = q->head->value;
    //prev = q->head;
    //printf("pop h%ld\n",(long)q->head);
    //printf("free h%ld\n",(long)q->head);
    
    prev = q->head;
    q->head = q->head->next;
    free(prev);
   
    if(!(q->head)) q->tail = NULL;
    //printf("h%ld t%ld free done \n",(long)q->head,(long)q->tail);
    
    return val;
}

void *producer(void* queue) {
	srand(time(NULL));
	int id = p_num++;
	while(prod_it++ < NUM_OF_IT){
		
		pthread_mutex_lock(&prod_mutex);
			//проверка на заполнение очереди
			pthread_mutex_lock(&prod_wait);
				if (size == ((Queue*)queue)->maxSize){
					pr_wait = true;
					//printf("prod wait\n");
						printf("producer %d : queue full\n", id);
					//чекаемо поки не звильниться место в очереди 5 елем щоб не добавили 6 елемент
					pthread_cond_wait(&tail_ready,&prod_wait);
						printf("producer %d : queue ready\n", id);
					//printf("prod run\n");
					pr_wait = false;
				}
			pthread_mutex_unlock(&prod_wait);
		//блокуемо ласт 1 проблема
		pthread_mutex_lock(&last);

			printf("producer %d : add %d\n", id,info);
		push((Queue*)queue,info++);
		//printf("prod locks size\n");
		pthread_mutex_lock(&size_mutex);
			//printf("prod locks cons_wait\n");
			pthread_mutex_lock(&cons_wait);
				++size;
				if (co_wait)
					pthread_cond_signal(&head_ready);
			pthread_mutex_unlock(&cons_wait);
		pthread_mutex_unlock(&size_mutex);
		pthread_mutex_unlock(&last);

		pthread_mutex_unlock(&prod_mutex);
		usleep(rand()%PERIOD);
	}

	pthread_exit(NULL);
}

void* consumer(void* queue) {
	srand(time(NULL));
	bool isLast;
	int id = c_num++;

	while(cons_it < NUM_OF_IT || size > 0){
		
		isLast = false;
		//printf("cons locks cons_mutex\n");
		//проблема 3
		//заблокували консумерив
		pthread_mutex_lock(&cons_mutex);
		//printf("%d\n",id);
		if (cons_it >= NUM_OF_IT && size == 0) {
			pthread_exit(NULL);
		}
		++cons_it;
		//printf("cons locks cons_wait\n");
		//проверка пустая ли очередь
			pthread_mutex_lock(&cons_wait);
				if (size == 0) {
					co_wait = true;
					//printf("cons wait\n");
					//pthread_mutex_lock(&console);
						printf("consumer %d : queue empty \n", id);
						//если пустая то ждем
					pthread_cond_wait(&head_ready,&cons_wait);
						printf("consumer %d : queue ready \n", id); 
					//printf("cons run\n");
					co_wait = false;
				}
			pthread_mutex_unlock(&cons_wait);

		//якщо черага пуста то блокуемо последний
		//если продюсер что то делал с последним елементом то мы ждем
		if(((Queue*)queue)->head == ((Queue*)queue)->tail){
			//printf("cons lock prod_mutex\n");
			//ждем
			pthread_mutex_lock(&last);
			if(((Queue*)queue)->head != ((Queue*)queue)->tail  ){
				pthread_mutex_unlock(&last);
				//printf("unblock h%ld\n",(long)(((Queue*)queue)->head));
			}
			else {
					printf("consumer %d : last\n", id);
				isLast = true;
			}
		}
			printf("consumer %d : pop %d\n", id,pop((Queue*)queue));
		//забрали из очереди 
		 if(isLast){
		 		printf("consumer %d : last unlocked\n", id);
			pthread_mutex_unlock(&last);
		}
		
		//меняемо розмер очереди
		pthread_mutex_lock(&size_mutex);
		//	printf("cons lock prod_wait\n");
			pthread_mutex_lock(&prod_wait);
				--size;
				//если продюсер ждет то отправляемо ему сигнал
				if (pr_wait)
					pthread_cond_signal(&tail_ready);
			pthread_mutex_unlock(&prod_wait);
		pthread_mutex_unlock(&size_mutex);

		pthread_mutex_unlock(&cons_mutex);
		usleep(rand()%PERIOD);
	}
	pthread_exit(NULL);
}
int main()
{
	pthread_t prods[NUM_OF_PROD];
	pthread_t cons[NUM_OF_CONS];
	Queue* q = (Queue*) malloc(sizeof(Queue));
	//q->size = 0;
	q->maxSize = MAX_SIZE;
	char ch;
	srand(time(NULL));


	
	for(int i = 0; i < NUM_OF_CONS; ++i){
		pthread_create(&cons[i],NULL,consumer,(void*)q);}
	

	for(int i = 0; i < NUM_OF_PROD; ++i){
		// pthread_create(&cons[i],NULL,consumer,(void*)q);
		pthread_create(&prods[i],NULL,producer,(void*)q);
		
	}
	for(int i = 0; i < NUM_OF_CONS; ++i){
		pthread_join(cons[i],NULL);
	}
	for(int i = 0; i < NUM_OF_PROD; ++i){
		pthread_join(prods[i],NULL);
	}
	return 0;
}

