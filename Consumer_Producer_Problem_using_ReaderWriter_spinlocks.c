#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define SIZE 5
#define PROCESS 7


int buffer[SIZE], prod = 0, cons = 0;
pthread_spinlock_t lock;
void* producer(void* param);
void* consumer(void* param);
void insert_item (int item);
void remove_item (void);


int main() {
 int i, value[PROCESS];
 pthread_t prod_id[PROCESS], cons_id[PROCESS];
 pthread_attr_t attr;
 pthread_attr_init(&attr);
 pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED);
 for (i=1; i<=PROCESS; i++) {
 value[i-1] = i;
 pthread_create(&prod_id[i-1], &attr, producer, &value[i-1]);
 pthread_create(&cons_id[i-1], &attr, consumer, &value[i-1]);
 }
 for (i=1; i<=PROCESS; i++) {
 pthread_join(prod_id[i-1], NULL);
 pthread_join(cons_id[i-1], NULL);
 }
 pthread_spin_destroy(&lock);
 return 0;
}

void* producer (void *param) {
 int item = *(int*)param;
 while (cons == (prod + 1)%SIZE);
 pthread_spin_lock(&lock);
 insert_item(item);
 pthread_spin_unlock(&lock);
 pthread_exit(0);
}

void* consumer (void *param) {
 while (prod == cons);
 pthread_spin_lock(&lock);
 remove_item();
 pthread_spin_unlock(&lock);
 pthread_exit(0);
}

void insert_item (int item) {
 buffer[prod] = item;
 prod = (prod + 1)%SIZE;
 printf("Produced an item : %d\n", item);
}

void remove_item (void) {
 int consumed;
 consumed = buffer[cons];
 cons = (cons + 1)%SIZE;
 printf("Consumed an item : %d\n", consumed);
}