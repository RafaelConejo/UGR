

//mpicxx -std=c++11 -g -Wall -o filo_ex filo_ex.cpp
//mpirun -np  11 ./filo_ex

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
   num_camarero = 1,
   num_procesos  = 2 * num_filosofos + num_camarero,
   id_camarero = 0,      //el camarero es el proceso 0

   MAX_SENTADOS = num_filosofos -1,

   coger_tenedor = 0 ,             //etiqueta para coger un tenedor
   soltar_tenedor= 1 ,             //etiqueta para soltar un tenedor

   etiq_sentarse = 2 ,//etiqueta para sentarse a comer
   etiq_levantarse = 3,//etiqueta para levantarse de comer

   id_filo_malafolla = 8;

bool malafolla_sentado = false;
bool hambrientos[num_procesos]{true};
bool come;
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
void funcion_camarero(){
  int filosofos_sentados = 0,
      etiq_aceptable,
      filosofo,
      peticion, confirmacion;
  MPI_Status estado;

  while(true){

    if(filosofos_sentados < MAX_SENTADOS){                   //el maximo de filosofos comiendo es 4
      etiq_aceptable = MPI_ANY_TAG;    //Puede sentarse o levantarse
    }
    else{
      etiq_aceptable = etiq_levantarse; //Solo puede levantarse
    }

    MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado);
    filosofo = estado.MPI_SOURCE;

    if(estado.MPI_TAG == etiq_sentarse){      //se le permite sentarse
      filosofos_sentados++;

      if(filosofo == id_filo_malafolla){
         malafolla_sentado = true;
         std::cout << "\t\t\tEl filosodo malafolla se sienta" << '\n';
      }


         if(malafolla_sentado && hambrientos[filosofo]){
            come = true;
            hambrientos[filosofo] = false;
            std::cerr << "\n---> Esta sentado el Filosofo malafolla pero tiene hambre el filosofo << " << filosofo << '\n';
         }
         else if(malafolla_sentado && hambrientos[filosofo] == false){
            come = false;
            hambrientos[filosofo] = true;
            std::cout << "\n--> Esta sentado el filosofo malafolla y no tiene hambre el filosofo " << filosofo << " No come." << '\n';
         }
         else{
            come = true;
            hambrientos[filosofo] = false;
         }
      

      MPI_Ssend(&confirmacion, 1, MPI_INT, filosofo, etiq_sentarse, MPI_COMM_WORLD);

      std::cout << "\t\tFilosofo " << estado.MPI_SOURCE << " se sienta. Hay " << filosofos_sentados << " filosofos sentados" << '\n';
    }
    else if(estado.MPI_TAG == etiq_levantarse){    //se le permite levantarse
      filosofos_sentados--;

      if(filosofo == id_filo_malafolla){
         malafolla_sentado = false;
         std::cout << "\t\t\tEl filosodo malafolla se levante" << '\n';
      }

      std::cout << "\t\tFilosofo " << estado.MPI_SOURCE << " se levanta. Hay " << filosofos_sentados << " filosofos sentados" << '\n';
    }
    else{
      std::cout << "El camarero ha recibido un mensaje incorrecto" << '\n';
      exit(1);
    }
  }
}




void funcion_filosofos( int id )
{
  int id_ten_izq = (id+1) % (num_procesos - num_camarero), //id. tenedor izq.
      id_ten_der = (id + num_procesos - 2) % (num_procesos - num_camarero); //id. tenedor der.

  int peticion, confirmacion;
  MPI_Status estado;

  while ( true ){
       //El filosofo pide sentarse
       std::cout << "Filosofo " << id << " solicita sentarse a comer" << '\n';
       MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_sentarse, MPI_COMM_WORLD);

       //confirmacion de la peticion para sentarse
       MPI_Recv(&confirmacion, 1, MPI_INT, id_camarero, etiq_sentarse, MPI_COMM_WORLD, &estado);
       std::cout << "Filosofo " << id << " se sienta" << '\n';


   if(come){

      //solicita el tenedor izquierdo
      cout << "\tFilósofo " << id << " solicita ten. izq." << id_ten_izq << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, coger_tenedor, MPI_COMM_WORLD);

      //solicita el tenedor derecho
      cout << "\tFilósofo " << id <<" solicita ten. der." << id_ten_der << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, coger_tenedor, MPI_COMM_WORLD);



       cout << "Filósofo " << id << " comienza a comer" << endl ;
       sleep_for( milliseconds( aleatorio<10,100>() ) );


       //suelta el tenedor izquierdo
       cout << "\tFilósofo " << id <<" suelta ten. izq. " << id_ten_izq << endl;
       MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, soltar_tenedor, MPI_COMM_WORLD);

       //suelta el tenedor derecho
       cout << "\tFilósofo " << id << " suelta ten. der. " << id_ten_der << endl;
       MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, soltar_tenedor, MPI_COMM_WORLD);

    }

       //el filosofo se levanta
       MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_levantarse, MPI_COMM_WORLD);
       std::cout << "Filosofo " << id << " se levanta" << '\n';

       cout << "Filosofo " << id << " comienza a pensar" << endl;
       sleep_for( milliseconds( aleatorio<10,100>() ) );
       cout << "Filosofo " << id << " termina de pensar" << endl;

 }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int valor, id_filosofo ;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {
     // recibir petición de cualquier filósofo
     MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, coger_tenedor, MPI_COMM_WORLD, &estado);
     // ...... guardar en 'id_filosofo' el id. del emisor (completar)
     id_filosofo = estado.MPI_SOURCE;       //Obtiene el filosofo
     cout << "Ten. " << id <<" ha sido cogido por filo. " << id_filosofo <<endl;

     // recibir liberación de filósofo 'id_filosofo'
     MPI_Recv(&valor, 1, MPI_INT, id_filosofo, soltar_tenedor, MPI_COMM_WORLD, &estado);
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


   if ( num_procesos == num_procesos_actual){
      // ejecutar la función correspondiente a 'id_propio'
      if(id_propio == id_camarero)
        funcion_camarero();
      else if ( id_propio % 2 == 0 )          // si es par
        funcion_filosofos( id_propio ); //   es un filósofo
      else                               // si es impar
        funcion_tenedores( id_propio ); //   es un tenedor
   }
   else{
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
