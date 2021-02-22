#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define tamano 1024
#define longnombre 50



int main(int argc, char const *argv[]) {
  //Variables
  char nombrefifoe[longnombre];   //nombre del FIFO de entrada
  char nombrefifos[longnombre];   //nombre del FIFO de salida
  int dfifoe, dfifos;             //Descriptores del FIFO de entrada y de salida
  int pid;
  int resultado;
  int dfifoproxy;
  char nombrefifoproxy[longnombre]; //nombre del FIFO del proxy


  //Evaluamos los parametros de entrada
  if(argc < 2){
    printf("Servidor: Formado: servidor <nombre fifo> \n");
    exit(EXIT_FAILURE);
  }

  //Creamos los nombres de los FIFOs
  sprintf(nombrefifoe, "%se", argv[1]);
  sprintf(nombrefifos, "%ss", argv[1]);


  //Creamos los FIFOs para comunicarnos con los clientes (nkfifo o nknod)
  if(mkfifo(nombrefifoe, S_IRWXU) == -1){
    //Con este if nos ahorramos tener que borrarlos y volver a crearlos
    if(errno == EEXIST)
      printf("Servidor: FIFO ya existe \n");

    else{
        printf("Servidor: error al crear FIFO \n");
        exit(1);
    }
  }
  if(mkfifo(nombrefifos, S_IRWXU) == -1){
    if(errno == EEXIST)
      printf("Servidor: FIFO ya existe \n");

    else{
        printf("Servidor: error al crear FIFO \n");
        exit(1);
    }
  }


  //Creamos el archivo de bloqueo
    creat("bloqueo", S_IRWXU);


  //Abrimos los FIFOs (open)
  if( (dfifoe = open(nombrefifoe, O_RDWR)) == -1){
    printf("Servidor: error al abrir FIFO \n");
    exit(1);
  }
  if( (dfifos = open(nombrefifos, O_RDWR)) == -1){
    printf("Servidor: error al abrir FIFO \n");
    exit(1);
  }


//El servidor siempre lee las peticiones del archivo FIFO
  while(resultado = read(dfifoe, &pid, sizeof(int))){
    //Creamos los procesos de cada cliente
    if( (pid = fork()) == -1){
      printf("Servidor: error al crear un proceso \n");
      exit(1);
    }
    else if(pid == 0){
      //codigo del hijo / el que será el proxy
      pid = getpid();
      sprintf(nombrefifoproxy, "fifo.%d", pid);

      //creamos el fichero
      mkfifo(nombrefifoproxy, S_IRWXU);

      //Escribimos el PID del proxy
      write(dfifos, &pid, sizeof(int));

      //Lo Abrimos
      dfifoproxy = open(nombrefifoproxy, O_RDONLY);

      //redireccionamos la entrada estandar
      dup2(dfifoproxy, STDIN_FILENO);
      close(dfifoproxy);


      //Lanzamos el programa proxy
      execlp("./proxy", "proxy", NULL);
      printf("Servidor: error al ejecutar el proxy \n");
      exit(1);
    }
  }

}
