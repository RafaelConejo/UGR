#ifndef TM_H
#define TM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"
#include "random.h"
#include <random>

using namespace std;

class trayectorias_multiples{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;
   const int EVALUACIONES = 10000;
   const int LIMITE_MEJORA = 20;
   const int BMB_ITERACIONES = 10;
   const double PORCENTAJE_MUTA = 0.1;
   const int ILS_ITERACIONES = 9;

   default_random_engine generator;


   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   trayectorias_multiples(int n, vector<vector<double>> valores, vector<vector<double>> restr);

   vector<int> evitarClusterVacio(vector<int> solucion);

   vector<int> solucionInicial();

   vector<int> cambioCluster(vector<int> solucion, int pos);

   vector<int> busquedaLocal(vector<int> solucion);

   vector<int> busquedaMultiarranqueBasica();

   bool extraerVector(vector<int> &vector, int buscado);

   bool contiene(vector<int> v, int valor);

   vector<int> RandVector(vector<int> v, int tamanio);

   vector<int> mutarBrusco(vector<int> solucion);

   vector<int> busquedaLocalReiterada();

};

#endif
