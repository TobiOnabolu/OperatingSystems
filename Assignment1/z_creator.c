#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    
    int lifespan = 10;
    pid_t pid = fork();
    //process is a parent
    if (pid > 0){
        
    }

    //process is a child
    else if (pid == 0){

    }

    //error occured during fork
    else {

    }

    return 0;
}