#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

//g++ -std=c++11 -pthread -o gasolinera gasolinera.cpp Semaphore.cpp


//Defino variables globales necesarias
int n_surtidores = 0;
const int num_coches_gasoil = 4;
const int num_coches_gasolina = 6;
const int num_sutidor_gasoil = 2;
const int num_sutidor_gasolina = 3;
//Defino los semaforos
Semaphore surtidor_gasolina[num_sutidor_gasolina] = {Semaphore(1), Semaphore(1), Semaphore(1)};
Semaphore surtidor_gasoil[num_sutidor_gasoil] = {Semaphore(1), Semaphore(1)};
//mutex para controlar el numero total de sutidores en uso
mutex mtx;

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

void repostando_gasolina(int coche){
   // calcular milisegundos aleatorios de duración de la acción de repostar
   chrono::milliseconds duracion_produ( aleatorio<100,200>() );

   // informa de que comienza a producir
   cout << "Repostando coche de gasolina " << coche << " tardara (" << duracion_produ.count() << " milisegundos)" << endl;
   std::cout << "Surtidores ocupados: " << n_surtidores << '\n';

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   // informa de que ha terminado de producir
   cout << "Coche de gasolina " << coche << "  : termina de repostar." << endl;
}

void saliendo_gasolina(int coche){
   // calcular milisegundos aleatorios de duración de la acción de repostar
   chrono::milliseconds duracion_produ( aleatorio<100,200>() );

   // informa de que comienza a producir
   cout << "Coche de gasolina " << coche << " saliendo de la gasolinera. (" << duracion_produ.count() << " milisegundos)" << endl;
   std::cout << "Surtidores ocupados: " << n_surtidores << '\n';

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );
}




void repostando_gasoil(int coche){
   // calcular milisegundos aleatorios de duración de la acción de repostar
   chrono::milliseconds duracion_produ( aleatorio<100,200>() );

   // informa de que comienza a producir
   cout << "Repostando coche de gasoil " << coche << " tardara (" << duracion_produ.count() << " milisegundos)" << endl;
   std::cout << "Surtidores ocupados: " << n_surtidores << '\n';

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   // informa de que ha terminado de producir
   cout << "Coche de gasoil " << coche << "  : termina de repostar." << endl;
}

void saliendo_gasoil(int coche){
   // calcular milisegundos aleatorios de duración de la acción de repostar
   chrono::milliseconds duracion_produ( aleatorio<100,200>() );

   // informa de que comienza a producir
   cout << "Coche de gasoil " << coche << " saliendo de la gasolinera. (" << duracion_produ.count() << " milisegundos)" << endl;
   std::cout << "Surtidores ocupados: " << n_surtidores << '\n';

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );
}


//Funcion hebra coches de gasolina
void funcion_hebra_coche_gasolina( int coche_gasolina ){
  int surtidor;
  while(true){
    surtidor = (aleatorio<0,num_sutidor_gasolina-1>());
    sem_wait(surtidor_gasolina[surtidor]);

    mtx.lock();
    n_surtidores += 1;
    mtx.unlock();

    repostando_gasolina(coche_gasolina);

    mtx.lock();
    n_surtidores -= 1;
    mtx.unlock();

    sem_signal(surtidor_gasolina[surtidor]);
    saliendo_gasolina(coche_gasolina);
  }
}

//Funcion hebra coches de gasoil
void funcion_hebra_coche_gasoil( int coche_gasoil ){
  int surtidor;
  while(true){
    surtidor = (aleatorio<0,num_sutidor_gasoil-1>());
    sem_wait(surtidor_gasoil[surtidor]);

    mtx.lock();
    n_surtidores += 1;
    mtx.unlock();

    repostando_gasoil(coche_gasoil);

    mtx.lock();
    n_surtidores -= 1;
    mtx.unlock();

    sem_signal(surtidor_gasoil[surtidor]);
    saliendo_gasoil(coche_gasoil);
  }
}



int main()
{
   // declarar hebras y ponerlas en marcha
   thread hebra_coches_gasolina[num_coches_gasolina];
   thread hebra_coches_gasoil[num_coches_gasoil];

   for (int i = 0; i < num_coches_gasolina; i++) {
     hebra_coches_gasolina[i] = thread(funcion_hebra_coche_gasolina, i);
   }

   for (int i = 0; i < num_coches_gasoil; i++) {
     hebra_coches_gasoil[i] = thread(funcion_hebra_coche_gasoil, i);
   }

   for (int i = 0; i < num_coches_gasolina; i++) {
     hebra_coches_gasolina[i].join();
   }

   for (int i = 0; i < num_coches_gasoil; i++) {
     hebra_coches_gasoil[i].join();
   }
}
























//
