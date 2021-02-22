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


// Funcion Para bloquear/desbloquear el descriptor de archivos "dbloqueo"
  // Como argumento ordenes usar:
      // F_WRLCK para bloquear
      // F_UNLCK para desbloquear

void bloqueodesbloqueo(int dbloqueo, int orden){
  struct flock cerrojo;

  //Inicializamos el cerrojo para bloquear todo el archivo
  cerrojo.l_type = orden;
  cerrojo.l_whence = SEEK_SET;
  cerrojo.l_start = 0;
  cerrojo.l_len = 0;

  //Si vamos a bloquarlo y ya lo esta, entonces el proceso duerme
  if(fcntl(dbloqueo, F_SETLKW, &cerrojo) == -1){
    perror("Proxy: problemas al bloquear para impresion\n");
    exit(1);
  }
}



int main(int argc, char const *argv[]) {
  /*
    1. Leer el contenido del FIFO proxy
    2. Almacenar en su archivo temporal
    3. Bloquear en pantalla e imprimir
    4. Borrar archivos
  */


  int FIFO_proxy;     //FIFO_proxy es un descriptor de archivos para el FIFO: "nombre_fifo_proxy"
  int ArchivoTMP;
  int leidos;
  char buffer[1024];   //Buffer para intercambios
  char nombre_fifo_proxy[80],nombre_temporal[10];   //Buffers para nombres

  //Ponemos el nombre del fifo.<pid_proxy> en el buffer nombre_fifo_proxy
  sprintf(nombre_fifo_proxy,"fifo.%i",getpid());


  //Abrimos el FIFO_proxy en modo Solo Lectura

  //De esta forma nos percatamos cuando el cliente acaba ya que
  //en ese momento deja de haber procesos escribiendo en el cauce.
  //NOTA read devuelve 0 cuando no hay escritores
  if((FIFO_proxy = open(nombre_fifo_proxy,O_RDONLY)) < 0){
    perror("Proxy: Error al abrir el Proxy");
    exit(1);
  }


  sprintf(nombre_temporal,"temp"); //nombre_temporal[10]


  //Abrimos/Creamos/Inicializamos un archivo temporal en modo lectura escritura
  //En el que volcaremos toda la informacion procedente del cliente para luego
  //una vez el cliente acabe imprimirlo todo de golpe en pantalla
  //Como vamos a leer y escribir damos permisos de lectura escritura
  if((ArchivoTMP = open(nombre_temporal,O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) < 0){
    perror("Proxy: Error al abrir el archivo temporal");
    exit(1);
  }


  //Leemos del FIFO_proxy y escribimos en un buffer y vamos escribiendo
  //todo lo que leemos del FIFO_proxy en el archivo temporal.
  while((leidos = read(FIFO_proxy,buffer,1024)) > 0){
    write(ArchivoTMP,buffer,leidos);
  }


  //Ahora una vez el cliente ha acabado ponemos el puntero de lectura/escritura
  //al principio del archivo temporal y lo leemos y escribimos en la STDOUT
  //(la pantalla por defecto)
  //Para evitar que los otros clientes se cuelen y salgan cosas tipo:
  //"aaaaaaaabbaaaaaaaaaccccccccccaaaaaaaaabbbbbbbb"
  //se bloquea la pantalla mientras escribimos y se desbloquea luego
  lseek(ArchivoTMP,SEEK_SET,0);
  bloqueodesbloqueo(STDOUT_FILENO,F_WRLCK);

  while((leidos = read(ArchivoTMP,buffer,1024)) > 0){
    write(STDOUT_FILENO,buffer,leidos);
  }

  bloqueodesbloqueo(STDOUT_FILENO,F_UNLCK);


  //Borramos los archivos que ha creado el proxy:
  unlink(nombre_fifo_proxy); //Borramos el fifo
  unlink(nombre_temporal); //Borramos el archivo tmp
}
