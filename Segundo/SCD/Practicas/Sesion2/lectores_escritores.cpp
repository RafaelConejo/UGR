#include <iostream>
#include <cassert>
#include <iomanip>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <condition_variable>
#include "HoareMonitor.h"

using namespace std ;
using namespace HM ;

//g++ -std=c++11 -pthread -o lectores_escritores lectores_escritores.cpp HoareMonitor.cpp Semaphore.cpp

const int num_lectores = 1;
const int num_escritores = 2;
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

//**********************************************************************
//Clase del monitor Estanco
//**********************************************************************
class Lec_Esc : public HoareMonitor{
  private:
    int n_lectores;      //numero de lectores
    bool escribiendo;          //true si hay algun escribiendo
    CondVar lectura;              //no hay escritores escribiendo, lectura posible
    CondVar escritura;            //no hay lectores ni escritores, escritura posible

  public:
    Lec_Esc();
    void inicio_lectura();
    void fin_lectura();
    void inicio_escritura();
    void fin_escritura();
};




Lec_Esc::Lec_Esc(){
  n_lectores = 0;
  escribiendo = false;
  for (size_t i = 0; i < num_lectores; i++) {
    lectura = newCondVar();
  }
  for (size_t i = 0; i < num_escritores; i++) {
    escritura = newCondVar();
  }
}


void Lec_Esc::inicio_lectura(){
  //si hay un escritor hay que esperar
  if(escribiendo){
    lectura.wait();
  }
  //si no lo hay aumento el numero de lectores
  n_lectores++;
  lectura.signal();
}


void Lec_Esc::fin_lectura(){
  //hay un lector menos
  n_lectores--;
  //si al irse este lector no queda ningun otro, se puede escribir
  if(n_lectores == 0){
    escritura.signal();
  }
}

void Lec_Esc::inicio_escritura(){
  //si hay lectores o hay un escritor se debe esperar
  if(n_lectores > 0 || escribiendo){
    escritura.wait();
  }
  //si no hay escribiendo ni lectores se puede escribir
  escribiendo = true;
}

void Lec_Esc::fin_escritura(){
  //ya no hay escritor
  escribiendo = false;
  //si hay lectores llamamos a uno, si no despertamos al escritor
  if(!lectura.empty()){
    lectura.signal();
  }
  else{
    escritura.signal();
  }
}





//-------------------------------------------------------------------------
// Función que simula la acción de leer
void leyendo( int lector ){
   // calcular milisegundos aleatorios de duración de la acción de leer)
   chrono::milliseconds duracion_leer( aleatorio<20,200>() );

   // informa de que comienza a leer

    cout << "Lector " << lector << "  :"
          << " empieza a leer (" << duracion_leer.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_leer' milisegundos
   this_thread::sleep_for( duracion_leer );

   // informa de que ha terminado de leer

    cout << "Lector " << lector << "  : termina de leer." << endl;

}

//-------------------------------------------------------------------------
// Función que simula la acción de escribir
void redactar( int escritor ){
   // calcular milisegundos aleatorios de duración de la acción de escribir)
   chrono::milliseconds duracion_escribir( aleatorio<20,200>() );

   // informa de que comienza a escribir

    cout << "Escritor " << escritor << "  :"
          << " empieza a escribir (" << duracion_escribir.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_escribir' milisegundos
   this_thread::sleep_for( duracion_escribir );

   // informa de que ha terminado de escribir

    cout << "Escritor " << escritor << "  : termina de escribir." << endl;

}






void funcion_hebra_lectura(MRef<Lec_Esc> monitor, int lector){
  while(true){
    std::cout << "Lector " << lector << " solicita la lectura." << '\n';
    monitor->inicio_lectura();
    leyendo(lector);
    monitor->fin_lectura();
  }
}


void funcion_hebra_escritura(MRef<Lec_Esc> monitor, int escritor){
  while (true) {
    std::cout << "Escritor " << escritor << " solicita la escritura." << '\n';
    monitor->inicio_escritura();
    redactar(escritor);
    monitor->fin_escritura();
  }
}



int main(){
  MRef<Lec_Esc> monitor = Create<Lec_Esc>();

  thread hebra_lectura[num_lectores], hebra_escritura[num_escritores];


  for (size_t i = 0; i < num_lectores; i++) {
    hebra_lectura[i] = thread(funcion_hebra_lectura, monitor, i);
  }
  for (size_t i = 0; i < num_escritores; i++) {
    hebra_escritura[i] = thread(funcion_hebra_escritura, monitor, i);
  }


  for (size_t i = 0; i < num_lectores; i++) {
    hebra_lectura[i].join();
  }
  for (size_t i = 0; i < num_escritores; i++) {
    hebra_escritura[i].join();
  }
}



















//
