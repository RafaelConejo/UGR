#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>


int main(int argc, char *argv[]){

	const int NUM_HIJOS = 5;
	int i;
	pid_t pid[NUM_HIJOS];
	
	for(i = 0; i < NUM_HIJOS; i++){
		if((pid[i] = fork()) == 0){
			printf("Soy el hijo de %d \n", getpid());
			exit(0);
		}
		else if(pid[i] < 0){
			printf("Error al crear al proceso hijo");
			return -1;
		}
	}
	
	for(i = 0; i < NUM_HIJOS; i++){
		if(waitpid(pid[i], 0, 0) > 0){
			printf("Acaba de finalizar mi hijo con PID: %d \n", pid[i]);
			printf("Solo me quedan %d hijos vivos \n", NUM_HIJOS - (i+1));
		}
	}
	return 0;
}
