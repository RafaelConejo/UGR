#include "utiles.h"
#include "AG.h"
#include "AM.h"
#include "random.h"
//#include "evaluacion.h"
#include <chrono>
#include <ctime>
#include <ratio>

/* La probabilidad de cruce será
0,7 en el AGG y 1 en el AGE (siempre se cruzan los dos padres). La probabilidad de
mutación (por gen) será de 0,001 en ambos casos. El criterio de parada en las dos
versiones del AG consistirá en realizar 100000 evaluaciones de la función objetivo.

El tamaño de la población del AM será de 10 cromosomas. Las probabilidades de
cruce y mutación serán 0,7 (por cromosoma) y 0,001 (por gen) en ambos casos. Cuando

la BLS no produce cambios en el cromosoma decimos que falla. El contador de fallos
está diseñado para evitar que se desperdicien evaluaciones de la función objetivo en
cromosomas de mucha calidad. Permitiremos como mucho ξ fallos en la BLS, siendo ξ =
0,1·n. El criterio de parada del AM consistirá en realizar 100000 evaluaciones de la
función objetivo, incluidas por supuesto las de la BLS.

*/

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
	std::cout << "Introduzca 1 si desea el algoritmo genetico: " << '\n';
	std::cout << "Introduzca 2 si desea el algoritmo memetico: " << '\n';
	std::cin >> algoritmo;

	int modo;
	int tipo;
	if(algoritmo == 1){
		std::cout << "Introduzca 1 si desea Estacionario con cruce Uniforme: " << '\n';
		std::cout << "Introduzca 2 si desea Estacionario con cruce por Segmento fijo: " << '\n';
		std::cout << "Introduzca 3 si desea Generacional con cruce Uniforme: " << '\n';
		std::cout << "Introduzca 4 si desea Generacional con cruce por Segmento fijo: " << '\n';
		std::cin >> tipo;

		switch (tipo) {
			case 1: modo = 0;
			break;
			case 2: modo = 1;
			break;
			case 3: modo = 2;
			break;
			case 4: modo = 3;
			break;
		}
	}

	if(algoritmo == 2){
		std::cout << "Introduzca 1 si desea AM-(10, 1.0): " << '\n';
		std::cout << "Introduzca 2 si desea AM-(10, 0.1): " << '\n';
		std::cout << "Introduzca 3 si desea AM-(10, 0.1mej): " << '\n';
		std::cin >> tipo;

		switch (tipo) {
			case 1: modo = 0;
			break;
			case 2: modo = 1;
			break;
			case 3: modo = 2;
			break;
		}
	}


	switch (algoritmo) {
		case 1:{
			//Algoritmos geneticos
			AG genetico(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<size_t> solucion = genetico.generarPoblacion(modo);
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			//for (size_t i = 0; i < solucion.size(); i++) {
			//	std::cout << solucion[i] << ' ';
			//}

			std::cout << '\n';
			size_t aux = 0;
			//size_t valoracion = genetico.evaluarPoblacion(solucion, aux);
			double valoracion = genetico.getValoracionFinal();
			//std::cout << '\n' << " Tasa_c: "  << genetico.getTasa_c() << '\n';
			//std::cout << " Tasa_inf: "  << genetico.getTasa_inf() << '\n';
			//std::cout << " Agregacion final: " << valoracion << '\n';
			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			break;
		}

		case 2:{
			//Algoritmos memeticos
			AM memetico(n_cluster, datos, restricciones);

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			vector<size_t> solucion = memetico.generarPoblacion(modo);
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

			//for (size_t i = 0; i < solucion.size(); i++) {
			//	std::cout << solucion[i] << ' ';
			//}

			std::cout << '\n';
			size_t aux = 0;
			//size_t valoracion = genetico.evaluarPoblacion(solucion, aux);
			double valoracion = memetico.getValoracionFinal();
			//std::cout << " Tasa_c: "  << memetico.getTasa_c() << '\n';
			//std::cout << " Tasa_inf: "  << memetico.getTasa_inf() << '\n';
			//std::cout << '\n' << " Agregacion final: " << valoracion << '\n';
			std::cout << " Tiempo ejecucción: " << time_span.count() << '\n';
			break;
		}
	}


//49137372








/*
//Busqueda Local
	//Establezco numero de cluster
	busquedaLocal busq_local(n_cluster, datos, restricciones);


	//vector<vector<double>> centroides = busq_local.obtenerCentroide();

	//double primera_valoracion = busq_local.funcionValoracion();
	//std::cout << primera_valoracion << '\n';

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	busq_local.funcionBusquedaLocal();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);


	double Tasa_c = busq_local.getTasa_c();
	double Tasa_inf = busq_local.getTasa_inf();
	std::cout << "Tasa_c: " << Tasa_c << '\n';
	std::cout << "Tasa_inf: " << Tasa_inf << '\n';

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

	Tasa_c = greedy.getTasa_c();
	Tasa_inf = greedy.getTasa_inf();
	std::cout << "Tasa_c: " << Tasa_c << '\n';
	std::cout << "Tasa_inf: " << Tasa_inf << '\n';

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
