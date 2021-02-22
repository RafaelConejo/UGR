#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>


void main(int argc, char *argv[]){

	const int NUM_HIJOS = 5;
	int i;
	pid_t pid[NUM_HIJOS];
	
	for(i = 0; i < NUM_HIJOS; i++){
		if((pid[i] = fork()) == 0){
			printf("Soy el hijo de %d \n", getpid());
			break;
		}
		else if(pid[i] < 0){
			printf("Error al crear al proceso hijo");
			return -1;
		}
	}
	
	for(i = 0; i < NUM_HIJOS; i += 2){
		if(waitpid(pid[i], 0, 0) > 0){
			printf("Acaba de finalizar mi hijo ORDEN IMPAR con PID: %d \n", pid[i]);
			printf("Solo me quedan %d hijos vivos \n", NUM_HIJOS - (i+1));
		}
	}
	for(i = 1; i < NUM_HIJOS-1; i += 2){
		if(waitpid(pid[i], 0, 0) > 0){
			printf("Acaba de finalizar mi hijo ORDEN PAR con PID: %d \n", pid[i]);
			printf("Solo me quedan %d hijos vivos \n", NUM_HIJOS - (i+1));
		}
	}
	
	return 0;
}
