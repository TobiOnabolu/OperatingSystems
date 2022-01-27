#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

	//Create zombie process
	system("./z_creator &");
	sleep(2);

	FILE *fp;
	char ppid[10];
	char result[20];

	//list all processes running
	system("ps -l");

	//store ouput from command in file pointer
	//make sure there is space in between ''
	fp = popen("ps -l|grep -w Z|tr -s ' '|cut -d ' ' -f 5", "r");

	//error handling
	if (fp == NULL) {
		printf("Didnt recieve output from command");
		exit(1);
	}

	//get ppid for zombie from commnad
	fgets(ppid, sizeof(ppid), fp);
	pclose(fp);

	//format the string to be passed into system call, 1st arg is resulting string
	sprintf(result, "kill -9 %s", ppid);

	printf("\n---------------Updated Processes--------------------\n");

	//kill parent process by id thats been given by PPID
	system(result);

	//list all processes again to ensure z process is no longer there
	system("ps -l");

	return 0;
}
