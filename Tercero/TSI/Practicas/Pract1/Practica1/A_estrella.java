package Practica1;

import java.util.ArrayList;
import java.util.PriorityQueue;

public class A_estrella {
	public static PriorityQueue<Node> lista_cerrados, lista_abiertos;
	public PathFinder pathfinder;
	
	public A_estrella(PathFinder pathfinder) {
		this.pathfinder = pathfinder;
	}
	
	
	/**
	 * Calcular coste mediante la distancia Manhattan
	 * nodo_actual
	 * nodo_final Nodo donde esta la meta
	 */
	private double costeEstimado(Node nodo_actual, Node nodo_final) {
		//abs es para obtener el valor absoluto
		double x = Math.abs(nodo_actual.position.x - nodo_final.position.x);
		double y = Math.abs(nodo_actual.position.y - nodo_final.position.y);
		return x+y;
	}
	
	/**
	 * Calcular el camino que hemos seguido
	 * @param nodo
	 * @return El camino de nodos seguido
	 */
	private ArrayList<Node> calcularCamino(Node nodo){
		ArrayList<Node> camino = new ArrayList<Node>();
		while(nodo != null) {
			if(nodo.parent != null) {
				camino.add(0, nodo);
			}
			nodo = nodo.parent;
		}
		return camino;
	}
	
	/**
	 * Algortimo A*
	 * Obtengo el camino desde el nodo inicio hasta el desttino
	 * @param
	 * @return camino del nodo inicio al final
	 */
	public ArrayList<Node> algoritmoAestrella(Node inicio, Node meta, int modoCalculo){
		Node nodo = null;
		lista_cerrados = new PriorityQueue<Node>();
		lista_abiertos = new PriorityQueue<Node>();
		
		inicio.totalCost = 0.0f;
		inicio.estimatedCost += costeEstimado(inicio, meta);
		
		lista_abiertos.add(inicio);
		while(lista_abiertos.size() != 0) {
			nodo = lista_abiertos.poll();	//quito el primero y guardo
			lista_cerrados.add(nodo);
			
			if(nodo.position.equals(meta.position)) {	//comprubo si he llegado a la meta
				return calcularCamino(nodo);
			}
			
			ArrayList<Node> vecinos = new ArrayList<Node>();
			switch(modoCalculo) {
			case 0:					//Modo 0
				vecinos = pathfinder.getPortal(nodo);
				break;
			case 1:
				vecinos = pathfinder.getGemas(nodo);
				break;
			}
			
			for(int i = 0; i < vecinos.size(); i++) {
				Node nodo_vecino = vecinos.get(i);
				double distancia_actual = nodo_vecino.totalCost;
				
				if(!lista_abiertos.contains(nodo_vecino) && !lista_cerrados.contains(nodo_vecino)) {
					nodo_vecino.totalCost = distancia_actual + nodo.totalCost;		//f(n) = g(n) + h(n)
					nodo_vecino.estimatedCost += costeEstimado(nodo_vecino, meta);
					
					nodo_vecino.parent = nodo;
					lista_abiertos.add(nodo_vecino);
				}
				else if(distancia_actual + nodo.totalCost < nodo_vecino.totalCost) {
					nodo_vecino.totalCost = distancia_actual + nodo.totalCost;
					nodo_vecino.parent = nodo;
					
					if(lista_abiertos.contains(nodo_vecino))
						lista_abiertos.remove(nodo_vecino);
					if(lista_cerrados.contains(nodo_vecino))
						lista_cerrados.remove(nodo_vecino);
					
					lista_abiertos.add(nodo_vecino);
				}
			}
		}
		if(nodo.position.equals(meta.position)) {
			return calcularCamino(nodo);
		}
		else {
			return null;
		}
	}
	
	

}
