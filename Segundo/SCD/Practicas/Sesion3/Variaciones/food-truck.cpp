//mpicxx -std=c++11 -g -Wall -o food-truck food-truck.cpp
//mpirun -np  11 ./food-truck


#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
            num_alumnos = 12,
            id_dependiente = num_alumnos,
            id_cocinero = id_dependiente + 1,

            num_procesos_esperado = 14,

            etiq_un_bocadillo = 0,
            etiq_dos_bocadillo = 1,
            etiq_dependiente = 2,
            etiq_cocinar = 3;

int num_bocatas = 20;

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio(){
   static default_random_engine generador( (random_device())() );
   static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
   return distribucion_uniforme( generador );
}



void proceso_alumno(int id_alumno){
   int peticion, confirmacion;
   MPI_Status estado;

   while(true){
      std::cout << "El alumno " << id_alumno << " camina hacia el camion" << '\n';
      sleep_for( milliseconds( aleatorio<100,200>()) );

      if(id_alumno % 2 == 0){
         std::cout << "El alumno " << id_alumno << " pide un bocadillo" << '\n';
         MPI_Ssend(&peticion, 1, MPI_INT, id_dependiente, etiq_un_bocadillo, MPI_COMM_WORLD);
      }
      else{
         std::cout << "El alumno " << id_alumno << " pide dos bocadillos" << '\n';
         MPI_Ssend(&peticion, 1, MPI_INT, id_dependiente, etiq_dos_bocadillo, MPI_COMM_WORLD);
      }

      MPI_Recv(&confirmacion, 1, MPI_INT, id_dependiente, etiq_dependiente, MPI_COMM_WORLD, &estado);
      std::cout << "El alumno " << id_alumno << " recibe su bocadillo" << '\n';
      sleep_for( milliseconds( aleatorio<100,200>()) );
   }
}


void proceso_cocinero(){
   int peticion, confirmacion;

   while(true){
      MPI_Recv(&peticion, 1, MPI_INT, id_dependiente, etiq_cocinar, MPI_COMM_WORLD, &estado);
      std::cout << "\t\tPreparando los bocatas" << '\n';
      sleep_for( milliseconds( aleatorio<100,200>()) );
      MPI_Ssend(&confirmacion, 1, MPI_INT, id_dependiente, etiq_cocinar, MPI_COMM_WORLD);
      std::cout << "\t\tBocatas preparados" << '\n';
   }
}


void proceso_dependiente(){
   int etiq_aceptable,
       peticion, confirmacion;
   MPI_Status estado;

   while (true) {
      if(num_bocatas >= 3){
         etiq_aceptable = MPI_ANY_TAG;
      }
      else if(num_bocatas == 1){
         etiq_aceptable = etiq_un_bocadillo;
      }
      else{
         MPI_Ssend(&peticion, 1, MPI_INT, id_cocinero, etiq_cocinar, MPI_COMM_WORLD);
         etiq_aceptable = etiq_cocinar;
      }

      MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado);

      switch( estado.MPI_TAG ){
         case etiq_un_bocadillo:
            MPI_Ssend(&confirmacion, 1, MPI_INT, estado.MPI_SOURCE, etiq_dependiente, MPI_COMM_WORLD);
            num_bocatas--;
            break;
         case etiq_dos_bocadillo:
            MPI_Ssend(&confirmacion, 1, MPI_INT, estado.MPI_SOURCE, etiq_dependiente, MPI_COMM_WORLD);
            num_bocatas = num_bocatas -2;
            break;

         case etiq_cocinar:
            num_bocatas = 20;
            break;
      }

   }
}





// ---------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   int id_propio, num_procesos_actual;

   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( num_procesos_esperado == num_procesos_actual )
   {
      // ejecutar la operación apropiada a 'id_propio'
      if ( id_propio < id_dependiente )
         proceso_alumno(id_propio);
      else if ( id_propio == id_dependiente )
         proceso_dependiente();
      else
         proceso_cocinero();
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
