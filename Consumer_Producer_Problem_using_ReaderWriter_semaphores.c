#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MaxSize 3
#define PROCESS 5


int buffer[MaxSize], cons = 0, prod = 0;
pthread_mutex_t semaphor;
sem_t full, empty;

void* producer(void* param);
void* consumer(void* param);
void insert_item(int item);
void remove_item(void);


int main(void) {
	int i, value[PROCESS];
	pthread_mutex_init(&semaphor, NULL);
	sem_init(&full, 0, MaxSize-1);
	sem_init(&empty, 0, 0);
	pthread_t prod_id[5], cons_id[5];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for (i=1; i<=PROCESS; i++) {
		value[i] = i;
		pthread_create(&prod_id[i], &attr, producer, &value[i]);
		pthread_create(&cons_id[i], &attr, consumer, &value[i]);
	}
	for (i=1; i<=PROCESS; i++) {
		pthread_join(prod_id[i], NULL);
		pthread_join(cons_id[i], NULL);
	}
	pthread_mutex_destroy(&semaphor);
	sem_destroy(&full);
	sem_destroy(&empty);
	return 0;
}

void* producer (void *param) {
	int p = *(int*)param;
	if (cons == (prod + 1)%MaxSize)
	printf("BUFFER FULL, Producer %d waiting\n", p);
	sem_wait(&full);
	pthread_mutex_lock(&semaphor);
	insert_item(p*10);
	pthread_mutex_unlock(&semaphor);
	sem_post(&empty);
	pthread_exit(0);
}

void* consumer (void *param) {
	int *c = (int*)param;
	if (cons == prod)
	printf("BUFFER EMPTY, Consumer waiting!\n");
	sem_wait(&empty);
	pthread_mutex_lock(&semaphor);
	remove_item();
	pthread_mutex_unlock(&semaphor);
	sem_post(&full);
	pthread_exit(0);
}

void insert_item(int item) {
	buffer[prod] = item;
	prod = (prod + 1)%MaxSize;
	printf("Produced an item : %d\n", item);
}

void remove_item(void) {
	int consumed;
	consumed = buffer[cons];
	cons = (cons + 1)%MaxSize;
	printf("Consumed an item : %d\n", consumed);
}
