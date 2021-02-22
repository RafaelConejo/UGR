#ifndef GSO_H
#define GSO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"
#include "random.h"
#include <random>

using namespace std;

class GSO{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;
   const int M_galaxias = 20;
   const int N_estrellas = 10;
   const int L1 = 450;
   const int L2 = 4500;
   const int EP_max = 10;          //max numero de epocas
   const double c1 = 2.5;         // coeficientes de aceleracion
   const double c2 = 2.5;
   const double c3 = 2.5;
   const double c4 = 2.5;
   //evaluaciones de la BL
   const int EVALUACIONES = 7000;

   default_random_engine generator;

   const double PORCENTAJE_MUTA = 0.1;


   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   GSO(int n, vector<vector<double>> valores, vector<vector<double>> restr);

   vector<double> evitarClusterVacio(vector<double> solucion);

   vector<double> inicioAleatorio(vector<double> &vector);

   vector<double> comprobarCluster(vector<double> vector);

   vector<double> algoritmo_GSO();

   vector<double> cambioCluster(vector<double> solucion, int pos);

   vector<double> busquedaLocal(vector<double> solucion_actual);

   vector<double> algoritmo_GSO_BL();

   bool extraerVector(vector<double> &vector, int buscado);

   bool contiene(vector<double> v, int valor);

   vector<double> RandVector(vector<double> v, int tamanio);

   vector<double> mutarBrusco(vector<double> solucion);

   vector<double> algoritmo_GSO_BL_mutacion();

};

#endif
