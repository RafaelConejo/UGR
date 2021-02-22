#include "busquedaLocal.h"
#include <iostream>
#include <cmath>
#include <algorithm>

busquedaLocal::busquedaLocal(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}



//Evito que haya cluster vacio, para ello miro todos y si hay una vacio le asigno
//ese cluster a un dato aleatorio
vector<int> busquedaLocal::evitarClusterVacio(vector<int> solucion){
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
vector<int> busquedaLocal::solucionInicial(){
   vector<int> solucion_inicial;
   size_t tamanio = datos.size();

   for(int i = 0; i < tamanio; i++){
      solucion_inicial.push_back(Randint(0, n_cluster - 1));
   }

   return evitarClusterVacio(solucion_inicial);
}



vector<int> busquedaLocal::cambioCluster(vector<int> solucion, int pos){
   int nuevo_cluster = Randint(0, n_cluster - 1);

   while(nuevo_cluster == solucion[pos]){
      nuevo_cluster = Randint(0, n_cluster - 1);
   }
   solucion[pos] = nuevo_cluster;

   return evitarClusterVacio(solucion);
}



vector<int> busquedaLocal::algoritmoBusquedaLocal(){
   const int tamanio = datos.size();
   vector<int> indices;
   int evaluaciones = 0;
   int vecinos = 0;
   bool mejora = false;
   //valoro la solucion aleatoria dada
   vector<int> solucion_actual = solucionInicial();
   double valoracion_actual = Evalua.funcionObjetivo(solucion_actual);
   //mejores casos
   vector<int> mejor_solucion = solucion_actual;
   double mejor_valoracion = valoracion_actual;
   //soluciones nuevas
   double nueva_valoracion;
   vector<int> nueva_solucion;

   //Inicializo el vector de indices
   for (size_t i = 0; i < tamanio; i++) {
      indices.push_back(i);
   }
   shuffle(indices.begin(), indices.end(), generator);

   while(evaluaciones < EVALUACIONES && vecinos < tamanio * (n_cluster)){
      //Selecciono el elemento
      int elemento = indices[evaluaciones % tamanio];
      //realizo el cambio en ese elemento
      nueva_solucion = cambioCluster(solucion_actual, elemento);
      nueva_valoracion = Evalua.funcionObjetivo(nueva_solucion);
      evaluaciones++;

      if(nueva_valoracion < mejor_valoracion){
         solucion_actual = nueva_solucion;
         mejor_solucion = nueva_solucion;
         mejor_valoracion = nueva_valoracion;
         //hay mejora en la valoracion
         mejora = true;
         vecinos = 0;
      }

      if(evaluaciones % tamanio == 0 || mejora){
         shuffle(indices.begin(), indices.end(), generator);
         mejora = false;
      }
      else{
         vecinos++;
      }
   }
   Evalua.funcionObjetivo(mejor_solucion);
   return mejor_solucion;
}
