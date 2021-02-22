#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>



int main(int argc, char argv[]){

	int i = 1;
	int leidos;
	int filein, fileout;
	char bloque[80];
	char* output_file = "salida_ejer_2_version_2";
	
	//Aceptamos como argumento el nombre del archivo (pathname)
	if(argc == 2){
		filein = open(argv[1], O_RDONLY);			//Solo lectura
	}
	//En caso contrario cogemos la salida estandar como archivo de entrada
	else{
		filein = STDIN_FILENO;
	}
	
	//Creamos el archivo que será usado para escribir la salida
	fileout = open(output_file, O_CREAT | O_TRUNC | O_WRONLY,S_IRUSR | S_IWUSR);
	
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
	
	
	
	//Leemos 80 bytes del primer archivo, (pathname)
	while((leidos = read(filein, &bloque, 80)) > 0){
		
		//Genero un string para el bloque de numeros
		char bloque_ID[12];
		sprintf(bloque_ID, "## Bloque %i ##", i);
		write(fileout, &bloque_ID, sizeof(bloque_ID));
		write(fileout, "\n", 1);
		
		//Escribo los 80 bytes del bloque
		write(fileout, &bloque, leidos);
		write(fileout, "\n", 1);
		write(fileout, "\n", 1);
		
		//Aumento el contador del numero de bloques
		i++;
	}
	close(filein);
	close(fileout);
	return 0;

}
