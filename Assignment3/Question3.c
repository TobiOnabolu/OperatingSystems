#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include<semaphore.h>
#define READY 1
#define INCOMING 0
#define DONE -1
#define EVEN 3
#define ODD 4

sem_t running;
sem_t even;
sem_t odd;

void logStart(char *tID); //function to log that a new thread is started
void logFinish(char *tID); //function to log that a thread has finished its time

void startClock(); //function to start program clock
long getCurrentTime(); //function to check current time since clock was started
time_t programClock; //the global timer/clock for the program

typedef struct thread //represents a single thread, you can add more members if required
{
	char tid[4]; //id of the thread as read from file
	unsigned int startTime;
	int state;
	pthread_t handle;
	int retVal;
	int bypass; //used for when only 1 parity remaining
	int type;
} Thread;

//you can add more functions here if required

int threadsLeft(Thread *threads, int threadCount);
int threadToStart(Thread *threads, int threadCount);
void* threadRun(void *t); //the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); //function to read the file content and build array of threads
int run_all(Thread *threads, int threadCount);
void* debugger(Thread *threads, int threadCount); //not needed was used for testing

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}

	//you can add some suitable code anywhere in main() if required
	sem_init(&running, 0, 1);
	sem_init(&even, 0, 1);
	sem_init(&odd, 0, 0); //lock the odd semaphore initially while even and running remain unlocked so even can go first

	Thread *threads = NULL;
	int threadCount = readFile(argv[1], &threads);
	//threads now populated with 6 thread each with start time
	//reference as (threads)[i].attr no star b4 thread cause not double ptr

	//start the time for the system
	startClock();

	//run while threads left in our list of threads
	while (threadsLeft(threads, threadCount) > 0) //put a suitable condition here to run your program
	{
		//you can add some suitable code anywhere in this loop if required
		int i = 0;

		//something here has to check for if there is only 1 parity of things currently running and no other thread left(inwhoel sys) then just run all these threads (by setting bypass = to true)
		if (run_all(threads, threadCount) == 1) {
			for (int k = 0; k < threadCount; k++) {
				threads[k].bypass = 1;
			}
		}

		//find a thread to be run from our list, if no thread AT THE MOMENT it returns -1 and break loop and wait for a thread to be run from our list if there is any
		while ((i = threadToStart(threads, threadCount)) > -1) {
			//you can add some suitable code anywhere in this loop if required
			threads[i].state = READY;
			//run the thread (might have to change last param
			threads[i].retVal = pthread_create(&(threads[i].handle), NULL,
					threadRun, &threads[i]);
		}

	}

	sem_destroy(&odd);
	sem_destroy(&even);
	sem_destroy(&running);

	return 0;
}

//used to identify if all the remaining threads have same parity AND there are no more threads incoming, if so run rest of threads
int run_all(Thread *threads, int threadCount) {
	int incoming_threads_flag = 0;
	int even_flag = 0;
	int odd_flag = 0;
	int same_parity_flag = 1; //if both even and odd flag= 1 then same parity flag will = 0

	for (int k = 0; k < threadCount; k++) {
		if (threads[k].state == INCOMING) {
			incoming_threads_flag = 1; //there exists at least 1 incoming thread so dont bypass any locks yet
		}
		if (threads[k].state == READY) {
			if (threads[k].type == EVEN) {
				even_flag = 1;
			} else {
				odd_flag = 1;
			}
		}

	}

	if (even_flag == 1 && odd_flag == 1) {
		same_parity_flag = 0;
	}

	if (same_parity_flag == 1 && incoming_threads_flag == 0)
		return 1;
	return 0;

}

int readFile(char *fileName, Thread **threads) //do not modify this method
{
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
		lines[i] = malloc(sizeof(command) * sizeof(char)); //Dynamically allocat space for each line command
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	//get each attribute from a line and put it into our struct
	for (int k = 0; k < threadCount; k++) {
		char *token = NULL; //init pointer of each line in file
		int j = 0;
		token = strtok(lines[k], ";");
		//looping thru the attributes of each line with j
		while (token != NULL) {
			(*threads)[k].state = INCOMING;
			(*threads)[k].bypass = 0;
			if (j == 0) {
				strcpy((*threads)[k].tid, token);
				int val = ((*threads)[k].tid[2]) - '0'; //this how to get integer version of char
				if (val % 2 == 0) {
					(*threads)[k].type = EVEN;
				} else {
					(*threads)[k].type = ODD;
				}

			}
			if (j == 1) {
				(*threads)[k].startTime = atoi(token);
			}
			j++;
			token = strtok(NULL, ";");
		}
	}
	return threadCount;
}

void logStart(char *tID) {
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char *tID) {
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

//returns how much thread need to be run by checking every threads state
int threadsLeft(Thread *threads, int threadCount) {
	int remainingThreads = 0;
	for (int k = 0; k < threadCount; k++) {
		if (threads[k].state > -1)
			remainingThreads++;
	}
	return remainingThreads;
}

//finds a thread thats ready to be run
int threadToStart(Thread *threads, int threadCount) {
	for (int k = 0; k < threadCount; k++) {
		if (threads[k].state == INCOMING
				&& threads[k].startTime == getCurrentTime())
			return k;
	}
	return -1;
}

void* debugger(Thread *threads, int threadCount) {
	for (int i = 0; i < threadCount; i++) {
		printf("Thread state: %d\n", threads[i].state);
		printf("Thread bypass: %d\n", threads[i].bypass);
		printf("Thread start time: %d\n", threads[i].startTime);
		printf("Thread tid: %s\n", threads[i].tid);
		printf("Thread type: %d\n\n", threads[i].type);

	}

	return NULL;
}

void* threadRun(void *t) //implement this function in a suitable way
{

//log start as soon as they come in
	logStart(((Thread*) t)->tid);

//your synchronization logic will appear here

//even sem and running sem should be init to open while odd should be init locked

//if even try to acquire even sem and then running sem
//if odd wait for odd sem and running sem

	if (((Thread*) t)->bypass == 0) { //we try to keep order of even and odd if bypass = 0
		if (((Thread*) t)->type == EVEN) {
			sem_wait(&even); //wait for even lock
		} else {
			sem_wait(&odd); //wait for odd lock
		}
	}

	sem_wait(&running);

//critical section starts here
	printf("[%ld] Thread %s is in its critical section\n", getCurrentTime(),
			((Thread*) t)->tid);
//critical section ends here

//your synchronization logic will appear here

	logFinish(((Thread*) t)->tid);
//make this thread process finished
	((Thread*) t)->state = -1;

	if (((Thread*) t)->bypass == 0) { //we try to keep order of even and odd if bypass = 0
		if (((Thread*) t)->type == EVEN) {
			sem_post(&odd); //keep the even lock locked and unlock the odd lock
		} else {
			sem_post(&even); //keep the odd lock locked and unlock the even lock
		}
	} else { //this thread will open up all the other thread after bypass is set
		sem_post(&odd);
		sem_post(&even);
	}

	sem_post(&running); //unlock the running lock

//once this thread done if it is even lock the even semaphore(to signify even cant go next) and release the odd semaphore and running semaphore

//this would work but when theres no more odd or and just 1 parity there will be endless wait by one thread
//END CRITICAL SECTION HERE
	pthread_exit(0);
}

void startClock() {
	programClock = time(NULL);
}

long getCurrentTime() //invoke this method whenever you want check how much time units passed
//since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}
