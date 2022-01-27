#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

	int lifespan = 10;
	int pid = fork();
	//process is a parent
	if (pid > 0) {
		//Sleep for (lifespan) seconds to allow zombie process to stay alive
		sleep(lifespan);
	}

	//process is a child
	else if (pid == 0) {
		//terminate child process to turn it to zombie
		exit(1);
	}

	//error occured during fork
	else {
		fprintf(stderr, "Failed to Fork");
		return 1;
	}

	return 0;
}
