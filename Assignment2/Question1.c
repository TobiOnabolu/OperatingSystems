#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

void* my_thread(void *args);

void* my_thread(void *args) {
	for (int i = 0; i < 5; i++) {
		if (i == 4) {
			sleep(2);
		}
		printf("I am a Custom Thread Function Created By user.\n");
		sleep(1);
	}
	//success
	exit(0);
}

int main(int argc, char *argv[]) {
	pthread_t tid;
	//create a default
	pthread_attr_t attr;
	int status;

	// initialized with default attributes, creates new thread attr object then populates it with default init */
	status = pthread_attr_init(&attr);

	if (status != 0) {
		printf("error\n");
		return -1;
	}

	printf("This program would create threads\n");

	//create working thread
	status = pthread_create(&tid, &attr, my_thread, NULL);
	sleep(1);

	if (status == 0) {

		printf("Custom thread created Successfully\n");

		//call parent function
		for (int i = 0; i < 5; i++) {
			printf("I am the process thread created by compiler By default.\n");
			sleep(1);
		}
	} else {
		printf("error");
	}
	return 0;

}
