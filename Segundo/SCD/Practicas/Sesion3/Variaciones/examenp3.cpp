

//mpicxx -std=c++11 -g -Wall -o examenp3 examenp3.cpp
//mpirun -np  11 ./examenp3


#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

// Colores ansi para la consola
#define BLACK    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define BLUE     "\033[34m"
#define MAGENTA  "\033[35m"
#define CYAN     "\033[36m"
#define WHITE    "\033[37m"


const int
    num_clientes = 10,
    id_intermedio = num_clientes,

   etiq_solicitar        = 0 ,
   etiq_pagado       = 1 ,
   etiq_intermedio = 2,

   num_procesos_esperado =  11;

int num_cajas = 3;

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}




// ---------------------------------------------------------------------
void proceso_cliente(int id_cliente){
  int peticion;
  MPI_Status estado;

  while (true) {
    std::cout << RED << "Cliente " << id_cliente << " compra en la tienda." << '\n';
    sleep_for( milliseconds( aleatorio<100,200>()) );
    std::cout << YELLOW << "Cliente " << id_cliente << " solicita el pago" << '\n';
    MPI_Ssend(&peticion, 1, MPI_INT, id_intermedio, etiq_solicitar, MPI_COMM_WORLD);

    MPI_Recv( &peticion, 1, MPI_INT, id_intermedio, etiq_intermedio, MPI_COMM_WORLD, &estado);
    cout << MAGENTA << "Cliente " << id_cliente << " entra en la caja, comienza a pagar." << '\n';
    sleep_for( milliseconds( aleatorio<100,200>()) );

    MPI_Ssend(&peticion, 1, MPI_INT, id_intermedio, etiq_pagado, MPI_COMM_WORLD);
    std::cout << WHITE << "Cliente " << id_cliente << " ha finalizado el pago." << '\n';

    sleep_for( milliseconds( aleatorio<100,200>()) );
  }
}



// ---------------------------------------------------------------------
void proceso_intermedio(){
  int id_emisor_aceptable,
      peticion, confirmacion;
  MPI_Status estado;

  while (true) {
    //determinar si puedo solo aceptar pagos, o todos
    if(num_cajas > 0){
      id_emisor_aceptable = MPI_ANY_TAG;
      //otra forma:
         //MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado)
    }
    else{
      id_emisor_aceptable = etiq_pagado;
      //otra forma:
         //MPI_Probe(MPI_ANY_SOURCE, etiq_pagado, MPI_COMM_WORLD, &estado)
    }

    // 2. recibir un mensaje del emisor o emisores aceptables
    MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, id_emisor_aceptable, MPI_COMM_WORLD, &estado);

    // 3. procesar el mensaje recibido
    switch( estado.MPI_TAG ){
       case etiq_solicitar:
          num_cajas--;
          MPI_Ssend(&confirmacion, 1, MPI_INT, estado.MPI_SOURCE, etiq_intermedio, MPI_COMM_WORLD);
          break;

       case etiq_pagado:
          num_cajas++;
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
      if ( id_propio < id_intermedio )
         proceso_cliente(id_propio);
      else if ( id_propio == id_intermedio )
         proceso_intermedio();
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
