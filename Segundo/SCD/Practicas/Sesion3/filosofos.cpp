// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-plantilla.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// -----------------------------------------------------------------------------

//mpicxx -std=c++11 -g -Wall -o filosofos filosofos.cpp
//mpirun -np  10 ./filosofos

#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;


const int
   num_filosofos = 5 ,
   num_procesos  = 2*num_filosofos,
   coger_tenedor = 0 ,             //etiqueta para coger un tenedor
   soltar_tenedor= 1;             //etiqueta para soltar un tenedor


//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos peticiones enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

// ---------------------------------------------------------------------

void funcion_filosofos( int id )
{
  int id_ten_izq = (id+1)              % num_procesos, //id. tenedor izq.
      id_ten_der = (id+num_procesos-1) % num_procesos; //id. tenedor der.
  int peticion;

  while ( true )
  {
    //caso del primer filosofo, debe coger los tenedores al reves para que no haya interbloquep
    if(id == 0){
      //solicita el tenedor derecho
      cout << "Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, coger_tenedor, MPI_COMM_WORLD);

      //solicita el tenedor izquierdo
      cout << "Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, coger_tenedor, MPI_COMM_WORLD);
    }

    else{
      //solicita el tenedor izquierdo
      cout << "Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, coger_tenedor, MPI_COMM_WORLD);

      //solicita el tenedor derecho
      cout << "Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, coger_tenedor, MPI_COMM_WORLD);
    }


    cout << "Filósofo " <<id <<" comienza a comer" <<endl ;
    sleep_for( milliseconds( aleatorio<10,100>() ) );


    //suelta el tenedor izquierdo
    cout << "Filósofo " <<id <<" suelta ten. izq. " <<id_ten_izq <<endl;
    MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

    //suelta el tenedor derecho
    cout << "Filósofo " <<id <<" suelta ten. der. " <<id_ten_der <<endl;
    MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);


    cout << "Filosofo " << id << " comienza a pensar" << endl;
    sleep_for( milliseconds( aleatorio<10,100>() ) );
    cout << "Filosofo " << id << " termina de pensar" << endl;
 }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int peticion, id_filosofo ;  // peticion recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {
     // recibir petición de cualquier filósofo
     MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, coger_tenedor, MPI_COMM_WORLD, &estado);
     // ...... guardar en 'id_filosofo' el id. del emisor (completar)
     id_filosofo = estado.MPI_SOURCE;       //Obtiene el filosofo
     cout << "Ten. " << id <<" ha sido cogido por filo. " << id_filosofo <<endl;

     // recibir liberación de filósofo 'id_filosofo'
     MPI_Recv(&peticion, 1, MPI_INT, id_filosofo, 0, MPI_COMM_WORLD, &estado);
     cout << "Ten. "<< id << " ha sido liberado por filo. " << id_filosofo <<endl ;
  }
}
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
   {
      // ejecutar la función correspondiente a 'id_propio'
      if ( id_propio % 2 == 0 )          // si es par
         funcion_filosofos( id_propio ); //   es un filósofo
      else                               // si es impar
         funcion_tenedores( id_propio ); //   es un tenedor
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
