#ifndef AM_H
#define AM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"


using namespace std;

class AM{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;

   const int TAM_POBLACION_AM = 10;
   const int EVALUACIONES = 100000;
   const double PROBABILIDAD_CROMOSOMA_AM = 0.7;
   const double PROBABILIDAD_GEN_AM = 0.001;
   const double MAX_FALLOS = 0.1;


   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   AM(int n, vector<vector<double>> valores, vector<vector<double>> restr);
   bool contiene(vector<size_t> v, int valor);

   //Devuelve un vector con valores aleatorios partiendo de un vector desordenado
   vector<size_t> RandVector(vector<size_t> v, int tamanio);

   //Selecciona
   //  modo -1: los dos peores cromosomas
   //  modo  0: el peor cromosoma
   //  modo  1: el mejor cromosoma
   size_t reemplazamiento(vector<vector<size_t>> poblacion, int modo);

   //Genero la poblacion para los algoritmos geneticos
   vector<size_t> generarPoblacion (int modo);

   vector<size_t> evitarClusterVacio(vector<size_t> hijo);

   vector<size_t> cruceUniforme(vector<size_t> padre_1, vector<size_t> padre_2);

   vector<size_t> torneoBinario(vector<vector<size_t>> poblacion);

   bool dado(double probabilidad);

   bool extraerVector(vector<size_t> &vector, size_t buscado);

   vector<size_t> cruceSegmentoFijo(vector<size_t> padre_1, vector<size_t> padre_2);

   vector<vector<size_t>> cruzarMejor(vector<vector<size_t>> poblacion, vector<size_t> &posiciones);

   vector<size_t> mutacionUniforme(vector<size_t> hijo);

   vector<vector<size_t>> mutarAM(vector<vector<size_t>> poblacion, vector<size_t> & posiciones);

   vector<vector<size_t>> RandVector(vector<vector<size_t>> v, int tamanio);

   vector<size_t> mejorMutacionUniforme(vector<size_t> hijo);

   vector<size_t> busquedaLocalSuave(vector<size_t> individuo);

   vector<vector<size_t>> optimizar(vector<vector<size_t>> poblacion, int modo);

   double evaluarPoblacion(vector<vector<size_t>> poblacion);

   vector<vector<size_t>> algoritmoMenetico(vector<vector<size_t>> poblacion, int modo);

   size_t getValoracionFinal(){
      return valoracion_final;
   }

   double getTasa_c(){
      return Tasa_c;
   }

   double getTasa_inf(){
      return Tasa_inf;
   }

};

#endif
