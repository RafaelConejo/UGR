#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#define NUM_PROC 20

void main(int argc, char *argv[]){
	int i = 0;
	pid_t pid1, pid2;
	
	//Jerarquia de procesos 1
	//Cada hijo genera un hijo
	
	printf("PID inicial: %d \n\n", getpid());
	
	for(i = 0; i < NUM_PROC; i++){
		if((pid1 = fork()) == 0){
			printf("Holi soy el hijo %d. Mi PID es %d y mi padre de PID %d. \n", i, getpid(), getppid());
			break;
		}
			
		else if(pid1 < 0){
			printf("No se ha podido crear el hijo en jerarquia 2");
			return;
		}
		else if(pid2 = (waitpid(pid1), 0, 0) > 0){}
			
	}
}
