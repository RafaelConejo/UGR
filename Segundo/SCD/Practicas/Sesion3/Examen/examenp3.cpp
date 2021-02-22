//mpicxx -std=c++11 -g -Wall -o examenp3 examenp3.cpp
//mpirun -np  15 ./examenp3


#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;


const int
         num_clientes = 11,
         num_mostradores = 3,
         num_cola = 1,

         id_clientes = num_clientes,
         id_mostradores = num_clientes + num_mostradores,
         id_controlador = id_mostradores + num_cola,

         num_procesos_esperado =  15,

         etiq_solicitar = 0,
         etiq_completado = 1,
         etiq_controlador = 2,
         etiq_llegada = 3,
         etiq_salida = 4;

int mostradores_libres = 3;
int mostradores[num_mostradores] = {true, true, true};

template< int min, int max > int aleatorio(){
   static default_random_engine generador( (random_device())() );
   static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
   return distribucion_uniforme( generador );
}


void proceso_cliente(int id_cliente){
   int peticion, confirmacion, n_m;
   MPI_Status estado;

   while(true){
      std::cout << "El Cliente " << id_cliente << " solicita un mostrador" << '\n';
      MPI_Ssend(&peticion, 1, MPI_INT, id_controlador, etiq_solicitar, MPI_COMM_WORLD);

      MPI_Recv(&n_m, 1, MPI_INT, id_controlador, etiq_controlador, MPI_COMM_WORLD, &estado);

      std::cout << "El Cliente " << id_cliente << " llega al mostrador " << n_m << '\n';
      MPI_Ssend(&n_m, 1, MPI_INT, id_mostradores, etiq_llegada, MPI_COMM_WORLD);

      MPI_Recv(&n_m, 1, MPI_INT, id_mostradores, etiq_salida, MPI_COMM_WORLD, &estado);

      MPI_Ssend(&n_m, 1, MPI_INT, id_controlador, etiq_completado, MPI_COMM_WORLD);
      sleep_for( milliseconds( aleatorio<100,200>()) );

   }
}

void proceso_mostrador(){
   int n_m, cliente;
   MPI_Status estado;

   while(true){
      MPI_Recv(&n_m, 1, MPI_INT, MPI_ANY_SOURCE, etiq_llegada, MPI_COMM_WORLD, &estado);
      cliente = estado.MPI_SOURCE;

      sleep_for( milliseconds( aleatorio<100,200>()) );

      MPI_Ssend(&cliente, 1, MPI_INT, id_clientes, etiq_salida, MPI_COMM_WORLD);
   }
}


void proceso_controlador(){
   int peticion, confirmacion, n_m, mostrador_elegido;
   int tag_admitido;
   MPI_Status estado;

   while(true){
      if(mostradores_libres >= 3){
         tag_admitido = MPI_ANY_TAG;
      }
      else{
         tag_admitido = etiq_solicitar;
      }

      MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, tag_admitido, MPI_COMM_WORLD, &estado);

      switch( estado.MPI_TAG ){
         case etiq_completado:
            mostradores_libres++;
            break;
         case etiq_solicitar:
            int i = 0;
            bool buscar = true;
            while(buscar){
               if(mostradores[i] == true){
                  mostrador_elegido = i;
                  mostradores[i] == false;
                  buscar = false;
               }
               i++;
            }
            n_m = mostrador_elegido;

            mostradores_libres--;

            MPI_Ssend(&n_m, 1, MPI_INT, estado.MPI_SOURCE, etiq_controlador, MPI_COMM_WORLD);
            break;
      }

   }
}



// ---------------------------------------------------------------------

int main( int argc, char *argv[] ){
            int id_propio, num_procesos_actual;

            // inicializar MPI, leer identif. de proceso y número de procesos
            MPI_Init( &argc, &argv );
            MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
            MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

            if ( num_procesos_esperado == num_procesos_actual )
            {
               // ejecutar la operación apropiada a 'id_propio'
               if ( id_propio < id_clientes )
                  proceso_cliente(id_propio);
               else if ( id_propio < id_mostradores && id_propio >= id_clientes)
                  proceso_mostrador();
               else if( id_propio == id_controlador){
                  proceso_controlador();
               }
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
