#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>



int main(int argc, char argv[]){

	int contador = 1;
	int leidos;
	int filein, fileout;
	char cadena[30];
	char cad_bloque[40];
	char caracter[1];
	int num_char = 1;
	char salto_linea[2] = "\n";
	
	//Aceptamos como argumento el nombre del archivo (pathname)
	if(argc == 2){
		filein = open(argv[1], O_RDONLY);			//Solo lectura
	}
	//En caso contrario cogemos la salida estandar como archivo de entrada
	else{
		filein = STDIN_FILENO;
	}
	
	//Creamos el archivo que será usado para escribir la salida
	fileout = open("archivo_salida", O_CREAT | O_TRUNC | O_WRONLY,S_IRUSR | S_IWUSR);
	
	//Caso de error al abrir el archivo de salida
	if(fileout < 0){
		printf("El archivo de salida no se pudo abrir correctamente \n");
		exit(-1);
	}
	
	//caso de error al abrir el archivo de entrada
	if(filein < 0){
		printf("El archivo de salida no se pudo abrir correctamente \n");
		exit(-1);
	}
	
	
	
	//Mientras haya algo que leer
	while((leidos = read(filein, caracter, 1)) != 0){
		if(num_char == 1 || num_char % 80 == 0){
			if(num_char != 1){
				printf("Num_char es != 1 \n");
				write(fileout, salto_linea, strlen(salto_linea));
			}
			else{
				sprintf(cad_bloque, "El numero de bloques es <%d> \n", contador);
				write(fileout, cad_bloque, strlen(cad_bloque));
			}
			sprintf(cad_bloque, "%s%d\n", "Bloque", contador);
			write(fileout, cad_bloque, strlen(cad_bloque));
			contador++;
		}
		write(fileout, caracter, 1);
		num_char++;
	}
	sprintf(cad_bloque, "El numero de bloques es <%d> \n", contador);
	lseek(fileout, 0, SEEK_SET);
	write(fileout, salto_linea, strlen(cad_bloque));
	close(filein);
	close(fileout);
	return 0;

}
