#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

void par_impar(int valor){
	if(valor % 2 == 0)
		printf("Holi, soy el hijo. Se trata de un numero Par. \n");
	else
		printf("Holi, soy el hijo. Se trata de un numero Impar. \n");
}

void divisible_4(int valor){
	if(valor % 4 == 0)
		printf("Holi, soy el Padre. Se trata de un numero Divisible por 4. \n");
	else
		printf("Holi, soy el Padre. Se trata de un numero Indivisible por 4. \n");
}


void main(int argc, char *argv[]){
	if(argc == 2){
		pid_t pid = fork();
		
		if(pid == 0){
			par_impar(atoi(argv[1]));
		}
		else if(pid > 0){
			divisible_4(atoi(argv[1]));
		}
	}
	else
		printf("Debes poner un numero: ./ejer_1 numero \n");
}
