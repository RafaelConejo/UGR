#ifndef COPKM_H
#define COPKM_H

#include "random.h"
#include <algorithm>
#include <vector>
#include <evaluacion.h>
#include <random>

using namespace std;

class COPKM{
private:
   int n_cluster;
   vector<double> maximos_valores;
   vector<double> minimos_valores;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;

   vector<vector<double>> centroides;
   vector<int> indicesAleatorios;
   double maxima_distancia; //almacena la mayor distancia
   vector<int> total_por_cluster;   //total elemntos por cluster

   evaluacion Evalua;
   default_random_engine generator;

public:
   COPKM(int n, vector<vector<double>> valores, vector<vector<double>> restr);
   vector<int> evitarClusterVacio(vector<int> solucion);
   vector<int> solucionInicial();
   int evaluarRestricciones(vector<int> solucion);
   int probarCluster(vector<int> solucion, size_t dato, size_t cluster);
   vector<int> elementosCadaCluster(vector<int> individuo);
   int obtenerCentroideCercano(vector<int> empate_violaciones, int valor_actual, vector<int> solucion);
   int violacionesCluster(int dato, int cluster, vector<vector<double>> centroide);



   void maximoMinimo();
   vector<vector<double>> centroidesAleatorios();
   vector<vector<double>> obtenerCentroide(vector<int> asignaciones);
   bool contiene(vector<int> v, int valor);
   double mayorDistancia();
   int calcularViolaciones(vector<int> asignaciones_clusters);
   int totalViolaciones();
   double funcionValoracion(vector<vector<double>> centroides, vector<int> asignaciones_clusters);


   void greedy();

};

#endif
