#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>

//Tambien puedo definirlos fuera evidentemente
//struct stat atributos;
//struct dirent *fichero;
//DIR *dirp;
//int chmod_result, permisos_antiguos, permisos_nuevos;



int main(int argc, char *argv[]){
	struct stat atributos;
	struct dirent *fichero;
	DIR *dirp;
	int chmod_result, permisos_antiguos, permisos_nuevos;
	
	if(argc == 3){
		//abro el directorio
		dirp = opendir(argv[1]);
		
		if(dirp == NULL){
			printf("No se puede abrir el directorio: %s\n", argv[1]);
			exit(2);
		}
	}
	//Caso paramentros dados mal
	else{
		printf("Uso: ejer_2 <pathname> <permisos>\n");
		exit(-1);
	}
	
	while((fichero = readdir(dirp)) != NULL){
		//cambio los permisos
		chdir(argv[1]);
		
		//comprobamos si puedo acceder al fichero (d_name) en el directorio y a sus atributos almacenados en &atributos
		if(stat(fichero->d_name, &atributos) < 0){
			printf("Error al intentar acceder a los atributos de %s\n", fichero->d_name);
			exit(-1);
		}
		else{
			//Comprobamos que no es un directorio
			if(!S_ISDIR(atributos.st_mode)){
				//asignamos nuevos permisos
				permisos_nuevos = strtol(argv[2], 0, 8) & 0777;
				
				//guardamos permisos antiguos
				permisos_antiguos = atributos.st_mode & 0777;
				
				//cambio de permisos
				chmod_result = chmod(fichero->d_name, permisos_nuevos);
				
				//comprobamos si se ha realizado el cambio
				if(chmod_result < 0){
					printf("No se han cambiado los permisos, error");
					exit(-1);
				}
				else{
					printf("%s : \t%o \t%o\n", fichero->d_name, permisos_antiguos, permisos_nuevos);
				}
			};
		}
	}
	closedir(dirp);
	return 0;
}






































//
