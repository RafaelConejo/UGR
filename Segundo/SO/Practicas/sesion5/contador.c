#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>



/*
	Creamos un array para contar el numero de veces que se ha recibido la señal i-esima
	y otro contador para saber si la señal i-esima se puede o no se puede manejar
	(se puede manejar la señal i ... 0 = true, 1 = false)
*/


static int contador_señales[65]={0};	//la posicion 0 no es usada
static int señales_invalidas[65]={0};	//la posicion 0 no es usada



static void manejador(int signum){
	if(signum > 64 || signum < 0){
		printf("El programa no puede capturar la señal %d\n", signum);
		
	}
}
