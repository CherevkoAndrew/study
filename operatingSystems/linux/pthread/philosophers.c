#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#define MAX_TIME 300
#define MIN_TIME 0
#define NUM_PH	5
#define NUM_ITER 20

bool empty = true;

typedef struct Phil {
	pthread_mutex_t* left_fork;
	pthread_mutex_t* right_fork;
	pthread_t* thread;
	Phil* leftPh;
	Phil* rightPh;
	int id;
	bool is_first;
} Phil;

void eat(Phil* ph, int time){
	if (ph->is_first) {
		// перший
		//printf("Pilosopher %d: wait right fork\n", ph->id);
		pthread_mutex_lock(ph->right_fork);
		printf("Pilosopher %d: right locked\n", ph->id);
		pthread_mutex_lock(ph->left_fork);
		printf("Pilosopher %d: left locked\n", ph->id);

	} else {
		// другий
		//printf("Pilosopher %d: wait left fork\n", ph->id);
		pthread_mutex_lock(ph->left_fork);
		printf("Pilosopher %d: left locked\n", ph->id);
		pthread_mutex_lock(ph->right_fork);
		printf("Pilosopher %d: right locked\n", ph->id);
	}
	//printf("Pilosopher %d :  eating\n", ph->id);
	usleep(time);
	//printf("Pilosopher %d : thinking\n", ph->id);
	// поили-поклали
	printf("Pilosopher %d: right unlocked\n", ph->id);
	pthread_mutex_unlock(ph->right_fork);
	printf("Pilosopher %d: left unlocked\n", ph->id);
	pthread_mutex_unlock(ph->left_fork);
	
}

void *phLife(void *ph)
{
	srand(time(NULL));
	for (int i = 0; i < NUM_ITER; ++i)
	{
		usleep(rand()%(MAX_TIME + MIN_TIME) + MIN_TIME);
		eat((Phil*)ph,rand()%(MAX_TIME + MIN_TIME) + MIN_TIME);
	}
	printf("Pilosopher %d dead\n", ((Phil*)ph)->id);
   pthread_exit(NULL);

}

//  создаються философы роздаються вилки
Phil* init(int N) {
	// 1 философ
	Phil* start = (Phil*) malloc(sizeof(Phil));
	start->is_first = true;
	start->left_fork = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	// инициализуем
	pthread_mutex_init(start->left_fork,NULL);
	// выд память под поток
	start->thread = (pthread_t*) malloc(sizeof(pthread_t));
	start->id = 0;
	Phil* tmp = start;
	for (int i = 0; i < N-1; ++i)
	{
		// на пред. цепляэм нового
		// на нового пред
		tmp->rightPh = (Phil*) malloc(sizeof(Phil));
		tmp->rightPh->is_first = false;
		tmp->rightPh->leftPh = tmp;
		tmp = tmp->rightPh;
		// роздаэмо вилки для 1 вона буде левой для 2 правовой вона одна
		tmp->left_fork = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
		tmp->leftPh->right_fork = tmp->left_fork;
		tmp->id = tmp->leftPh->id + 1;
		tmp->thread = (pthread_t*) malloc(sizeof(pthread_t));
		pthread_mutex_init(tmp->left_fork,NULL);
	}
	tmp->rightPh = start;
	tmp->right_fork = start->left_fork;
	return start;
}

int main(int argc, char *argv[])
{
   Phil* p = init(NUM_PH);
   Phil* ps = p;
   do{
   	//printf("In main: ph %d\n", p->id);
   	//printf(" %ld\n", (long)p->left_fork);
   	//printf(" %ld\n", (long)p->right_fork);
   	//pthread_create(p->thread, NULL, phLife, (void *)p);
   	p = p->rightPh;
   } while (p->id);
   do{
   
   	pthread_create(p->thread, NULL, phLife, p);
   	p = p->rightPh;
   } while (p->id);
   do{
   
   	pthread_join(*(p->thread),NULL);
   	p = p->rightPh;
   } while (p->id);
}