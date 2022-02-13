#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>

void logStart(char *tID); //function to log that a new thread is started
void logFinish(char *tID); //function to log that a thread has finished its time

void startClock(); //function to start program clock
long getCurrentTime(); //function to check current time since clock was started
time_t programClock; //the global timer/clock for the program
#define FALSE 0
#define TRUE 1

typedef struct thread //represents a single thread
{
	char tid[4]; //id of the thread as read from file
	int start_time;
	int life_time;
	int processed; //used to ensure we dont process a thread twice

} Thread;

void* threadRun(void *t); //the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); //function to read the file content and build array of threads
int get_total_time(Thread **threads, int thread_count);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}

	//you can add some suitable code here as per problem sepcification

	//Read threads attributes from files into Thread** pointer (dont create threads yet

	//get filename and threads objects and send to readfile
	char *fileName = argv[1];
	//Create your list of threads
	Thread *threads;
	//Send the address of threads since readfile is expecting double pointer
	int thread_count = readFile(fileName, &threads);

	//get total time for program
	int total_time = get_total_time(&threads, thread_count);

	//for safe measures
	total_time = total_time + 1;

	//Start clock
	startClock();

	while (total_time > getCurrentTime()) //put a suitable condition here to run your program
	{
		//write suitable code here to run the threads
		//Check if our clock is at any of the start times

		for (int i = 0; i < thread_count; i++) {
			if ((getCurrentTime() >= threads[i].start_time)
					&& (threads[i].processed == FALSE)) {	//start this thread
				threads[i].processed = TRUE;
				//create thread
				pthread_t thread;
				pthread_create(&thread, NULL, threadRun, (void*) &(threads[i])); //type cast the address of the thread to expect a void instead of a thread. In thread run will typecast it back
				//do not invoke join because we wont be able to call other threads to start at a certain time since join is a blocking function

			}
		}

		//for safe meaures
		if (getCurrentTime() > 45) {
			printf("\nWhile loop failed");
			break;
		}

	}

	return 0;
}

//function to calcualte the amount of time needed for whole program
//did it this way so i know when to end program, since decrementing a shared variable between 4 threads is difficult
int get_total_time(Thread **threads, int thread_count) {
	int total_time = 0;

//find the max time for thread
	for (int i = 0; i < thread_count; i++) {
		int time_for_thread = (*threads)[i].life_time
				+ (*threads)[i].start_time;
		total_time =
				time_for_thread > total_time ? time_for_thread : total_time;
	}

	return total_time;
}

//double pointer for readfile indicates we want to save the state of our pointers
int readFile(char *fileName, Thread **threads) //use this method in a suitable way to read file
{
//opens file
	FILE *in = fopen(fileName, "r");
	if (!in) {
		printf(
				"Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

//assign file contents to dynamic array
	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char*) malloc(((int) st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in)) {
		char line[100];
		if (fgets(line, 100, in) != NULL) {
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

//Creating file copy of contents to tokenize since tokenizing ruins string
	char *command = NULL;
	int threadCount = 0;
	char *fileCopy = (char*) malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);

//ignore command, this is used to retreive the number of threads we need
	command = strtok(fileCopy, "\r\n");
	while (command != NULL) {
		threadCount++;
		command = strtok(NULL, "\r\n");
	}

//create a dynamic array of threads of size (threadcount). our Thread** pointer will point to this
	*threads = (Thread*) malloc(sizeof(Thread) * threadCount);

//getting the lines from our file contents
	char *lines[threadCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL) {
		lines[i] = malloc(sizeof(command) * sizeof(char)); //dynamically allocate space for each command/line
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

//get each attribute from a line and put it into the threads
	for (int k = 0; k < threadCount; k++) { //looping thru each line
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ";");
		while (token != NULL) {
			//this loop tokenizes each line of input file
			//write your code here to populate instances of Thread to build a collection
			if (j == 0) { //thread id
				strcpy((*threads)[k].tid, token);
			} else if (j == 1) { //start time
				(*threads)[k].start_time = atoi(token);
			} else { //lifetime
				(*threads)[k].life_time = atoi(token);
			}

			j++;
			token = strtok(NULL, ";");
		}
		(*threads)[k].processed = FALSE; //initialize to false for all threads

	}

	return threadCount;
}

void logStart(char *tID) //invoke this method when you start a thread
{
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char *tID) //invoke this method when a thread is over
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

void* threadRun(void *t) //implement this function in a suitable way
{
	//typecast thread back into thread pointer
	Thread *thread = (Thread*) t;

	//call log start
	logStart(thread->tid);

	int time_for_thread = thread->start_time + thread->life_time;

	while (time_for_thread > getCurrentTime()) {
		//do nothing
	}

	//once exit loop call log finish
	logFinish(thread->tid);

	return NULL;
}

void startClock() //invoke this method when you start servicing threads
{
	programClock = time(NULL);
}

long getCurrentTime() //invoke this method whenever you want to check how much time units passed
//since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}
