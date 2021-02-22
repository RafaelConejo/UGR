#ifndef EVALUACION_H
#define EVALUACION_H


#include <vector>
#include <cmath>

using namespace std;

class evaluacion{
private:
   int n_cluster;
   vector<vector<double>> datos;
   vector<vector<double>> restricciones;

   int evaluaciones;
   double distancia_max;
   double n_restricciones;

   double Tasa_c;
   double Tasa_inf;

public:
   evaluacion(){
      evaluaciones = 0;
   }

   void inicializar(int n, vector<vector<double>> valores, vector<vector<double>> restr);

   int calcularRestricciones(vector<int> individuo);

   int totalRestricciones();

   vector<vector<double>> obtenerCentroide(vector<int> individuo);

   double mayorDistancia(vector<vector<double>> centroides, vector<int> individuo);

   double distanciaMedia(vector<vector<double>> centroides, vector<int> individuo);

   double distanciaMaxima();

   double funcionObjetivo(vector<int> individuo);


   int getEvaluaciones(){
      return evaluaciones;
   }

   double getTasa_c(){
      return Tasa_c;
   }

   double getTasa_inf(){
      return Tasa_inf;
   }

   double getMax(){
      return distancia_max;
   }

   double getRestricciones(){
      return n_restricciones;
   }

   //Numero total de elementos en cada cluster
   vector<int> elementosCadaCluster(vector<int> individuo);


};
#endif
