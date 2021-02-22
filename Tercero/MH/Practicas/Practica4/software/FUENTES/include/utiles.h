//Clase para leer los datos de entrada y salida
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace std;

class utiles{
private:
  ifstream ifs;

public:
  vector<vector<double>> leerArchivoMatriz(string nombre_archivo);

};
