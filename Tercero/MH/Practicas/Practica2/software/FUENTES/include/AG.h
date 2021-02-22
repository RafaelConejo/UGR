#ifndef AG_H
#define AG_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "evaluacion.h"


using namespace std;

class AG{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;
   const int TAM_POBLACION_AG = 50;
   const int EVALUACIONES = 100000;
   const int PADRES_ESTACIONARIO = 2;
   const int PADRES_GENERACIONAL = 50;
   const double PROBABILIDAD_CRUCE_AGE = 1;
   const double PROBABILIDAD_CRUCE_AGG = 0.7;
   const double PROBABILIDAD_MUTACION_AG = 0.001;

   evaluacion Evalua;
   double valoracion_final;
   double Tasa_c;
   double Tasa_inf;

public:
   AG(int n, vector<vector<double>> valores, vector<vector<double>> restr);
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

   //Si un cluster tiene 0 elementos asignados le asigna un elemento aleatorio
   vector<size_t> evitarClusterVacio(vector<size_t> hijo);

   bool extraerVector(vector<size_t> &vector, size_t buscado);

   //genera un nuevo hijo en base a dos padres, tomando la mitad de genes de un padre y la mitad del otro
   vector<size_t> cruceUniforme(vector<size_t> padre_1, vector<size_t> padre_2);

   //Genero un nuevo hijo copiando del primer padre los genes con los índices en el intervalo {r, ((r + v) mod n) − 1}
   //y el resto de genes por cruce uniforme
   vector<size_t> cruceSegmentoFijo(vector<size_t> padre_1, vector<size_t> padre_2);

   vector<size_t> mutacionUniforme(vector<size_t> hijo);

   vector<size_t> torneoBinario(vector<vector<size_t>> poblacion);

   bool dado(double probabilidad);

   vector<vector<size_t>> cruzarEstacionario(vector<vector<size_t>> poblacion, int modo);

   vector<vector<size_t>> mutarAG(vector<vector<size_t>> poblacion, vector<size_t> & posiciones);

   vector<vector<size_t>> cruzarGeneracional(vector<vector<size_t>> poblacion, int modo, vector<size_t> & posiciones);

   double evaluarPoblacion(vector<vector<size_t>> poblacion);

   vector<vector<size_t>> algoritmoGenetico(vector<vector<size_t>> poblacion, int modo);

   double getValoracionFinal(){
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
