#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

// Global Data variables.
int a = 5, b = 7;
sem_t mutex; //have to declare the semaphore in global section of threads
// Function that access the global data.
void* inc_dec(void *arg) {

	//all this is critical section
	//create lock where init set to 0 and then thread has to collect before executing all this code

	//enter crit section
	sem_wait(&mutex);
	printf("Read value of 'a' global variable is: %d\n", a);
	printf("Read value of 'b' global variable is: %d\n", b);
	sleep(1);	// have to sleep before modifying global vars
	a = a + 1;
	b = b - 1;
	printf("Updated value of 'a' variable is: %d\n", a);
	printf("Updated value of 'b' variable is: %d\n", b);

	//leave crit section
	sem_post(&mutex);
	return NULL;
}

int main() {
//init semaphore
	sem_init(&mutex, 0, 1);
//1 for binary sem, 0 for shared btwn thread

// Creating the thread instances.
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, inc_dec, NULL);
	pthread_create(&t2, NULL, inc_dec, NULL);
	pthread_create(&t3, NULL, inc_dec, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	sem_destroy(&mutex);
//Destroying the threads and mutex.
	/*
	 pthread_exit(t1);
	 pthread_exit(t2);
	 pthread_exit(t3);
	 */
	return 0;
}
