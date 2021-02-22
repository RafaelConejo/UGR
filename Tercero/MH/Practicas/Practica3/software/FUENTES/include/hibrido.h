#ifndef HI_H
#define HI_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"
#include "random.h"

using namespace std;

class hibrido{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;
   const int EVALUACIONES = 10000;
   const double U = 0.3;
   const double TF = 0.001;
   const double PORCENTAJE_MUTA = 0.1;
   const int ILS_ITERACIONES = 9;


   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   hibrido(int n, vector<vector<double>> valores, vector<vector<double>> restr);

   vector<int> evitarClusterVacio(vector<int> solucion);

   vector<int> solucionInicial();

   vector<int> cambioCluster(vector<int> solucion);

   vector<int> enfriamientoSimilado(vector<int> solucion_actual);

   bool extraerVector(vector<int> &vector, int buscado);

   bool contiene(vector<int> v, int valor);

   vector<int> RandVector(vector<int> v, int tamanio);

   vector<int> mutarBrusco(vector<int> solucion);

   vector<int> ILS_ES();

};

#endif
