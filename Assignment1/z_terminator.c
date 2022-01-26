#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	printf("testing");

	//Create zombie process
	system("./z_creator &");

	//list all processes running
	system("ps -l");

	//get zombie process id then get its PPID
	system("ps -l| grep -w Z|tr -s''|cut -d''-f5");

	//kill parent process by id thats been given by PPID

	//list all processes again to ensure z process is no longer there

	return 0;
}
