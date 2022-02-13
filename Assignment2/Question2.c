#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <limits.h>

//store 4 values globally
float avg;
int max;
int min;
float median;
#define BUFFER 100

//Question can I define the array globally
int numbers[BUFFER];
int SIZE;

void* get_average(void *param);
void* get_max(void *param);
void* get_min(void *param);
void* get_median(void *param);
int comparator(const void *p, const void *q);

int main(int argc, char *argv[]) {

	SIZE = argc - 1;
	//int numbers[size];

	for (int i = 1; i < argc; i++) {
		numbers[i - 1] = atoi(argv[i]);
	}

	qsort(numbers, SIZE, sizeof(int), comparator);

	// create 4 threads
	pthread_t avg_thread;
	pthread_t max_thread;
	pthread_t min_thread;
	pthread_t median_thread;

	// use pthread create to assign the 4 threads to the functions
	//first arg = address of thread, 2nd arg = attributes of thread if null then defaul attr
	//3rd arg function thread should execute, 4th arg = paramater that need to be passed to that function
	pthread_create(&avg_thread, NULL, get_average, NULL);
	pthread_create(&max_thread, NULL, get_max, NULL);
	pthread_create(&min_thread, NULL, get_min, NULL);
	pthread_create(&median_thread, NULL, get_median, NULL);

	// join all threads back to main thread
	//join acts like wait for proceesses, block this calling thread until called thread is finished termination
	pthread_join(avg_thread, NULL);
	pthread_join(max_thread, NULL);
	pthread_join(min_thread, NULL);
	pthread_join(median_thread, NULL);

	//output values in main function
	printf("The average value calculated by first thread one is %.2f\n", avg);
	printf("The max value calculated by the second thread is %d\n", max);
	printf("The min value calculated by the third thread is %d\n", min);

	printf("The median value calculated by the fourth thread is %.2f\n",
			median);

	return 0;

}

int comparator(const void *p, const void *q) {
	int l = *((int*) p); //type cast void pointer to int pointer then dereference it
	int r = *((int*) q);
	return (l - r);
}

// Create 4 function to calculate values
//Question: Do i need this param
void* get_average(void *param) {
	int sum = 0;
	for (int i = 0; i < SIZE; i++) {
		sum += numbers[i];
	}

	avg = (float) sum / SIZE;
	return NULL;
}

void* get_max(void *param) {
	//initialize to lowest number
	max = INT_MIN;
	for (int i = 0; i < SIZE; i++) {
		max = numbers[i] > max ? numbers[i] : max;
	}

	return NULL;
}

void* get_min(void *param) {
	//initialize to highest number
	min = INT_MAX;
	for (int i = 0; i < SIZE; i++) {
		min = numbers[i] < min ? numbers[i] : min;
	}

	return NULL;
}

void* get_median(void *param) {

	int midpoint = (SIZE) / 2;

	//if array is even
	if (SIZE % 2 == 0) {
		median = (float) (numbers[(SIZE) / 2] + numbers[(SIZE - 1) / 2]) / 2;
	} else {	//if array is odd
		median = numbers[midpoint];
	}

	return NULL;
}

