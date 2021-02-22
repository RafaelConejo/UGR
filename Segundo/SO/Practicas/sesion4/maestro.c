#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

void esclavo(int inicio, int fin, int *fd){
	for(int n = inicio; n < fin; n++){
		int k;
		for(k = 2; k < (int)sqrt(n); k++){
			printf("%d\t%d\t%d\n", n, k, (int)sqrt(n));
			if(n % k == 0)
				break;				//no es primo
		}
		if((k == (int)sqrt(n) && n % k != 0) || n == 2 || n == 3){
			printf("El numero %d es primo. \n", n);
			write(fd[1], &n, sizeof(int));
		}
	}
	int senialFinal = -1;
	write(fd[1], &senialFinal, sizeof(int));
	exit(0);
}


int main(int argc, char *argv[]){
	int inicio, fin, mitad, numero, fd1[1], fd2[2];
	char parametros[2], buffer[80];
	pid_t pid[2];
	
	//Comprobamos que se han pasado dos enteros como parametros
	if(argc != 3){
		printf("Modo de uso: %s <inicio> <final>\n\n", argv[0]);
		exit(1);
	}
	
	//COnvertimos el argumento a int, y calculo el punto de inicio por esclavo 2
	inicio = atoi(argv[1]);
	fin = atoi(argv[2]);
	mitad = inicio + ((fin - inicio) / 2);
	
	//Creamos los pipe
	pipe(fd1);
	pipe(fd2);
	
	if((pid[0] = fork()) < 0){
		perror("Error al hacer el fork");
		exit(1);
	}
	//proceso hijo
	if(pid[0] == 0){
		//Cierre del descriptor de lectura del proceso hijo
		close(fd1[0]);
		parametros[0] = inicio;
		parametros[1] = mitad;
		//Envio el mensaje a traves del cauce usando el descriptor de escritura
		printf("Paso 1\n");
		dup2(fd1[1], STDOUT_FILENO);
		
		esclavo(parametros[0], parametros[1], fd1);
	}
	else{
		//Estoy en el proceso padre porque pid != 0
		if((pid[1] = fork()) < 0){
			perror("Error al hacer el fork");
			exit(1);
		}
		
		if(pid[1] == 0){
			//Cierre del descriptor de lectura del proceso hijo
			close(fd2[0]);
			
			parametros[0] = mitad + 1;
			parametros[1] = fin;
			
			//Envio el mensaje a traves del cauce usando el descriptor de escritura
			printf("Paso 2\n");
			dup2(fd2[1], STDOUT_FILENO);
			
			esclavo(parametros[0], parametros[1], fd2);
			
			exit(0);
		}
	}
	
	if(waitpid(pid[0], 0, 0) > 0 && waitpid(pid[1], 0, 0) > 0){
		//Cerramos el descriptor de escritura del proceso padre
		close(fd1[1]);
		close(fd2[1]);
		
		//Leimos datos desde el cauce
		dup2(fd1[0], STDIN_FILENO);
		dup2(fd2[0], STDIN_FILENO);
		
		numero = 0;
		
		printf("Paso 3\n");
		//Veamos los resultados
		while(numero > 0){
			read(fd1[0], &numero, 4);
			printf("Numero primo de esclavo 1: %d\n", numero);
		}
		
		close(fd1[0]);
		
		while(numero > 0){
			read(fd2[0], &numero, 4);
			printf("Numero primo de esclavo 2: %d\n", numero);
		}
		
		close(fd2[0]);
	}
	exit(0);
}






























//
