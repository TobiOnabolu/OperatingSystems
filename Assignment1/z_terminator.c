#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    //Create zombie process
    system("./z_creator &");
    
    //TODO list all processes running
    system("<ps -l>");

    //TODO get zombie process id then get its PPID


    //kill parent process by id thats been given by PPID

    //list all processes again to ensure z process is no longer there

    

    return 0;
}