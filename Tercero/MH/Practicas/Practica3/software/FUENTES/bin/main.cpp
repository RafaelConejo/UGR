#include "utiles.h"
#include "trayectorias_simples.h"
#include "trayectorias_multiples.h"
#include "hibrido.h"
#include "random.h"
#include <chrono>
#include <ctime>
#include <ratio>
#include <random>


using namespace std::chrono;

const string ecoli = "datos/ecoli_set.dat";
const string iris = "datos/iris_set.dat";
const string rand_ = "datos/rand_set.dat";
const string newthy = "datos/newthyroid_set.dat";
const string ecoli_rest_10 = "datos/ecoli_set_const_10.const";
const string ecoli_rest_20 = "datos/ecoli_set_const_20.const";
const string iris_rest_10 = "datos/iris_set_const_10.const";
const string iris_rest_20 = "datos/iris_set_const_20.const";
const string rand_rest_10 = "datos/rand_set_const_10.const";
const string rand_rest_20 = "datos/rand_set_const_20.const";
const string newthy_rest_10 = "datos/newthyroid_set_const_10.const";
const string newthy_rest_20 = "datos/newthyroid_set_const_20.const";


int main(int argc, char const *argv[]) {

	utiles util;
	vector<vector<double>> datos;
	vector<vector<double>> restricciones;

	double semilla;
	std::cout << "Introduzca la semilla: " << '\n';
	std::cin >> semilla;

	//inicilazo la semilla
	Set_random(semilla);
	std::minstd_rand0 generator (semilla);

	int dat;
	std::cout << "Introduzca 1 si desea el conjunto de datos Iris: " << '\n';
	std::cout << "Introduzca 2 si desea el conjunto de datos Ecoli: " << '\n';
	std::cout << "Introduzca 3 si desea el conjunto de datos Rand: " << '\n';
	std::cout << "Introduzca 4 si desea el conjunto de datos Newthyroid: " << '\n';
	std::cin >> dat;

	int rest;
	std::cout << "Introduzca 1 si desea restricciones 10 por ciento: " << '\n';
	std::cout << "Introduzca 2 si desea restricciones 20 por ciento: " << '\n';
	std::cin >> rest;

	switch (dat) {
		case 1: datos = util.leerArchivoMatriz(iris);
			switch (rest) {
				case 1: restricciones = util.leerArchivoMatriz(iris_rest_10);
				break;
				case 2: restricciones = util.leerArchivoMatriz(iris_rest_20);
				break;
			}
			break;

		case 2: datos = util.leerArchivoMatriz(ecoli);
			switch (rest) {
				case 1: restricciones = util.leerArchivoMatriz(ecoli_rest_10);
				break;
				case 2: restricciones = util.leerArchivoMatriz(ecoli_rest_20);
				break;
			}
			break;

		case 3: datos = util.leerArchivoMatriz(rand_);
			switch (rest) {
				case 1: restricciones = util.leerArchivoMatriz(rand_rest_10);
				break;
				case 2: restricciones = util.leerArchivoMatriz(rand_rest_20);
				break;
			}
			break;

		case 4: datos = util.leerArchivoMatriz(newthy);
				switch (rest) {
					case 1: restricciones = util.leerArchivoMatriz(newthy_rest_10);
					break;
					case 2: restricciones = util.leerArchivoMatriz(newthy_rest_20);
					break;
				}
				break;
	}

	int n_cluster = 3;
	if(dat == 2){
		n_cluster = 8;
	}

	int algoritmo;
	std::cout << "Introduzca 1 si desea el algoritmo ES: " << '\n';
	std::cout << "Introduzca 2 si desea el algoritmo BMB: " << '\n';
	std::cout << "Introduzca 2 si desea el algoritmo ILS: " << '\n';
	std::cout << "Introduzca 2 si desea el algoritmo ILS_ES: " << '\n';
	std::cin >> algoritmo;

	switch (algoritmo) {
		case 1:{
			trayectorias_simples ts(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<int> solucion = ts.enfriamientoSimilado();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();

			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			std::cout << '\n';

			for (size_t i = 0; i < solucion.size(); i++) {
				std::cout << solucion[i] << ' ';
			}


		}
		break;

		case 2:{
			trayectorias_multiples tm(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<int> solucion = tm.busquedaMultiarranqueBasica();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();

			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			std::cout << '\n';

			for (size_t i = 0; i < solucion.size(); i++) {
				std::cout << solucion[i] << ' ';
			}

		}
		break;

		case 3:{
			trayectorias_multiples tm(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<int> solucion = tm.busquedaLocalReiterada();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();

			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			std::cout << '\n';

			for (size_t i = 0; i < solucion.size(); i++) {
				std::cout << solucion[i] << ' ';
			}

		}
		break;
		case 4:{
			hibrido h(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<int> solucion = h.ILS_ES();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();

			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			std::cout << '\n';

			for (size_t i = 0; i < solucion.size(); i++) {
				std::cout << solucion[i] << ' ';
			}

		}
		break;
	}





  return 0;
}
