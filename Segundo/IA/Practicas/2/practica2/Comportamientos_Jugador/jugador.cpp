#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <list>
#include <stack>


//función para pintar el mapa
void ComportamientoJugador::pintarMapa(int brujula, Sensores sensores){
	switch (brujula) {
		//Norte
		case 0: mapaResultado[fil-1][col-1] = sensores.terreno[1];
			mapaResultado[fil-1][col] = sensores.terreno[2];
			mapaResultado[fil-1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col-2] = sensores.terreno[4];
			mapaResultado[fil-2][col-1] = sensores.terreno[5];
			mapaResultado[fil-2][col] = sensores.terreno[6];
			mapaResultado[fil-2][col+1] = sensores.terreno[7];
			mapaResultado[fil-2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col-3] = sensores.terreno[9];
			mapaResultado[fil-3][col-2] = sensores.terreno[10];
			mapaResultado[fil-3][col-1] = sensores.terreno[11];
			mapaResultado[fil-3][col] = sensores.terreno[12];
			mapaResultado[fil-3][col+1] = sensores.terreno[13];
			mapaResultado[fil-3][col+2] = sensores.terreno[14];
			mapaResultado[fil-3][col+3] = sensores.terreno[15];
		break;
		//ESte
		case 1: mapaResultado[fil-1][col+1] = sensores.terreno[1];
			mapaResultado[fil][col+1] = sensores.terreno[2];
			mapaResultado[fil+1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col+2] = sensores.terreno[4];
			mapaResultado[fil-1][col+2] = sensores.terreno[5];
			mapaResultado[fil][col+2] = sensores.terreno[6];
			mapaResultado[fil+1][col+2] = sensores.terreno[7];
			mapaResultado[fil+2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col+3] = sensores.terreno[9];
			mapaResultado[fil-2][col+3] = sensores.terreno[10];
			mapaResultado[fil-1][col+3] = sensores.terreno[11];
			mapaResultado[fil][col+3] = sensores.terreno[12];
			mapaResultado[fil+1][col+3] = sensores.terreno[13];
			mapaResultado[fil+2][col+3] = sensores.terreno[14];
			mapaResultado[fil+3][col+3] = sensores.terreno[15];
		break;
		//Sur
		case 2:mapaResultado[fil+1][col+1] = sensores.terreno[1];
			mapaResultado[fil+1][col] = sensores.terreno[2];
			mapaResultado[fil+1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col+2] = sensores.terreno[4];
			mapaResultado[fil+2][col+1] = sensores.terreno[5];
			mapaResultado[fil+2][col] = sensores.terreno[6];
			mapaResultado[fil+2][col-1] = sensores.terreno[7];
			mapaResultado[fil+2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col+3] = sensores.terreno[9];
			mapaResultado[fil+3][col+2] = sensores.terreno[10];
			mapaResultado[fil+3][col+1] = sensores.terreno[11];
			mapaResultado[fil+3][col] = sensores.terreno[12];
			mapaResultado[fil+3][col-1] = sensores.terreno[13];
			mapaResultado[fil+3][col-2] = sensores.terreno[14];
			mapaResultado[fil+3][col-3] = sensores.terreno[15];
		break;
		//Oeste
		case 3:mapaResultado[fil+1][col-1] = sensores.terreno[1];
			mapaResultado[fil][col-1] = sensores.terreno[2];
			mapaResultado[fil-1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col-2] = sensores.terreno[4];
			mapaResultado[fil+1][col-2] = sensores.terreno[5];
			mapaResultado[fil][col-2] = sensores.terreno[6];
			mapaResultado[fil-1][col-2] = sensores.terreno[7];
			mapaResultado[fil-2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col-3] = sensores.terreno[9];
			mapaResultado[fil+2][col-3] = sensores.terreno[10];
			mapaResultado[fil+1][col-3] = sensores.terreno[11];
			mapaResultado[fil][col-3] = sensores.terreno[12];
			mapaResultado[fil-1][col-3] = sensores.terreno[13];
			mapaResultado[fil-2][col-3] = sensores.terreno[14];
			mapaResultado[fil-3][col-3] = sensores.terreno[15];
		break;
	}
}

//Este metodo lo utilizo para que cuando aparezca un K en la vision
//del jugador vaya directo hacia él
bool ComportamientoJugador::buscarK(int brujula, Sensores sensores){
	Action accion;
	hayPlan = true;
	bool acciones = false;
	plan.clear();
	switch (brujula) {
		//En todas las direcciones es igual.
		case 0: case 1: case 2: case 3:
			if(sensores.terreno[1] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[2] == 'K' or sensores.terreno[6] == 'K'
														or sensores.terreno[12] == 'K'){
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[3] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[4] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[5] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[7] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[8] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[9] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[10] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[11] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[13] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[14] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
			if(sensores.terreno[15] == 'K'){
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				acciones = true;
			}
		break;
	}
	if(acciones)
		std::cout << "Localizado punto amarillo" << '\n';
	return acciones;
}



// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	//Capturar los valores de las filas y columnas
	if(sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		ultimaAccion = actIDLE;
		dondeEstoy = true;
	}

	//Actualizar el efecto de la última acción
	switch (ultimaAccion) {
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula) {
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
		break;
	}

	//Mirar si ha cambiado el destino
	if(sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;
		hayPlan = false;
	}
	//Separo los algoritmos del nivel 1 con lo del nivel 2.
	if(sensores.nivel != 4){
			//Calcular camino hasta el destino
		if(!hayPlan){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
		}

		Action sigAccion;
		if(hayPlan and plan.size() > 0){	//hay plan y hay que seguirlo
			if((sensores.superficie[2] == 'a' or sensores.terreno[2] == 'P'
				or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M') and actFORWARD){
				sigAccion = actIDLE;
				hayPlan = false;
			}
			else{
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}
		}
		else{	//No hay plan y se activa comportamiento reactivo
			if(sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M' or
				sensores.terreno[2] == 'D' or sensores.superficie[2] == 'a'){
					sigAccion = actTURN_R;
			}
			else{
				sigAccion = actFORWARD;
			}
		}

		//Recordar última accion
		ultimaAccion = sigAccion;
		return sigAccion;
	}

	//NIVEL 2

	else{
		//Calcular camino hasta el destino
		if(!hayPlan and dondeEstoy){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
			if(hayPlan)
				pintar = true;
		}

		Action sigAccion;
		if(hayPlan and (plan.size() > 0)){	//hay plan y hay que seguirlo
			if(pintar){
				mapaResultado[fil][col] = sensores.terreno[0];
				//funcion que me permite ir pintando el mapa
				pintarMapa(brujula, sensores);
			}
			if((sensores.superficie[2] == 'a' or sensores.terreno[2] == 'P'
				or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M')
					and plan.front() == actFORWARD){
				sigAccion = actIDLE;
				hayPlan = false;

				if(sensores.superficie[2] == 'a'){
					hayPlan = true;
				}
			}
			else{
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}
		}
		else{	//No hay plan y se activa comportamiento reactivo
			//devolverá true si ha encontrado K por lo tanto hay un plan que debe llevar a cabo.
			hayPlan = buscarK(brujula, sensores);

			if(sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M' or
				sensores.terreno[2] == 'D' or sensores.superficie[2] == 'a'){
					//De esta forma no siempre girará hacia el mismo lado al encontrarse
					//con una de estas situaciones.
					int giro = aleatorio(2);
					if(giro == 0)
						sigAccion = actTURN_R;
					else
						sigAccion = actTURN_L;
			}
			else{
				//si hay plan dejará de ejecutarse, y se desarrolarán las acciones del plan.
				if(!hayPlan){
					//le doy mayor prioridad a andar recto que ha girar para evitar
					//que gire constantemente y siempre recorra la misma zona
					int cual = aleatorio(30);
					if (cual == 1)
						sigAccion = actTURN_L;
					else if(cual == 2)
						sigAccion = actTURN_R;
					else
						sigAccion = actFORWARD;
				}
			}
		}

		//Recordar última accion
		ultimaAccion = sigAccion;
		return sigAccion;
	}
}


// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			      return pathFinding_Anchura(origen, destino, plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
					return pathFinding_Costo_Uniforme(origen, destino, plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
					return pathFinding_Nivel_2(origen, destino, plan);
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M' or casilla =='D')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}




struct nodo{
	estado st;
	list<Action> secuencia;
	int coste;
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

// Implementación de la búsqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	list<nodo> lista;											// Lista de Abiertos

  	nodo current;	//nodo actual
	current.st = origen;
	current.secuencia.empty();

	lista.push_front(current);

  while (!lista.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		lista.pop_front();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			//inserto en la lista
			lista.push_back(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			//inserto en la lista
			lista.push_back(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				//inserto en la lista
				lista.push_back(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!lista.empty()){
			current = lista.front();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}



struct ComparaCosto{
	bool operator()(const nodo &a, const nodo &b) const{
		if (a.coste <= b.coste)
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda de costo uniforme.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Costo_Uniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	set<nodo,ComparaCosto> ordenados;											// Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	current.coste = 0;
	ordenados.insert(current);
	set<nodo,ComparaCosto>::iterator itSig;

  while (!ordenados.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

	  	//creo iterator que apunte al primer elemento del set ordeandos
	  	set<nodo,ComparaCosto>::iterator it = ordenados.begin();
		ordenados.erase(it);
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			//coste por girar es de 1
			hijoTurnR.coste++;

			ordenados.insert(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			//coste por girar es de 1
			hijoTurnL.coste++;

			ordenados.insert(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);

				if(mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'T')
					hijoForward.coste += 2;
				else if (mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'B')
					hijoForward.coste += 5;
				else if (mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'A')
					hijoForward.coste += 10;
				else
					hijoForward.coste++;

				ordenados.insert(hijoForward);
			}
		}

		// Tomo el siguiente valor del set
		if (!ordenados.empty()){
			//iterator definido anteriormente
			itSig = ordenados.begin();
			//tomo el valor que contiene
			current = *itSig;
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;

}



bool ComportamientoJugador::pathFinding_Nivel_2(const estado &origen, const estado &destino, list<Action> &plan){
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	set<nodo,ComparaCosto> ordenados;											// Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();

	current.coste = 0;
	ordenados.insert(current);
	set<nodo,ComparaCosto>::iterator itSig;

  while (!ordenados.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

	  	//creo iterator que apunte al primer elemento del set ordeandos
	  	set<nodo,ComparaCosto>::iterator it = ordenados.begin();
		ordenados.erase(it);
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);

			hijoTurnR.coste++;

			ordenados.insert(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);

			hijoTurnL.coste++;

			ordenados.insert(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);

				if(mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'T')
					hijoForward.coste += 2;
				else if (mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'B')
					hijoForward.coste += 5;
				else if (mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == 'A')
					hijoForward.coste += 10;
				else if(mapaResultado[hijoForward.st.fila][hijoForward.st.columna] == '?')
					hijoForward.coste += 3;
				else
					hijoForward.coste++;

				ordenados.insert(hijoForward);
			}
		}

		// Tomo el siguiente valor del set
		if (!ordenados.empty()){
			itSig = ordenados.begin();
			current = *itSig;
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;

}






// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
