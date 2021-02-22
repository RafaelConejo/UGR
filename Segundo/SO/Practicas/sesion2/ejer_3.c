#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>

int n_ficheros;
long unsigned tam = 0;

void buscar_dir(char* path, char* file_name){
	struct stat atributos;
	struct dirent *fichero;
	DIR *dirp;
	char temp_path[256], file_name_path[256];
	
	strcpy(file_name_path, file_name);
	strcpy(temp_path, path);
	strcat(temp_path, file_name_path);
	
	//Abro el directorio
	dirp = opendir(file_name_path);
	if(dirp == NULL){
		printf("No se puede abrir el directorio: %s\n", temp_path);
		exit(2);
	}
	
	while((fichero = readdir(dirp)) != NULL){
		//Cambio directorio de trabajo
		chdir(file_name_path);
		
		//compruebo si puedo acceder al fichero (d_name) en el directorio y a sus atributos almacenados en &atributos
		if(stat(fichero->d_name, &atributos) < 0){
			printf("Error al intentar acceder a los atributos de %s%s\n", temp_path, fichero->d_name);
			exit(-1);
		}
		else{
			//Si puedo acceder compruebo si es fichero regular o directorio
			if(fichero->d_type != DT_DIR){
				//Si es un fichero regular
				if((S_ISREG(atributos.st_mode)) && (atributos.st_mode & S_IXGRP) && (atributos.st_mode & S_IXOTH)){
					//Los inodos son unsigned long int, necesito lu
					printf("./%s%s			%lu\n", temp_path, fichero->d_name, atributos.st_ino);
					//incremento los contadores
					n_ficheros++;
					tam += atributos.st_ino;
				}
			}
			else{
				//Si es un nuevo directorio se llama a si misma, recursivamente
				if(strcmp(fichero->d_name,".") != 0 && strcmp(fichero->d_name,"..") != 0){
					buscar_dir(temp_path, strcat(fichero->d_name,"/"));
					//Cuando sale de la recursividad, tiene que salir del directorio tambien
					chdir("../");
				}
			}
		}
	}
	closedir(dirp);
}








int main(int argc, char *argv[]){
	printf("Los inodos son:\n");
	
	buscar_dir("", argv[1]);
	
	printf("Existen %d archivos regulares con permisos x para grupo y otros\n", n_ficheros);
	printf("El tamaño total ocupado por dichos archivos es de %lu bytes\n", tam);
	
	return 0;
}





























//
