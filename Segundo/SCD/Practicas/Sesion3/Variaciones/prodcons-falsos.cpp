

//mpicxx -std=c++11 -g -Wall -o prodcons-falsos prodcons-falsos.cpp
//mpirun -np  14 ./prodcons-falsos


#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;


const int
   etiq_productor          = 0 ,
   etiq_consumidor         = 1 ,
   etiq_productor_falso = 2,

   np                    = 2,           //np, numero de productores
   np_falsos            = 2,
   nc                    = 8,           //nc, numero de consumidores
   id_buffer             = np+np_falsos,
   id_buffer_falso      = id_buffer+1,
   id_consumidor_mas_bajo = id_buffer + 2,

   num_procesos_esperado = np + nc + 2 + np_falsos,

   num_items             = 8,
   tam_vector            = 10,
   items_producidos      = num_items / np,
   items_consumidos      = num_items / nc,
   items_producidos_falsos = num_items / np_falsos;

   bool primera_vez = true;

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


int calcular_id_consumidor(int id_origen){
   return id_origen - ( id_buffer + 1 );
}


// ---------------------------------------------------------------------
// ptoducir produce los numeros en secuencia (1,2,3,....)
// y lleva espera aleatorio
int producir(int id_productor)
{
   static int contador = id_productor * items_producidos;
   sleep_for( milliseconds( aleatorio<10,100>()) );
   contador++ ;
   cout << "Productor " << id_productor << " ha producido valor " << contador << endl << flush;
   return contador ;
}

int producir_falso(int id_productor)
{
   static int contador = id_productor * items_producidos_falsos;
   sleep_for( milliseconds( aleatorio<10,100>()) );
   contador = 44 ;
   cout << "Productor " << id_productor << " ha producido valor 44" << endl << flush;
   return contador ;
}



// ---------------------------------------------------------------------

void funcion_productor(int id_productor)
{
   for ( unsigned int i = 0 ; i < items_producidos ; i++ )
   {
      // producir valor
      int valor_prod = producir(id_productor);
      // enviar valor
      cout << "Productor " << id_productor << " va a enviar valor " << valor_prod << endl << flush;
      MPI_Ssend( &valor_prod, 1, MPI_INT, id_buffer, etiq_productor, MPI_COMM_WORLD );
   }
}

void funcion_productor_falso(int id_productor)
{
   for ( unsigned int i = 0 ; i < items_producidos_falsos ; i++ )
   {
      // producir valor
      int valor_prod = producir_falso(id_productor);
      // enviar valor
      cout << "Productor " << id_productor << " va a enviar valor " << valor_prod << endl << flush;
      MPI_Ssend( &valor_prod, 1, MPI_INT, id_buffer_falso, etiq_productor_falso, MPI_COMM_WORLD );
   }
}



// ---------------------------------------------------------------------

void consumir( int valor_cons, int id_consumidor )
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<110,200>()) );
   cout << "Consumidor " << id_consumidor << " ha consumido valor " << valor_cons << endl << flush ;
}



// ---------------------------------------------------------------------

void funcion_consumidor(int id_consumidor)
{
   int         peticion,
               valor_rec = 1 ;
   MPI_Status  estado ;

   for( unsigned int i = 0 ; i < items_consumidos; i++ )
   {
      MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_consumidor, MPI_COMM_WORLD);
      MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_productor, MPI_COMM_WORLD, &estado );
      cout << "Consumidor " << id_consumidor << " ha recibido valor " << valor_rec << endl << flush ;
      consumir( valor_rec, id_consumidor);
   }
}

void funcion_consumidor_falso(int id_consumidor)
{
   int         peticion,
               valor_rec = 1 ;
   MPI_Status  estado ;

   int comienzo;

   for( unsigned int i = 0 ; i < items_consumidos; i++ )
   {
      if(primera_vez){
         primera_vez = false;
         comienzo = aleatorio<0,1>();
      }

      if(comienzo == 0){
         MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_consumidor, MPI_COMM_WORLD);
         MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_productor, MPI_COMM_WORLD, &estado );
         cout << "Consumidor " << id_consumidor << " ha recibido valor " << valor_rec << endl << flush ;
         comienzo = 1;
         consumir( valor_rec, id_consumidor);
      }
      else{
         MPI_Ssend( &peticion,  1, MPI_INT, id_buffer_falso, etiq_consumidor, MPI_COMM_WORLD);
         MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer_falso, etiq_productor_falso, MPI_COMM_WORLD, &estado );
         cout << "Consumidor " << id_consumidor << " ha recibido valor falso" << valor_rec << endl << flush ;
         comienzo = 0;
         consumir( valor_rec, id_consumidor);
      }

   }
}
// ---------------------------------------------------------------------

void funcion_buffer()
{
   int        buffer[tam_vector],      // buffer con celdas ocupadas y vacías
              valor,                   // valor recibido o enviado
              primera_libre       = 0, // índice de primera celda libre

              num_celdas_ocupadas = 0, // número de celdas ocupadas
              id_emisor_aceptable ;    // identificador de emisor aceptable
   MPI_Status estado ;                 // metadatos del mensaje recibido

   for( unsigned int i=0 ; i < num_items*2 ; i++ )
   {
      // 1. determinar si puede enviar solo prod., solo cons, o todos

      if ( num_celdas_ocupadas == 0 )               // si buffer vacío
         id_emisor_aceptable = etiq_productor;       // $~~~$ solo prod.

      else if ( num_celdas_ocupadas == tam_vector ) // si buffer lleno
         id_emisor_aceptable = etiq_consumidor;      // $~~~$ solo cons.

      else                                          // si no vacío ni lleno
         id_emisor_aceptable = MPI_ANY_TAG ;     // $~~~$ cualquiera

      // 2. recibir un mensaje del emisor o emisores aceptables

      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, id_emisor_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      switch( estado.MPI_TAG ) // leer emisor del mensaje en metadatos
      {
         case etiq_productor: // si ha sido el productor: insertar en buffer
            buffer[primera_libre] = valor ;
            primera_libre++;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor " << valor << endl ;
            break;

         case etiq_consumidor: // si ha sido el consumidor: extraer y enviarle
            valor = buffer[primera_libre-1] ;
            primera_libre--;
            num_celdas_ocupadas-- ;
            cout << "Buffer va a enviar valor " << valor << endl ;
            MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_productor, MPI_COMM_WORLD);
            break;
      }
   }
}


void funcion_buffer_falso()
{
   int        buffer[tam_vector],      // buffer con celdas ocupadas y vacías
              valor,                   // valor recibido o enviado
              primera_libre       = 0, // índice de primera celda libre

              num_celdas_ocupadas = 0, // número de celdas ocupadas
              id_emisor_aceptable ;    // identificador de emisor aceptable
   MPI_Status estado ;                 // metadatos del mensaje recibido

   for( unsigned int i=0 ; i < num_items*2 ; i++ )
   {
      // 1. determinar si puede enviar solo prod., solo cons, o todos

      if ( num_celdas_ocupadas == 0 )               // si buffer vacío
         id_emisor_aceptable = etiq_productor_falso;       // $~~~$ solo prod.

      else if ( num_celdas_ocupadas == tam_vector ) // si buffer lleno
         id_emisor_aceptable = etiq_consumidor;      // $~~~$ solo cons.

      else                                          // si no vacío ni lleno
         id_emisor_aceptable = MPI_ANY_TAG ;     // $~~~$ cualquiera

      // 2. recibir un mensaje del emisor o emisores aceptables

      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, id_emisor_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      switch( estado.MPI_TAG ) // leer emisor del mensaje en metadatos
      {
         case etiq_productor_falso: // si ha sido el productor: insertar en buffer
            buffer[primera_libre] = 44 ;
            primera_libre++;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor falso " << valor << endl ;
            break;

         case etiq_consumidor: // si ha sido el consumidor: extraer y enviarle
            valor = buffer[primera_libre-1] ;
            primera_libre--;
            num_celdas_ocupadas-- ;
            cout << "Buffer va a enviar valor falso " << valor << endl ;
            MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_productor_falso, MPI_COMM_WORLD);
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
      if ( id_propio < 2 )
         funcion_productor(id_propio);

      else if(id_propio < id_buffer){
         funcion_productor_falso(id_propio);
      }

      else if ( id_propio == id_buffer )
         funcion_buffer();

      else if(id_propio == id_buffer_falso){
         funcion_buffer_falso();
      }

      else if(id_propio == id_consumidor_mas_bajo){
         id_propio = calcular_id_consumidor(id_propio);
         funcion_consumidor_falso(id_propio);
      }

      else{
         id_propio = calcular_id_consumidor(id_propio);         //Para que salga 0,1,2..
         funcion_consumidor(id_propio);                         // y no 5,6,7...
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
