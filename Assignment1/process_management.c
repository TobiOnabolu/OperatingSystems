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

int main() {
	//TODO Fork child process to read from sample_in.txt
	pid_t pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	} else if (pid == 0) { /* child process */
		//create shared memory model
		const int SIZE = 4096;
		fd = shm_open("/shm", O_CREAT | O_RDWR, 0666);
		if (fd == -1) { //Error happened
			printf("Shared Memory not opened properly!!!");
			return 1;
		}

		//Attempt to create size of region if it fails out put failed
		if (ftruncate(fd, SIZE) == -1) {
			printf("Not able make space for region");
			return 1;
		}

		//create a place in memeory for our shm
		ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			return 1;
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

		return 0;

	} else { /* parent process */
		//wait for child to finish executing NOTE This might be a problem cause we not calling execvp from child
		wait(NULL);
		printf("\n Child Has Read Data to shm \n");

		//open shared memory model
		const int SIZE = 4096;
		fd = shm_open("/shm", O_RDONLY, 0666);
		if (fd == -1) { //Error happened
			printf("Shared Memory not opened properly by parent processs!!!");
			return 1;
		}

		//create a place in memeory for our shm
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			return 1;
		}

		//TODO read ptr into dynamic array
	}

	return 0;

	//TODO place data read in shm using share memory model producer code. Done with child process not parent

	//TODO once its in shm, Parent (consumer) should read from the shared memory model into dynamic arra

	//TODO Parent should terminate child process once parent has finished reading from shm

	//TODO fork new child process to execute a single command from the array using execvp

	//TODO child process should also take the output from the command and pass it to a pipe

	//TODO parent process should read the output from a pipe and write a file output.txt(function to do this is given to us)

	return 0;
}

