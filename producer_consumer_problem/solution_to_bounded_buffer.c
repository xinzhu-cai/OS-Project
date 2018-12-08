#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define RAND 100000000
/* The buffer */
buffer_item buffer[BUFFER_SIZE];
/* The mutex lock */
pthread_mutex_t mutex;
/* The semaphores */
sem_t empty,full;
/* buffer counter */
int count;
/* pthread */
pthread_t tid; // thread ID
pthread_attr_t attr; // Set of thread attributes

/*Acquire the mutex lock*/
//pthread_mutex_lock(&mutex);
/*Release the mutex lock*/
// pthread_mutex_unlock(&mutex);
void initialization()
{
	/* Create the mutex lock */
	pthread_mutex_init(&mutex,NULL);
	/* Create the semaphores */
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);
	/* Get the default attributes*/
	pthread_attr_init(&attr);
	count = 0;
}

/*Add item to the buffer*/
int insert_item(buffer_item item)
{
	/*insert item into buffer
     returns 0 if successful
     otherwise returns -1 indicating an error condition
	*/
	if(count < BUFFER_SIZE)
	{
		buffer[count] = item;
		count++;
		return 0;
	}
	else
	{
		return -1;
	}
}

/*Remove item from the buffer*/
int remove_item(buffer_item *item)
{
	/*removes an object from buffer and placing it in item
     returns 0 if successful
     otherwise return -1 indicating an error condition
	*/
	if(count > 0)
	{
		*item = buffer[count-1];
		count--;
		return 0;
	}
	else
	{
		// If buffer has no element return -1
		return -1;
	}
}

/* Producer thread */
void *producer(void *param)
{
	buffer_item item;
	int rand_sleep;
	while(1){
        /*Sleep for a random period of time*/
		rand_sleep = rand()/RAND;
		sleep(rand_sleep);
		/*Generate a random number*/
		item = rand();
		/*Accquire the empty lock*/
		sem_wait(&empty);
		/*Acquire the mutex lock*/
		pthread_mutex_lock(&mutex);
        printf("producer produced %d \n",item);
        if(insert_item(item))
        	fprintf(stderr,"report error condition\n");
        /*Release the mutex lock*/
		pthread_mutex_unlock(&mutex);
		/*Signal the full lock */
		sem_post(&full);
	}
}

/* Consumer thread */
void *consumer(void *param)
{
	buffer_item item;
	int rand_sleep;
	while(1)
	{
		/*sleep for a random period of time*/
		rand_sleep = rand()/RAND;
		sleep(rand_sleep);
		/*Acquire the full lock*/
		sem_wait(&full);
		/*Acquire the mutex lock*/
		pthread_mutex_lock(&mutex);
		/*Remove a number in buffer*/
        if(remove_item(&item))
        	fprintf(stderr,"report error condition\n");
        else
        	printf("cosumer comsumed %d \n", item);
        /*Release the mutex lock*/
		pthread_mutex_unlock(&mutex);
		/*Signal the emtpy lock*/
		sem_post(&empty);
	}
}

int main(int argc, char *argv[])
{
	initialization();
	if(argc != 4)
		fprintf(stderr, "parameters error!\n");
	/* Get command line arguments */
	//sleep_time denotes How long to sleep before termination
	int producer_num, consumer_num, sleep_time, i; // number of producers and consumers
	sleep_time = atoi(argv[1]);
	producer_num = atoi(argv[2]);
	consumer_num = atoi(argv[3]);
	/* Create producer threads */
	for(i=0;i<producer_num;++i)
	{
		pthread_create(&tid,&attr,producer,NULL);
	}
	/* Create consumer threads */
	for(i=0;i<consumer_num;++i)
	{
		pthread_create(&tid,&attr,consumer,NULL);
	}
	/* Sleep */
	sleep(sleep_time);
	/* Exit */
	printf("Exit\n");
	exit(0);
}