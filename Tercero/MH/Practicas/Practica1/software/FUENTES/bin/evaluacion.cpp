#include "evaluacion.h"
#include <iostream>
#include <vector>
#include <cmath>

void evaluacion::inicializar(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  evaluaciones = 0;
  distancia_max = distanciaMaxima();
  n_restricciones = totalRestricciones();
}




int evaluacion::calcularRestricciones(vector<int> individuo){
   int no_cumplo = 0;
   for(int i = 0; i < restricciones.size(); i++){
      for(int j = i; j < restricciones[i].size(); j++){
         if(restricciones[i][j] == -1){
            if(individuo[i] == individuo[j])      //estan juntos, no deberían, sumo
               no_cumplo++;
         }
         if(restricciones[i][j] == 1){
            if(individuo[i] != individuo[j])      //no estan juntos, no deberían, sumo
               no_cumplo++;
         }
      }
   }
   return no_cumplo;
}




int evaluacion::totalRestricciones(){
   int violaciones = 0;
   for(int i = 0; i < restricciones.size(); i++){
      for(int j = i; j < restricciones[i].size(); j++){
         if(restricciones[i][j] == 1 || restricciones[i][j] == -1)
            violaciones++;
      }
   }
   return violaciones;
}



vector<int> evaluacion::elementosCadaCluster(vector<int> individuo){
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




vector<vector<double>> evaluacion::obtenerCentroide(vector<int> individuo){
   int cluster_actual = 0;
   vector<double> sumatoria;
   vector<double> sumas(datos[0].size(), 0.0);
   std::vector<int> total_por_cluster = elementosCadaCluster(individuo);
   vector<vector<double>> centroides;

   //for (size_t i = 0; i < dim; i++) sumatoria.push_back(0.0);

   do{
     for(int i = 0; i < individuo.size()-1; i++){
       if(individuo[i] == cluster_actual){
         for(int j = 0; j < datos[0].size(); j++){
           //obtengo la sumatoria de cada valor de datos recibidos
           sumas[j] += datos[i][j];
         }
       }
     }
     //realizo la division
     for(int i = 0; i < sumas.size(); i++){
       sumas[i] = sumas[i] / total_por_cluster[cluster_actual];
     }
     centroides.push_back(sumas);

     //paso al siguiente cluster
     cluster_actual++;
     for(int i = 0; i < datos[0].size(); i++){
       sumas[i] = 0.0;
    }
     //sumas = valoresCero(datos[0].size());
  }while(cluster_actual != n_cluster);

   return centroides;
}




double evaluacion::distanciaMedia(vector<vector<double>> centroides, vector<int> individuo){
   int cluster_actual = 0;
   vector<double> sumas;            //tendrá el tam de cluster que haya
   std::vector<int> total_por_cluster = elementosCadaCluster(individuo);
   vector<double> distancia_por_cluster;

   for (size_t i = 0; i < datos[0].size(); i++)
      sumas.push_back(0.0);

   for (size_t i = 0; i < n_cluster; i++) {
      distancia_por_cluster.push_back(0.0);
   }


   do{
      for(int i = 0; i < individuo.size()-1; i++){
         if(individuo[i] == cluster_actual){                        //asegurar que los datos son del cluster actual
            for(int j = 0; j < datos[0].size(); j++){
               sumas[j] += pow(centroides[cluster_actual][j] - datos[i][j], 2);
            }
         }
      }
      double distancia = 0;
      for(int k = 0; k < datos[0].size(); k++){
         distancia += sumas[k];
      }
      distancia = sqrt(distancia);
      distancia_por_cluster[cluster_actual] += distancia;

      distancia_por_cluster[cluster_actual] /= total_por_cluster[cluster_actual];

      for (size_t i = 0; i < datos[0].size(); i++)
         sumas[i] = 0.0;

      cluster_actual++;
   }while(cluster_actual != n_cluster);

  //Tendré que devolver el mayor valor en "distancia_por_cluster"
  double media = 0;
  for(int i = 0; i < distancia_por_cluster.size(); i++){
     media = media + distancia_por_cluster[i];
     //media k clusters
  }
  media = media / n_cluster;
  distancia_por_cluster.clear();
  return media;
}




double evaluacion::distanciaMaxima(){
   vector<double> sumas;            //tendrá el tam de cluster que haya
   double max = 0;

   for (size_t i = 0; i < datos[0].size(); i++)
      sumas.push_back(0.0);

int v;
int v_2;

   for(int i = 0; i < datos.size()-1; i++){
      for(int j = i + 1; j < datos.size()-1; j++){
         for(int k = 0; k < datos[0].size(); k++){
            sumas[k] += pow(datos[i][k] - datos[j][k], 2);
         }
         double distancia = 0;
         for(int k = 0; k < datos[0].size(); k++){
            distancia += sumas[k];
         }
         distancia = sqrt(distancia);
         if(distancia > max){
            max = distancia;
            v = i;
            v_2 = j;
            std::cout << "\nMax: " << max << '\n';
         }

         for (size_t i = 0; i < datos[0].size(); i++)
            sumas[i] = 0.0;
      }
   }

std::cout << "v1 " << v << '\n';
std::cout << "v2 " << v_2 << '\n';

   return max;
}





double evaluacion::funcionObjetivo(vector<int> individuo){
   evaluaciones++;

   double restricciones_incumplidas = calcularRestricciones(individuo);

   vector<vector<double>> centroides =  obtenerCentroide(individuo);

   double landa = getMax() / getRestricciones();

   double media = distanciaMedia(centroides, individuo);

   std::cout << "\nDistancia Maxima = " << distancia_max << '\n';

   double valoracion = media + (restricciones_incumplidas * landa);


   //almaceno resultados para la tabla
   Tasa_c = media;
   std::cout << "Tasa_c = " << Tasa_c << '\n';
   Tasa_inf = restricciones_incumplidas;
   std::cout << "Tasa_inf = " << Tasa_inf << '\n';
   std::cout << "Valoracion = " << valoracion << '\n';

   return valoracion;
}
