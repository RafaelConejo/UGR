#include "AG.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "random.h"


AG::AG(int n, vector<vector<double>> valores, vector<vector<double>> restr){
  n_cluster = n;
  datos = valores;
  restricciones = restr;
  Evalua.inicializar(n_cluster, datos, restricciones);
}




bool AG::contiene(vector<size_t> v, int valor){
   for(int i = 0; i < v.size(); i++){
      if(v[i] == valor)
         return true;
   }
   return false;
}




size_t AG::reemplazamiento(vector<vector<size_t>> poblacion, int modo){
   size_t tamanio_poblacion = poblacion.size();
   size_t sustituto;
   double aux;
   valoracion_final = 0;

   //peor
   if(modo == -1){
      aux = 0;
      sustituto = 0;

      for(int i = 0; i < tamanio_poblacion; i++){
         double valoracion = Evalua.funcionObjetivo(poblacion[i]);
         if(valoracion > aux){
            aux = valoracion;
            sustituto = i;
         }
      }
   }
   else if(modo == 0){

   }
   //mejor
   else if(modo == 1){
      aux = 9999999;
      sustituto = 0;

      for(int i = 0; i < tamanio_poblacion; i++){
         double valoracion = Evalua.funcionObjetivo(poblacion[i]);
         if(valoracion < aux){
            aux = valoracion;
            sustituto = i;
         }
      }
   }
   return sustituto;
}




vector<size_t> AG::generarPoblacion(int modo){
  vector<vector<size_t>> poblacion;
  size_t cromosomas = datos.size();

  poblacion.resize(TAM_POBLACION_AG);

  for (size_t i = 0; i < TAM_POBLACION_AG; i++){
     for (size_t j = 0; j < cromosomas; j++)
         poblacion[i].push_back(Randint(0, n_cluster - 1));
  }

  poblacion = algoritmoGenetico(poblacion, modo);

  size_t mejorCromosoma = reemplazamiento(poblacion, 1);

  valoracion_final = Evalua.funcionObjetivo(poblacion[mejorCromosoma]);
  Tasa_c = Evalua.getTasa_c();
  Tasa_inf = Evalua.getTasa_inf();

  vector<size_t> mejorSolucion = poblacion[mejorCromosoma];

  return mejorSolucion;
}




vector<size_t> AG::evitarClusterVacio(vector<size_t> hijo){
   std::vector<size_t> comprobar;
   bool vacio = true;
   size_t genes = hijo.size();
   //todos los valores de comprobar a 0
   for(size_t i = 0; i < n_cluster; i++)
      comprobar.push_back(0);

   //Introduzco los elemento de cada cluster
   for (size_t i = 0; i < genes; i++)
      comprobar[hijo[i]]++;

   while(vacio){
      for (size_t i = 0; i < n_cluster; i++){
         if(comprobar[i] == 0){
            //asigno valor aleatorio a cluster vacio
            size_t gen_rand = Randint(0, genes - 1);
            size_t aux = hijo[gen_rand];
            hijo[gen_rand] = i;
            comprobar[i]++;
            comprobar[aux]--;
            break;
         }
         if(comprobar[i] != 0 && i == n_cluster - 1){
            vacio = false;
         }
      }
   }
   return hijo;
}




vector<size_t> AG::cruceUniforme(vector<size_t> padre_1, vector<size_t> padre_2){
   vector<size_t> seleccion;
   vector<size_t> hijo;
   size_t cromosomas = padre_1.size();
   //selecciono uno de los padres
   for (size_t i = 0; i < cromosomas / 2; i++)
      seleccion.push_back(Randint(0, cromosomas - 1));

   for (size_t i = 0; i < cromosomas; i++){
      //copiamos los que coinciden con los aleatorios
      if(contiene(seleccion, i))
         hijo.push_back(padre_1[i]);
      else
         hijo.push_back(padre_2[i]);
   }
   //compruebo ningun cluster vacio y devuelvo el hijo
  return evitarClusterVacio(hijo);
}



bool AG::extraerVector(vector<size_t> &vector, size_t buscado){
   for(std::vector<size_t>::iterator it = vector.begin(); it != vector.end() ;it++){
      if((*it) == buscado){
         vector.erase(it);
         return true;
      }
   }
   return false;
}



vector<size_t> AG::RandVector(vector<size_t> v, int tamanio){
   std::vector<size_t> aux;
   std::vector<size_t> aux_2;
   std::vector<size_t> indices;

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




vector<size_t> AG::cruceSegmentoFijo(vector<size_t> padre_1, vector<size_t> padre_2){
   vector<size_t> hijo;
   vector<size_t> resto;
   size_t genes = padre_1.size();
   //genero r y v
   size_t inicio = Randint(0, genes - 1);
   size_t tam_segmento = Randint(0, genes - 1);
   vector<size_t> posicion_segmento;

   for (size_t i = 0; i < tam_segmento; i++)
      posicion_segmento.push_back((inicio + i) % genes);

   for (size_t i = 0; i < genes; i++){
      hijo.push_back(-1);
      if (!extraerVector(posicion_segmento, i))
         resto.push_back(i);

      else
         hijo[i] = padre_1[i];
   }

   vector<size_t> seleccion = RandVector(resto, resto.size() / 2);

   for(size_t i = 0; i < genes; i++){
      if (extraerVector(resto, i)){
         if (extraerVector(seleccion, i))
            hijo[i] = padre_1[i];

         else
            hijo[i] = padre_2[i];
      }
   }
  return evitarClusterVacio(hijo);
}




vector<size_t> AG::mutacionUniforme(vector<size_t> hijo){
   size_t nuevo_cluster = Randint(0, n_cluster - 1);
   size_t genes = hijo.size();
   size_t pos = Randint(0, genes - 1);

   while(nuevo_cluster == hijo[pos]){
      nuevo_cluster = Randint(0, n_cluster - 1);
   }
   hijo[pos] = nuevo_cluster;

   return evitarClusterVacio(hijo);
}



//revisar
vector<size_t> AG::torneoBinario(vector<vector<size_t>> poblacion){
   vector<size_t> cromosomas_torneo;
   double min = 99999999;
   size_t mejor = 0;

   size_t cromosoma_1 = Randint(0, poblacion.size() - 1);
   size_t cromosoma_2 = Randint(0, poblacion.size() - 1);
   //compruebo que sean diferentes
   while(cromosoma_1 == cromosoma_2){
      cromosoma_2 = Randint(0, poblacion.size() - 1);
   }

   cromosomas_torneo.push_back(cromosoma_1);
   cromosomas_torneo.push_back(cromosoma_2);

   for(size_t i = 0; i < 2; i++){
      double valoracion = Evalua.funcionObjetivo(poblacion[cromosomas_torneo[i]]);
      if(valoracion < min){
         min = valoracion;
         mejor = i;
      }
   }
   return poblacion[mejor];
}




bool AG::dado(double probabilidad){
   float dado = Rand();
   return (dado < probabilidad);
}




vector<vector<size_t>> AG::cruzarEstacionario(vector<vector<size_t>> poblacion, int modo){
   vector<vector<size_t>> padres;

   size_t esperanza = PROBABILIDAD_CRUCE_AGE * PADRES_ESTACIONARIO;

   for (size_t i = 0; i < esperanza; i++){
      padres.push_back(torneoBinario(poblacion));
   }

   vector<vector<size_t>> hijos;
   switch (modo){
      case 0:
         for(size_t i = 0; i < esperanza; i++){
            hijos.push_back(cruceUniforme(padres[0], padres[1]));
         }
         break;

      case 1:
         for(size_t i = 0; i < esperanza; i++){
            //std::cout << "lo hace 1" << '\n';
            hijos.push_back(cruceSegmentoFijo(padres[0], padres[1]));
            //std::cout << "lo hace 1.5" << '\n';
         }
         break;
   }
   //std::cout << "lo hace 2" << '\n';
   return hijos;
}




vector<vector<size_t>> AG::mutarAG(vector<vector<size_t>> poblacion, vector<size_t> & posiciones){
   size_t tamanio_poblacion = poblacion.size();
   size_t n_genes = poblacion[0].size();
   vector<vector<size_t>> mutados;

   for (size_t i = 0; i < tamanio_poblacion; i++){
      if (dado(PROBABILIDAD_MUTACION_AG * n_genes)){
         mutados.push_back(mutacionUniforme(poblacion[i]));
         posiciones.push_back(i);
      }
   }
   return mutados;
}




vector<vector<size_t>> AG::cruzarGeneracional(vector<vector<size_t>> poblacion, int modo, vector<size_t> & posiciones){
   vector<vector<size_t>> padres;
   vector<vector<size_t>> hijos;
   size_t esperanza = PROBABILIDAD_CRUCE_AGG * PADRES_GENERACIONAL;

   for (size_t i = 0; i < esperanza; i++){
      padres.push_back(torneoBinario(poblacion));
   }

   esperanza = esperanza / 2;
   switch (modo) {
      case 3:
         for (size_t i = 0; i < esperanza; i++) {
            hijos.push_back(cruceUniforme(padres[i], padres[i + esperanza]));
            hijos.push_back(cruceUniforme(padres[i], padres[i + esperanza]));
         }
         break;

      case 4:
         for (size_t i = 0; i < esperanza; i++) {
            double valoracion_1 = Evalua.funcionObjetivo(padres[i]);
            double valoracion_2 = Evalua.funcionObjetivo(padres[i + esperanza]);

            if(valoracion_1 < valoracion_2){
               hijos.push_back(cruceSegmentoFijo(padres[i], padres[i + esperanza]));
               hijos.push_back(cruceSegmentoFijo(padres[i], padres[i + esperanza]));
            }
            else{
               hijos.push_back(cruceSegmentoFijo(padres[i + esperanza], padres[i]));
               hijos.push_back(cruceSegmentoFijo(padres[i + esperanza], padres[i]));
            }
         }
         break;
   }
  return hijos;
}




double AG::evaluarPoblacion(vector<vector<size_t>> poblacion){
   size_t tamanio_poblacion = poblacion.size();
   double valoracion = 0;
   for (size_t i = 0; i < tamanio_poblacion; i++) {
      valoracion = valoracion + Evalua.funcionObjetivo(poblacion[i]);
   }
   //media
   valoracion = valoracion / tamanio_poblacion;
   return valoracion;
}




//0 es estacionario cruceUniforme y 1 estacionario cruceSegmentoFijo
//2 es generacional cruceUniforme y 3 es generacional cruceSegmentoFijo
vector<vector<size_t>> AG::algoritmoGenetico(vector<vector<size_t>> poblacion, int modo){
   vector<vector<size_t>> nueva_poblacion = poblacion;

   while (Evalua.getEvaluaciones() < EVALUACIONES/* && funcion_poblacion >= funcion_nueva_poblacion*/){
      poblacion = nueva_poblacion;
      vector<vector<size_t>> hijos;
      vector<vector<size_t>> mutados;
      vector<size_t> posiciones;
      switch (modo) {
         case 0:
         case 1: {
            //genero los hijos
            hijos = cruzarEstacionario(nueva_poblacion, modo);
            //mutacion
            mutados = mutarAG(nueva_poblacion, posiciones);
            for(size_t i = 0; i < posiciones.size(); i++){
               //actualizo los elementos mutados
               nueva_poblacion[posiciones[i]] = mutados[i];
            }
            //sustituyo hijos
            size_t peor = reemplazamiento(nueva_poblacion, -1);
            if(Evalua.funcionObjetivo(nueva_poblacion[peor]) > Evalua.funcionObjetivo(hijos[0])){
               nueva_poblacion[peor] = hijos[0];
            }
            peor = reemplazamiento(nueva_poblacion, -1);
            if(Evalua.funcionObjetivo(nueva_poblacion[peor]) > Evalua.funcionObjetivo(hijos[1])){
               nueva_poblacion[peor] = hijos[1];
            }

            break;
         }
         //generacional
         case 2:
         case 3: {
            vector<size_t> posiciones_hijos;
            //genero los hijos
            hijos = cruzarGeneracional(nueva_poblacion, modo, posiciones);
            //mutacion
            mutados = mutarAG(nueva_poblacion, posiciones);
            //añado las mutaciones
            for (size_t i = 0; i < posiciones.size(); i++){
               nueva_poblacion[posiciones[i]] = mutados[i];
            }
            //sustituyo hijos
            size_t mejor = reemplazamiento(nueva_poblacion, 1);

            bool elitista = false;
            vector<size_t> aux;
            for (size_t i = 0; i < posiciones_hijos.size(); i++){
               nueva_poblacion[posiciones_hijos[i]] = hijos[i];
               if (posiciones_hijos[i] == mejor){
                  elitista = true;
                  aux = nueva_poblacion[mejor];
               }
            }

            if(elitista){
               size_t peor = reemplazamiento(hijos, -1);
               nueva_poblacion[peor] = aux;
            }
            break;
         }
      }
   }
  return poblacion;
}



















//
