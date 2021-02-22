#include "GSO.h"

GSO::GSO(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}



//Evito que haya cluster vacio, para ello miro todos y si hay una vacio le asigno
//ese cluster a un dato aleatorio
vector<double> GSO::evitarClusterVacio(vector<double> solucion){
   vector<double> comprobar;
   bool vacio = true;
   int tamanio = solucion.size();

   //inicilazo comprobar a 0
   for(size_t i = 0; i < n_cluster; i++)
      comprobar.push_back(0);

   //introduzco el elemento de cada cluster
   for (size_t i = 0; i < tamanio; i++)
      comprobar[solucion[i]]++;

   while (vacio) {
      for (size_t i = 0; i < n_cluster; i++) {
         if(comprobar[i] == 0){
            int elemento_aleatorio = Randint(0, tamanio - 1);
            int aux = solucion[elemento_aleatorio];

            solucion[elemento_aleatorio] = i;
            comprobar[i]++;
            comprobar[aux]--;
            break;
         }
         if(comprobar[i] != 0 && i == n_cluster - 1){
            vacio = false;
         }
      }
   }
   return solucion;
}


//Comenzamos creando una función que genere una solucion aleatoria
vector<double> GSO::inicioAleatorio(vector<double> &vector){

   for(int i = 0; i < vector.size(); i++){
      vector[i] = Randint(0, n_cluster - 1);
   }

   return evitarClusterVacio(vector);
}



vector<double> GSO::comprobarCluster(vector<double> vector){
   for(int i = 0; i < vector.size(); i++){
      if(vector[i] > n_cluster-1){
         vector[i] = n_cluster-1;
      }
      if(vector[i] < 0){
         vector[i] = 0;
      }
   }

   return evitarClusterVacio(vector);
}



vector<double> GSO::algoritmo_GSO(){
   vector<vector<vector<double>>> x(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> v1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> p1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<double>> g(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> v2(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> p2(M_galaxias, vector<double>(datos.size()));
   vector<double> mejor_g(datos.size());

   vector<vector<double>> valoracion_p1(M_galaxias, vector<double>(N_estrellas));
   vector<double> valoracion_p2(M_galaxias), valoracion_g(M_galaxias);
   double mejor_g_valoracion;

   //inicializamos todos los valores
   for(int i = 0; i < M_galaxias; i++){
      for(int j = 0; j < N_estrellas; j++){
         inicioAleatorio(x[i][j]);
         inicioAleatorio(v1[i][j]);
         inicioAleatorio(p1[i][j]);
         valoracion_p1[i][j] = Evalua.funcionObjetivo(p1[i][j]);
      }
   }

   for(int i = 0; i < M_galaxias; i++){
      inicioAleatorio(g[i]);
      inicioAleatorio(v2[i]);
      inicioAleatorio(p2[i]);
      valoracion_p2[i] = Evalua.funcionObjetivo(p2[i]);
      valoracion_g[i] = Evalua.funcionObjetivo(g[i]);
   }

   inicioAleatorio(mejor_g);
   mejor_g_valoracion = Evalua.funcionObjetivo(mejor_g);

   //Comenzamos a definir los niveles 1 y 2
   for(int epocas = 0; epocas < EP_max; epocas++){
      //PSO Nivel 1
      for(int i = 0; i < M_galaxias; i++){
         for(int k = 0; k <= L1; k++){
            double L1_div = 1.0 / (L1 + 1.0);
            double w1 = 1.0 - k*L1_div;

            for(int j = 0; j < N_estrellas; j++){
               double r1 = Randfloat(-1, 1);           //numero aleatorio entre -1 y 1
               double r2 = Randfloat(-1, 1);

               for(int d = 0; d < datos.size(); d++){
                  v1[i][j][d] = w1*v1[i][j][d] + c1*r1*(p1[i][j][d] - x[i][j][d]) + c2*r2*(g[i][d] - x[i][j][d]);
                  //Evitar truncar, añado 0.5 si es mayor que 0 o quito 0.5 si es menor.
                  v1[i][j][d] += 0.5 - (v1[i][j][d] < 0);
                  x[i][j][d] = (int) x[i][j][d] + (int) v1[i][j][d];
               }
               //tengo que comprobar que el valor generado esta dentro del dominio de cluster permitido
               x[i][j] = comprobarCluster(x[i][j]);
               double nueva_valoracion = Evalua.funcionObjetivo(x[i][j]);

               //Comienzo a actualizar nuevas valoraciones si es el caso
               if(nueva_valoracion < valoracion_p1[i][j]){
                  for(int d = 0; d < datos.size(); d++){
                     p1[i][j][d] = x[i][j][d];
                  }
                  valoracion_p1[i][j] = nueva_valoracion;

                  if(nueva_valoracion < valoracion_g[i]){
                     for(int d = 0; d < datos.size(); d++){
                        g[i][d] = p1[i][j][d];
                     }
                     valoracion_g[i] = nueva_valoracion;

                     if(nueva_valoracion < mejor_g_valoracion){
                        for(int d = 0; d < datos.size(); d++){
                           mejor_g[d] = g[i][d];
                        }
                        mejor_g_valoracion = nueva_valoracion;
                     }
                  }
               }
            }
         }
      }
      //PSO Nivel 2
      vector<vector<double>> y(g);

      for(int k = 0; k <= L2; k++){
         double L2_div = 1.0 / (L2 + 1.0);
         double w2 = 1.0 - k*L2_div;

         for(int i = 0; i < M_galaxias; i++){
            double r3 = Randfloat(-1, 1);           //numero aleatorio entre -1 y 1
            double r4 = Randfloat(-1, 1);

            for(int d = 0; d < datos.size(); d++){
               v2[i][d] = w2*v2[i][d] + c3*r3*(p2[i][d] - y[i][d]) + c4*r4*(mejor_g[d] - y[i][d]);
               //Evitar truncar
               v2[i][d] += 0.5 - (v2[i][d] < 0);
               y[i][d] = (int) y[i][d] + (int) v2[i][d];
            }
            //tengo que comprobar que el valor generado esta dentro del dominio de cluster permitido
            y[i] = comprobarCluster(y[i]);
            double nueva_valoracion = Evalua.funcionObjetivo(y[i]);

            if(nueva_valoracion < valoracion_p2[i]){
               for(int d = 0; d < datos.size(); d++){
                  p2[i][d] = y[i][d];
               }
               valoracion_p2[i] = nueva_valoracion;

               if(nueva_valoracion < mejor_g_valoracion){
                  for(int d = 0; d < datos.size(); d++){
                     mejor_g[d] = p2[i][d];
                  }
                  mejor_g_valoracion = nueva_valoracion;
               }
            }
         }
      }
   }
   double mejor_valoracion = Evalua.funcionObjetivo(mejor_g);
   return mejor_g;
}


vector<double> GSO::cambioCluster(vector<double> solucion, int pos){
   int nuevo_cluster = Randint(0, n_cluster - 1);

   while(nuevo_cluster == solucion[pos]){
      nuevo_cluster = Randint(0, n_cluster - 1);
   }
   solucion[pos] = nuevo_cluster;

   return evitarClusterVacio(solucion);
}


//Paramos si tras 20 * datos no encontramos mejora
vector<double> GSO::busquedaLocal(vector<double> solucion_actual){
   const int tamanio = datos.size();
   vector<int> indices;
   int evaluaciones = 0;
   int vecinos = 0;
   bool mejora = false;
   //valoro la solucion aleatoria dada
   double valoracion_actual = Evalua.funcionObjetivo(solucion_actual);
   //mejores casos
   vector<double> mejor_solucion = solucion_actual;
   double mejor_valoracion = valoracion_actual;
   //soluciones nuevas
   double nueva_valoracion;
   vector<double> nueva_solucion;

   //Inicializo el vector de indices
   for (size_t i = 0; i < tamanio; i++) {
      indices.push_back(i);
   }

   while(evaluaciones < EVALUACIONES && vecinos < tamanio * (n_cluster-1)){
      //Selecciono el elemento
      if(evaluaciones % tamanio == 0 || mejora){
         shuffle(indices.begin(), indices.end(), generator);
         mejora = false;
      }
      int elemento = indices[evaluaciones % tamanio];
      //realizo el cambio en ese elemento
      nueva_solucion = cambioCluster(solucion_actual, elemento);
      nueva_valoracion = Evalua.funcionObjetivo(nueva_solucion);
      evaluaciones++;

      if(nueva_valoracion < mejor_valoracion){
         solucion_actual = nueva_solucion;
         mejor_solucion = nueva_solucion;
         mejor_valoracion = nueva_valoracion;
         //hay mejora en la valoracion
         mejora = true;
         vecinos = 0;
      }

      vecinos++;

   }
   return mejor_solucion;
}



vector<double> GSO::algoritmo_GSO_BL(){
   vector<vector<vector<double>>> x(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> v1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> p1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<double>> g(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> v2(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> p2(M_galaxias, vector<double>(datos.size()));
   vector<double> mejor_g(datos.size());

   vector<vector<double>> valoracion_p1(M_galaxias, vector<double>(N_estrellas));
   vector<double> valoracion_p2(M_galaxias), valoracion_g(M_galaxias);
   double mejor_g_valoracion;

   //inicializamos todos los valores
   for(int i = 0; i < M_galaxias; i++){
      for(int j = 0; j < N_estrellas; j++){
         inicioAleatorio(x[i][j]);
         inicioAleatorio(v1[i][j]);
         inicioAleatorio(p1[i][j]);
         valoracion_p1[i][j] = Evalua.funcionObjetivo(p1[i][j]);
      }
   }

   for(int i = 0; i < M_galaxias; i++){
      inicioAleatorio(g[i]);
      inicioAleatorio(v2[i]);
      inicioAleatorio(p2[i]);
      valoracion_p2[i] = Evalua.funcionObjetivo(p2[i]);
      valoracion_g[i] = Evalua.funcionObjetivo(g[i]);
   }

   inicioAleatorio(mejor_g);
   mejor_g_valoracion = Evalua.funcionObjetivo(mejor_g);

   //Comenzamos a definir los niveles 1 y 2
   for(int epocas = 0; epocas < EP_max; epocas++){
      //PSO Nivel 1
      for(int i = 0; i < M_galaxias; i++){
         for(int k = 0; k <= L1; k++){
            double L1_div = 1.0 / (L1 + 1.0);
            double w1 = 1.0 - k*L1_div;

            for(int j = 0; j < N_estrellas; j++){
               double r1 = Randfloat(-1, 1);           //numero aleatorio entre -1 y 1
               double r2 = Randfloat(-1, 1);

               for(int d = 0; d < datos.size(); d++){
                  v1[i][j][d] = w1*v1[i][j][d] + c1*r1*(p1[i][j][d] - x[i][j][d]) + c2*r2*(g[i][d] - x[i][j][d]);
                  //Evitar truncar, añado 0.5 si es mayor que 0 o quito 0.5 si es menor.
                  v1[i][j][d] += 0.5 - (v1[i][j][d] < 0);
                  x[i][j][d] = (int) x[i][j][d] + (int) v1[i][j][d];
               }
               //tengo que comprobar que el valor generado esta dentro del dominio de cluster permitido
               x[i][j] = comprobarCluster(x[i][j]);
               double nueva_valoracion = Evalua.funcionObjetivo(x[i][j]);

               //Comienzo a actualizar nuevas valoraciones si es el caso
               if(nueva_valoracion < valoracion_p1[i][j]){
                  for(int d = 0; d < datos.size(); d++){
                     p1[i][j][d] = x[i][j][d];
                  }
                  valoracion_p1[i][j] = nueva_valoracion;

                  if(nueva_valoracion < valoracion_g[i]){
                     for(int d = 0; d < datos.size(); d++){
                        g[i][d] = p1[i][j][d];
                     }
                     valoracion_g[i] = nueva_valoracion;

                     if(nueva_valoracion < mejor_g_valoracion){
                        for(int d = 0; d < datos.size(); d++){
                           mejor_g[d] = g[i][d];
                        }
                        mejor_g_valoracion = nueva_valoracion;
                     }
                  }
               }
            }
         }
      }
      //PSO Nivel 2
      vector<vector<double>> y(g);

      for(int i = 0; i < M_galaxias; i++){

         y[i] = busquedaLocal(y[i]);

         for(int d = 0; d < datos.size(); d++){
            p2[i][d] = y[i][d];
         }
         valoracion_p2[i] = Evalua.funcionObjetivo(y[i]);


         if(valoracion_p2[i] < mejor_g_valoracion){
            for(int d = 0; d < datos.size(); d++){
               mejor_g[d] = p2[i][d];
            }
            mejor_g_valoracion = valoracion_p2[i];
         }
      }
   }
   double mejor_valoracion = Evalua.funcionObjetivo(mejor_g);
   return mejor_g;
}






bool GSO::extraerVector(vector<double> &vector, int buscado){
   for(std::vector<double>::iterator it = vector.begin(); it != vector.end(); it++){
      if((*it) == buscado){
         vector.erase(it);
         return true;
      }
   }
   return false;
}



bool GSO::contiene(vector<double> v, int valor){
   for(int i = 0; i < v.size(); i++){
      if(v[i] == valor)
         return true;
   }
   return false;
}



vector<double> GSO::RandVector(vector<double> v, int tamanio){
   std::vector<double> aux;
   std::vector<double> aux_2;
   std::vector<double> indices;

   //caso de que el 10 por ciento sea mayor que lo que queda
   if(v.size() < tamanio){
      tamanio = v.size();
   }

   for(size_t i = 0; i < v.size(); i++){
      indices.push_back(i);
   }

   for (size_t i = 0; i < tamanio; i++){
      int indice = Randint(0, indices.size() - 1);
      while(contiene(aux_2, indice)){
         indice = Randint(0, indices.size() - 1);
      }
      aux_2.push_back(indice);
      aux.push_back(v[indice]);
   }
   return aux;
}


vector<double> GSO::mutarBrusco(vector<double> solucion){
   int numero_mutaciones = PORCENTAJE_MUTA * datos.size();
   int tamanio = solucion.size();
   //genero r y v
   int inicio = Randint(0, tamanio - 1);
   int tamanio_segmento = Randint(0, tamanio - 1);
   vector<double> resto;
   vector<double> salida;
   vector<double> posicion_segmento;

   //tomo el segmento que no se modifica
   for (size_t i = 0; i < tamanio_segmento; i++) {
      posicion_segmento.push_back((inicio + i) % tamanio);
   }
   //lleno el vector de salida
   for (size_t i = 0; i < tamanio; i++) {
      salida.push_back(10);
   }

   for (size_t i = 0; i < tamanio; i++) {
      if(extraerVector(posicion_segmento, i))
         salida[i] = solucion[i];

      else
         resto.push_back(i);
   }
   //ahora debo asignar aleatoriamente el resto, solo un 10%
   vector<double> seleccion = RandVector(resto, numero_mutaciones);

   for (size_t i = 0; i < tamanio; i++) {
      if(extraerVector(resto, i)){
         if(extraerVector(seleccion, i)){
            int cluster_actual = solucion[i];
            int nuevo_cluster = Randint(0, n_cluster - 1);

            while (cluster_actual == nuevo_cluster) {
               nuevo_cluster = Randint(0, n_cluster - 1);
            }
            salida[i] = nuevo_cluster;
         }
         else
            salida[i] = solucion[i];
      }
   }
  return evitarClusterVacio(salida);
}


vector<double> GSO::algoritmo_GSO_BL_mutacion(){
   vector<vector<vector<double>>> x(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> v1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<vector<double>>> p1(M_galaxias, vector< vector<double> >(N_estrellas, vector<double>(datos.size())));
   vector<vector<double>> g(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> v2(M_galaxias, vector<double>(datos.size()));
   vector<vector<double>> p2(M_galaxias, vector<double>(datos.size()));
   vector<double> mejor_g(datos.size());

   vector<vector<double>> valoracion_p1(M_galaxias, vector<double>(N_estrellas));
   vector<double> valoracion_p2(M_galaxias), valoracion_g(M_galaxias);
   double mejor_g_valoracion;

   //inicializamos todos los valores
   for(int i = 0; i < M_galaxias; i++){
      for(int j = 0; j < N_estrellas; j++){
         inicioAleatorio(x[i][j]);
         inicioAleatorio(v1[i][j]);
         inicioAleatorio(p1[i][j]);
         valoracion_p1[i][j] = Evalua.funcionObjetivo(p1[i][j]);
      }
   }

   for(int i = 0; i < M_galaxias; i++){
      inicioAleatorio(g[i]);
      inicioAleatorio(v2[i]);
      inicioAleatorio(p2[i]);
      valoracion_p2[i] = Evalua.funcionObjetivo(p2[i]);
      valoracion_g[i] = Evalua.funcionObjetivo(g[i]);
   }

   inicioAleatorio(mejor_g);
   mejor_g_valoracion = Evalua.funcionObjetivo(mejor_g);

   //Comenzamos a definir los niveles 1 y 2
   for(int epocas = 0; epocas < EP_max; epocas++){
      //PSO Nivel 1
      for(int i = 0; i < M_galaxias; i++){

            for(int j = 0; j < N_estrellas; j++){
               x[i][j] = mutarBrusco(x[i][j]);
               double nueva_valoracion = Evalua.funcionObjetivo(x[i][j]);

               //Comienzo a actualizar nuevas valoraciones si es el caso
               if(nueva_valoracion < valoracion_p1[i][j]){
                  for(int d = 0; d < datos.size(); d++){
                     p1[i][j][d] = x[i][j][d];
                  }
                  valoracion_p1[i][j] = nueva_valoracion;

                  if(nueva_valoracion < valoracion_g[i]){
                     for(int d = 0; d < datos.size(); d++){
                        g[i][d] = p1[i][j][d];
                     }
                     valoracion_g[i] = nueva_valoracion;

                     if(nueva_valoracion < mejor_g_valoracion){
                        for(int d = 0; d < datos.size(); d++){
                           mejor_g[d] = g[i][d];
                        }
                        mejor_g_valoracion = nueva_valoracion;
                     }
                  }
               }

         }
      }
      //PSO Nivel 2
      vector<vector<double>> y(g);

      for(int i = 0; i < M_galaxias; i++){

         y[i] = busquedaLocal(y[i]);

         for(int d = 0; d < datos.size(); d++){
            p2[i][d] = y[i][d];
         }
         valoracion_p2[i] = Evalua.funcionObjetivo(y[i]);


         if(valoracion_p2[i] < mejor_g_valoracion){
            for(int d = 0; d < datos.size(); d++){
               mejor_g[d] = p2[i][d];
            }
            mejor_g_valoracion = valoracion_p2[i];
         }
      }
   }
   double mejor_valoracion = Evalua.funcionObjetivo(mejor_g);
   return mejor_g;
}














//
