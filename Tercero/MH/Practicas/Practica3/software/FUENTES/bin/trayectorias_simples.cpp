#include "trayectorias_simples.h"
#include <math.h>

trayectorias_simples::trayectorias_simples(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}


//Evito que haya cluster vacio, para ello miro todos y si hay una vacio le asigno
//ese cluster a un dato aleatorio
vector<int> trayectorias_simples::evitarClusterVacio(vector<int> solucion){
   vector<int> comprobar;
   bool vacio = true;
   int tamanio = solucion.size();

   //inicilazo comprobar a 0
   for(size_t i = 0; i < n_cluster; i++)
      comprobar.push_back(0);

   //introduzco el elemento de cada cluster
   for (size_t i = 0; i < tamanio; i++)
      comprobar[solucion[i]]++;

   while (vacio) {
      for (size_t i = 0; i < n_cluster; i++) {
         if(comprobar[i] == 0){
            int elemento_aleatorio = Randint(0, tamanio - 1);
            int aux = solucion[elemento_aleatorio];

            solucion[elemento_aleatorio] = i;
            comprobar[i]++;
            comprobar[aux]--;
            break;
         }
         if(comprobar[i] != 0 && i == n_cluster - 1){
            vacio = false;
         }
      }
   }
   return solucion;
}


//Comenzamos creando una función que genere una solucion aleatoria
vector<int> trayectorias_simples::solucionInicial(){
   vector<int> solucion_inicial;
   size_t tamanio = datos.size();

   for(int i = 0; i < tamanio; i++){
      solucion_inicial.push_back(Randint(0, n_cluster - 1));
   }

   return evitarClusterVacio(solucion_inicial);
}



vector<int> trayectorias_simples::cambioCluster(vector<int> solucion){
   int nuevo_cluster = Randint(0, n_cluster - 1);
   int pos = Randint(0, solucion.size() - 1);

   while(nuevo_cluster == solucion[pos]){
      nuevo_cluster = Randint(0, n_cluster - 1);
   }
   solucion[pos] = nuevo_cluster;

   return evitarClusterVacio(solucion);
}



double trayectorias_simples::calcularTemperaturaInicial(double C){
   double T0 = (U * (C)) / -log(U);
   return T0;
}



double trayectorias_simples::calcularBeta(double T0, double M){
   double beta = (T0 - TF) / (M * T0 * TF);
   return beta;
}



double trayectorias_simples::cauchyModificado(double T, double beta){
   return (T / (1 + beta * T));
}



vector<int> trayectorias_simples::enfriamientoSimilado(){
   int evaluaciones = 0;
   int vecinos;
   vector<int> solucion_actual = solucionInicial();
   double valoracion_actual = Evalua.funcionObjetivo(solucion_actual);
   //inicilizo la mejor solucion
   vector<int> mejor_solucion = solucion_actual;
   double mejor_valoracion = valoracion_actual;
   //duda si deberia dividirlo entre 100 y restarle 1
   double T0 = calcularTemperaturaInicial(valoracion_actual);
   double T = T0;

   //compruebo que la temperatura inicial es mayor que la final
   while (TF > T) {
      T = T * TF;
   }

   double max_vecinos = 10 * datos.size();
   double max_exitos = 0.1 * max_vecinos;
   double M = EVALUACIONES / max_vecinos;
   double beta = calcularBeta(T0, M);
   int exitos = 1;

   while (exitos != 0 && T > TF) {
      exitos = 0;
      vecinos = 0;

      while (exitos < max_exitos && vecinos < max_vecinos && evaluaciones < EVALUACIONES) {
         vector<int> nueva_solucion = cambioCluster(solucion_actual);
         double nueva_valoracion = Evalua.funcionObjetivo(nueva_solucion);

         evaluaciones++;
         vecinos++;

         double diferencia = nueva_valoracion - valoracion_actual;
         if(diferencia < 0 || Rand() <= exp(-diferencia / T)){
            exitos++;
            solucion_actual = nueva_solucion;
            valoracion_actual = nueva_valoracion;

            if(valoracion_actual < mejor_valoracion){
               mejor_solucion = solucion_actual;
               mejor_valoracion = valoracion_actual;
            }
         }

      }
      //Esquema de enfriamiento de Cauchy
      T = cauchyModificado(T, beta);
   }
   //La ejecuto de nuevo para implimir los valores
   Evalua.funcionObjetivo(mejor_solucion);
   //
   return mejor_solucion;
}












//
