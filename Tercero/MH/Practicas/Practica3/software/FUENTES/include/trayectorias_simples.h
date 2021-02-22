#ifndef TS_H
#define TS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"
#include "random.h"

using namespace std;

class trayectorias_simples{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;
   const double U = 0.3;
   const double TF = 0.001;
   const int EVALUACIONES = 100000;


   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   trayectorias_simples(int n, vector<vector<double>> valores, vector<vector<double>> restr);

   vector<int> evitarClusterVacio(vector<int> solucion);

   vector<int> solucionInicial();

   vector<int> cambioCluster(vector<int> solucion);

   double calcularTemperaturaInicial(double C);

   double calcularBeta(double T0, double M);

   double cauchyModificado(double T, double beta);

   vector<int> enfriamientoSimilado();

};

#endif
