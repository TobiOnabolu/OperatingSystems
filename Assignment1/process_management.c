#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
void writeOutput(char *command, char *output);
void write_to_pipe(char *commands, int p[]);
void read_from_pipe(int p[]);
char* read_shm();
void get_status();
void create_shm();
#define READ_END	0
#define WRITE_END	1
const int SIZE = 4096;

//wait for child to finish executing NOTE This might be a problem cause we not calling execvp from child
void get_status() {
	int status;
	if (wait(&status) >= 0) {
		if (WIFEXITED(status)) {
			/* Child process exited normally, through `return` or `exit` */
			printf("Child process exited with %d status\n",
					WEXITSTATUS(status));

		}
	}
}

//create the shm and reads the words from smaple txt into it
void create_shm() {
	//create shared memory model
	int fd;
	void *ptr;

	fd = shm_open("/shm", O_CREAT | O_RDWR, 0666);

	//Attempt to create size of region if it fails out put failed
	if (ftruncate(fd, SIZE) == -1) {
		printf("Not able make space for region");
		exit(1);
	}

	//create a place in memeory for our shm
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed from child\n");
		exit(1);
	}

	//open file pointer to read from
	FILE *fp;
	fp = fopen("./sample_in.txt", "r");
	char line[25];

	//read our input file line by line
	while (fgets(line, sizeof(line), fp) != NULL) {
		//add each line to shm
		sprintf(ptr, "%s", line);
		ptr += strlen(line);
	}
}

//read from the shm and store it in a dynamic array
char* read_shm() {
	//Create our d_array
	char *words;
	words = (char*) malloc(500 * sizeof(char));
	if (words == NULL) {
		printf("Was not able to create array");
		exit(1);
	}

	//find our shared memory model
	int fd;
	void *ptr;
	const int SIZE = 4096;
	fd = shm_open("/shm", O_RDONLY, 0666);

	//create a place in memeory for our shm
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(1);
	}

	//assign words from our shared memory model to our dynamic array
	//increment ptr by 1 to access each character
	words = (char*) ptr;

	//unlink from shm from the parent process
	if (shm_unlink("/shm") == -1) {
		printf("Error removing");
		exit(1);
	}

	return words;

}

//write output of our commands to text file
void writeOuput(char *command, char *output) {
	/* Complete code to save the commands in a output.txt*/
	FILE *fp;
	fp = fopen("output.txt", "a");

	if (fp == NULL) {
		printf("Error!");
		exit(1);
	}
	fprintf(fp, "The output of: %s : is\n", command);
	fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);

	fclose(fp);
}

//read command + output from pipe sends it to output function
void read_from_pipe(int p[]) {
	close(p[WRITE_END]);

	//read message from pipe, message will contain the command and output seperated by a new line character
	char message[500];
	read(p[READ_END], message, strlen(message));

	//close pipe
	close(p[READ_END]);

	/*
	 //split the message by \n
	 char *command = strtok(message, '\n');

	 //passing in null paramet tokenizes the last string strtok called
	 char *output = strtok(message, '\n');

	 */

	//writeOutput("free -l", message); //check how to pass paramsh
}

//executes 1 command and writes it to the pipe p
void write_to_pipe(char *command, int p[]) {
	//Close unused end of pipe
	close(p[READ_END]);

	//split the word by space into the array arg list
	char *word;

	char *argument_list[25];	//needed for execv

	int k = 0;
	//split by space
	word = strtok(command, " ");

	//Populate argument list
	while (word != NULL) {
		*(argument_list[k]) = *word;
		word = strtok(NULL, " ");
		k++;
	}

	argument_list[k] = NULL; //set the last spot to null, needed for execvp

	//Make whatever we output from execvp call go to write end of pipe
	//dup2(p[WRITE_END], STDOUT_FILENO);

	//(p[WRITE_END], STDOUT_FILENO);
	//execute command
	if (execvp(argument_list[0], argument_list) == -1) {
		printf("Error executing command with execvp");
	}

	//closed write end to pipe not needed
	close(p[WRITE_END]);
}

int main() {
	//-------------------------------------------------Starting Shared memory part of Assignment-------------------------------------------------
	//Fork child process
	pid_t pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	} else if (pid == 0) { /* child process */
		//create the shm from child, read sample txt into it
		create_shm();
		exit(0);
	}

	// parent process Resumes
	//terminate child process
	get_status();

	//read shm into dynamic array
	char *commands;
	commands = read_shm();

	//-------------------------------------------------Starting Pipe part of Assignment-------------------------------------------------

	//Create the pipe from the 2 ele array
	int p[2];
	if (pipe(p) == -1) {
		fprintf(stderr, "Pipe failed");
		exit(1);
	}

	//fork new child to read from darray
	pid_t pid2 = fork();

	if (pid2 < 0) { // error occurred
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	} else if (pid2 == 0) { // child process
		printf("\nThis is the child");
		//Strip a command from our dynamic array of commands and send it through the pipe
		char *command;
		command = strtok(commands, "\n");
		printf("\nThis is the command: %p", command);
		//write_to_pipe(command, p);

		exit(0);

	}
	printf("\nThis is the parent");
	//parent process Resumes
	//No wait call needed due to reading pipe being a blocking function
	//read_from_pipe(p);

	return 0;

}

//TODO fork new child process to execute a single command from the array using execvp

//TODO child process should also take the output from the command and pass it to a pipe

//TODO parent process should read the output from a pipe and write a file output.txt(function to do this is given to us)
//printf("\nThis is the address of our array: %d", getpid());
//this one way to print string
//printf("\n%s\n", (char*) (commands));
//command = (char*) malloc(100 * sizeof(char));
//printf("\nThis is the child");

/*
 //set index pointer so we wont lose track of where word starts
 char *i = commands;
 char *j = command;
 while (*(i) != '\n') {
 //get command
 printf("\nThis is the letter: %c", *i);
 *(j) = *(i);
 i++;
 j++;
 }

 i++; //skip past \n character to get next command
 */
//printf("\nThis is the command: %p", commands);
//printf("\nThis is the command: %s", (char*) commands);
