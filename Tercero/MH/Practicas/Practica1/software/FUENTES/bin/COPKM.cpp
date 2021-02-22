#include "COPKM.h"
#include <utility>
#include <cmath>
#include <iostream>
#include <list>

using namespace std;

COPKM::COPKM(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}



//Evito que haya cluster vacio, para ello miro todos y si hay una vacio le asigno
//ese cluster a un dato aleatorio
vector<int> COPKM::evitarClusterVacio(vector<int> solucion){
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
vector<int> COPKM::solucionInicial(){
   vector<int> solucion_inicial;
   size_t tamanio = datos.size();

   for(int i = 0; i < tamanio; i++){
      solucion_inicial.push_back(Randint(0, n_cluster - 1));
   }

   return evitarClusterVacio(solucion_inicial);
}



//recorro todos los valores y tomo las variables maximas y menores de cada uno
//asi obtengo los mayores posibles valores de los centroides y menores
void COPKM::maximoMinimo(){
   vector<double> max(datos[0].size(), -99999);
   vector<double> min(datos[0].size(), 99999);

   for(int i = 0; i < datos.size()-1; i++){
      for(int j = 0; j < datos[0].size(); j++){
         if(datos[i][j] > max[j]){
            max[j] = datos[i][j];
         }
         if(datos[i][j] < min[j]){
            min[j] = datos[i][j];
         }
      }
   }
   maximos_valores.clear();
   minimos_valores.clear();
   for(int i = 0; i < datos[0].size(); i++){
      maximos_valores.push_back(max[i]);
      minimos_valores.push_back(min[i]);
   }
}



bool COPKM::contiene(vector<int> v, int valor){
   for(int i = 0; i < v.size(); i++){
      if(v[i] == valor)
         return true;
   }
   return false;
}



//creo los centroides de forma aleatoria con valores complendidos
//entre el maximo y el minimo obtenido, cada cluster un centroide aleatorio
vector<vector<double>> COPKM::centroidesAleatorios(){
   centroides.clear();
   for (size_t i = 0; i < n_cluster; i++) {
      std::vector<double> aux;
      for (size_t j = 0; j < datos[0].size(); j++) {
         float rand = Randfloat(minimos_valores[j], maximos_valores[j]-1);
			aux.push_back(rand);
      }
      centroides.push_back(aux);
      aux.clear();
   }
}



//calculo las violaciones que supondría en el cluster
int COPKM::violacionesCluster(int dato, int cluster, vector<vector<double>> centroide){
   int violaciones = 0;
   for(int i = 0; i < restricciones[dato].size(); i++){
      if(restricciones[dato][i] == -1){
         if(centroide[cluster][i] == centroide[cluster][dato])      //estan juntos, no deberían, sumo
            violaciones++;
      }
      if(restricciones[dato][i] == 1){
         if(centroide[i] != centroide[dato])      //no estan juntos, no deberían, sumo
            violaciones++;
      }
   }
   return violaciones;
}



int COPKM::evaluarRestricciones(vector<int> solucion){
   int no_cumplo = 0;
   for(int i = 0; i < restricciones.size(); i++){
      for(int j = i; j < restricciones[i].size(); j++){
         if(restricciones[i][j] == -1){
            if(solucion[i] == solucion[j])      //estan juntos, no deberían, sumo
               no_cumplo++;
         }
         if(restricciones[i][j] == 1){
            if(solucion[i] != solucion[j])      //no estan juntos, no deberían, sumo
               no_cumplo++;
         }
      }
   }
   return no_cumplo;
}



int COPKM::probarCluster(vector<int> solucion, size_t dato, size_t cluster){
   solucion[dato] = cluster;
	return evaluarRestricciones(solucion);
}



vector<int> COPKM::elementosCadaCluster(vector<int> individuo){
  int cluster_actual = 0;
  vector<int> n_elementos(n_cluster, 0);

  do{
    for(int i = 0; i < individuo.size(); i++){
      if(individuo[i] == cluster_actual){
        n_elementos[cluster_actual]++;
      }
    }
    cluster_actual++;
  }while(cluster_actual != n_cluster);

  return n_elementos;
}



//obtengo la distancia del cluster al puntos
//empate_violaciones contiene los posibles clusters empatados
int COPKM::obtenerCentroideCercano(vector<int> empate_violaciones, int valor_actual, vector<int> solucion){
   double min = 99999;
   double diferencia, distancia;
   double sumatoria = 0;
   int cluster_cercano;
   valor_actual = solucion[valor_actual];

   for(int i = 0; i < n_cluster; i++){
      if(contiene(empate_violaciones, i)){
         for(int j = 0; j < datos[0].size(); j++){
            diferencia = pow((centroides[valor_actual][j] - datos[i][j]) , 2);
            sumatoria = sumatoria + diferencia;
         }
         distancia = sqrt(abs(sumatoria));
         sumatoria = 0;
         if(distancia < min){
             min = distancia;
             cluster_cercano = i;
         }
      }
   }
   return cluster_cercano;
}



void COPKM::greedy(){
   std::vector<int> solucion = solucionInicial();
   int contador = 1;
   bool diferencia = true;
   maximoMinimo();
   int dif = evaluarRestricciones(solucion);
   centroidesAleatorios();
   double distancia = 0;

   while (diferencia) {
      int restricciones;
      vector<int> empate_violaciones;
      for (size_t i = 0; i < datos.size(); i++) {
         int aux_max = 9999;
         bool vacio = false;
         for (size_t j = 0; j < n_cluster; j++) {
            std::vector<int> aux = solucion;
            restricciones = probarCluster(aux, i, j);
            if(restricciones < aux_max){
               empate_violaciones.clear();
               aux_max = restricciones;
               empate_violaciones.push_back(j);
            }
            else if(restricciones == aux_max){
               empate_violaciones.push_back(j);
            }
         }
         int cluster;
         if(empate_violaciones.size() > 1){
            std::vector<int> total_por_cluster = elementosCadaCluster(solucion);
            for(int k = 0; k < empate_violaciones.size(); k++){
               if(total_por_cluster[empate_violaciones[k]] == 0){
                  vacio = true;
               }
            }
            if(!vacio){
               cluster = obtenerCentroideCercano(empate_violaciones, i, solucion);
            }
            else{
               evitarClusterVacio(solucion);
            }
         }
         else{
            cluster = empate_violaciones[0];
         }
         empate_violaciones.clear();
         solucion[i] = cluster;
      }
   }
   if(evaluarRestricciones(solucion) == dif){
      diferencia = false;
   }
   else{
      dif = evaluarRestricciones(solucion);
   }
}




















//
