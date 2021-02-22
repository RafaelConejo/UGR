#include "hibrido.h"
#include "set"

hibrido::hibrido(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}



//Evito que haya cluster vacio, para ello miro todos y si hay una vacio le asigno
//ese cluster a un dato aleatorio
vector<int> hibrido::evitarClusterVacio(vector<int> solucion){
   vector<int> comprobar;
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
vector<int> hibrido::solucionInicial(){
   vector<int> solucion_inicial;
   size_t tamanio = datos.size();

   for(int i = 0; i < tamanio; i++){
      solucion_inicial.push_back(Randint(0, n_cluster - 1));
   }

   return evitarClusterVacio(solucion_inicial);
}



vector<int> hibrido::cambioCluster(vector<int> solucion){
   int nuevo_cluster = Randint(0, n_cluster - 1);
   int pos = Randint(0, solucion.size() - 1);

   while(nuevo_cluster == solucion[pos]){
      nuevo_cluster = Randint(0, n_cluster - 1);
   }
   solucion[pos] = nuevo_cluster;

   return evitarClusterVacio(solucion);
}




vector<int> hibrido::enfriamientoSimilado(vector<int> solucion_actual){
   int evaluaciones = 0;
   int vecinos;
   double valoracion_actual = Evalua.funcionObjetivo(solucion_actual);
   //inicilizo la mejor solucion
   vector<int> mejor_solucion = solucion_actual;
   double mejor_valoracion = valoracion_actual;
   //duda si deberia dividirlo entre 100 y restarle 1
   double T0 = (U * (valoracion_actual)) / -log(U);
   double T = T0;

   //compruebo que la temperatura inicial es mayor que la final
   while (TF > T) {
      T = T * TF;
   }

   double max_vecinos = 10 * datos.size();
   double max_exitos = 0.1 * max_vecinos;
   double M = EVALUACIONES / max_vecinos;
   double beta = (T0 - TF) / (M * T0 * TF);
   int exitos = 1;

   while (exitos != 0 && T > TF) {
      exitos = 0;
      vecinos = 0;

      while (exitos < max_exitos && vecinos < max_vecinos && evaluaciones < EVALUACIONES) {
         vector<int> nueva_solucion = cambioCluster(solucion_actual);
         double nueva_valoracion = Evalua.funcionObjetivo(nueva_solucion);

         evaluaciones++;
         vecinos++;

         double diferencia = nueva_valoracion - valoracion_actual;
         if(diferencia < 0 || Rand() <= exp(-diferencia / T)){
            exitos++;
            solucion_actual = nueva_solucion;
            valoracion_actual = nueva_valoracion;

            if(valoracion_actual < mejor_valoracion){
               mejor_solucion = solucion_actual;
               mejor_valoracion = valoracion_actual;
            }
         }

      }
      //Esquema de enfriamiento de Cauchy
      T = T / (1 + beta * T);
   }
   return mejor_solucion;
}




bool hibrido::extraerVector(vector<int> &vector, int buscado){
   for(std::vector<int>::iterator it = vector.begin(); it != vector.end(); it++){
      if((*it) == buscado){
         vector.erase(it);
         return true;
      }
   }
   return false;
}




bool hibrido::contiene(vector<int> v, int valor){
   for(int i = 0; i < v.size(); i++){
      if(v[i] == valor)
         return true;
   }
   return false;
}




vector<int> hibrido::RandVector(vector<int> v, int tamanio){
   std::vector<int> aux;
   std::vector<int> aux_2;
   std::vector<int> indices;

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



vector<int> hibrido::mutarBrusco(vector<int> solucion){
   int numero_mutaciones = PORCENTAJE_MUTA * datos.size();
   int tamanio = solucion.size();
   //genero r y v
   int inicio = Randint(0, tamanio - 1);
   int tamanio_segmento = Randint(0, tamanio - 1);
   vector<int> resto;
   vector<int> salida;
   vector<int> posicion_segmento;

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
   vector<int> seleccion = RandVector(resto, numero_mutaciones);

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




vector<int> hibrido::ILS_ES(){
   vector<int> solucion_actual = solucionInicial();
   solucion_actual = enfriamientoSimilado(solucion_actual);
   double valoracion_actual = Evalua.funcionObjetivo(solucion_actual);

   double mejor_valoracion = valoracion_actual;
   vector<int> mejor_solucion = solucion_actual;
   vector<int> nueva_solucion;
   double nueva_valoracion;
   int it = 0;

   while (it < ILS_ITERACIONES){
      nueva_solucion = mejor_solucion;

      nueva_solucion = mutarBrusco(nueva_solucion);

      nueva_solucion = enfriamientoSimilado(nueva_solucion);

      nueva_valoracion = Evalua.funcionObjetivo(nueva_solucion);

      if(nueva_valoracion < mejor_valoracion){
         mejor_valoracion = nueva_valoracion;
         mejor_solucion = nueva_solucion;
      }
      it++;
   }
   Evalua.funcionObjetivo(mejor_solucion);
   return mejor_solucion;
}
