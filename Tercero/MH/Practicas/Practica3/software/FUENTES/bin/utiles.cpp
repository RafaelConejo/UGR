#include "utiles.h"

vector<vector<double>> utiles::leerArchivoMatriz(string nombre_archivo){
  ifs.open(nombre_archivo);
  vector<vector<double>> valores;
  vector<double> aux;

  while (ifs.eof() == false) {
    string linea;
    getline(ifs, linea);
    size_t pos_coma = linea.find_first_of(",");
    size_t pos_1 = 0;


    while(pos_coma != -1){
      string subcadena = linea.substr(pos_1, pos_coma-pos_1);
      pos_1 = pos_coma;
      linea.erase(pos_1, 1);
      //pos_1 = pos_1+1;
      pos_coma = linea.find_first_of(",");

      aux.push_back(stof(subcadena));

      if(pos_coma == -1){
         std::string::iterator it = linea.end();
         string subcadena = linea.substr(pos_1, linea.length()-1);
         aux.push_back(stof(subcadena));
      }
    }
    valores.push_back(aux);
    aux.clear();
  }
  ifs.close();
  return valores;
}
