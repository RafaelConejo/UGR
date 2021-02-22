#ifndef UTILES_H
#define UTILES_H

#include "random.h"
#include <algorithm>
#include <vector>
#include <evaluacion.h>
#include <random>

using namespace std;

class busquedaLocal{
private:
  int n_cluster;
  vector<vector<double>> datos;
  vector<vector<double>> restricciones;
  vector<int> clusters;
  vector<vector<double>> centroides;
  double media;
  vector<int> total_por_cluster;     //numero elementos por cluster

  evaluacion Evalua;
  const int EVALUACIONES = 100000;
  const int LIMITE_MEJORA = 20;
  default_random_engine generator;

  double n_restricciones;
  double distancia_max;


public:
  busquedaLocal(int n, vector<vector<double>> valores, vector<vector<double>> restr);

  vector<int> evitarClusterVacio(vector<int> solucion);

  vector<int> solucionInicial();

  vector<int> cambioCluster(vector<int> solucion, int pos);

  vector<int> algoritmoBusquedaLocal();
};

#endif
