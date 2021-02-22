#include "busquedaLocal.h"
#include "utiles.h"
#include "COPKM.h"
#include <chrono>
#include <ctime>
#include <ratio>

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

//Busqueda Local
	//Establezco numero de cluster
	busquedaLocal busq_local(n_cluster, datos, restricciones);


	//vector<vector<double>> centroides = busq_local.obtenerCentroide();

	//double primera_valoracion = busq_local.funcionValoracion();
	//std::cout << primera_valoracion << '\n';

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	vector<int> solucion = busq_local.algoritmoBusquedaLocal();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "Tiempo ejecucción busqueda local: " << time_span.count() << '\n';




	char seguir;
	std::cout << "Introduce cualquier elemento por teclado para continuar" << '\n';
	std::cin >> seguir;

	//Greedy
	COPKM greedy(n_cluster, datos, restricciones);

	high_resolution_clock::time_point t3 = high_resolution_clock::now();
	greedy.greedy();
	high_resolution_clock::time_point t4 = high_resolution_clock::now();
	duration<double> time_span_2 = duration_cast<duration<double>>(t4 - t3);



	std::cout << "Tiempo ejecucción Greedy: " << time_span_2.count() << '\n';

//Para mostrar los datos y restricciones
/*	for (size_t i = 0; i < datos.size(); i++) {
		for (size_t j = 0; j < datos[i].size(); j++) {
			std::cout << (int)datos[i][j] << " ";
		}
		std::cout << "\n";
	}



	for (size_t i = 0; i < restricciones.size(); i++) {
		for (size_t j = 0; j < restricciones[i].size(); j++) {
			std::cout << (int)restricciones[i][j] << " ";
		}
		std::cout << "\n";
	}*/


  return 0;
}
