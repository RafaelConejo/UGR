package Practica1;

import core.game.Observation;
import core.game.StateObservation;
import tools.Vector2d;

import java.util.ArrayList;

public class PathFinder {
	public A_estrella a_estrella;
	public StateObservation state;
	public ArrayList<Observation> grid[][];
	
	//Creo arrays p
	private static int[] vecinos_x = null;
    private static int[] vecinos_y = null;
    
    public PathFinder(StateObservation stateObs) {
    	this.state = stateObs;
    	this.grid = stateObs.getObservationGrid();
    	this.a_estrella = new A_estrella(this);
    	init();
    }
    
    private void init() {
    	//up, down, left, right
    	vecinos_x = new int[]{0,    0,    -1,    1};
        vecinos_y = new int[]{-1,   1,     0,    0};
    }
    
    public void updateStateObs(StateObservation stateObs) {
    	this.state = stateObs;
    	this.grid = stateObs.getObservationGrid();
    }
    
    /**
     * Calculo los vecinos en el Algoritmo A*. Modo 0
     * @param
     * @return Vecino del nodo actual
     */
    public ArrayList<Node> getPortal(Node nodo_actual){
    	ArrayList<Node> vecinos = new ArrayList<Node>();
        int posicion_x = (int) (nodo_actual.position.x);
        int posicion_y = (int) (nodo_actual.position.y);
        
        for(int i = 0; i < vecinos_x.length; i++) {
        	if(grid[posicion_x + vecinos_x[i]][posicion_y + vecinos_y[i]].isEmpty()) {
        		vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        	}
        	else {
        		switch(grid[posicion_x + vecinos_x[i]][posicion_y + vecinos_y[i]].get(0).itype) {
        		case 0:		//Muro
        			break;
        		case 5:		//Portal
        			vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        			break;
        		case 10:	//Escorpion
        			vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        			break;
        		}
        	}
        
        }
        return vecinos; 
    }
    
    public ArrayList<Node> getGemas(Node nodo_actual){
    	ArrayList<Node> vecinos = new ArrayList<Node>();
        int posicion_x = (int) (nodo_actual.position.x);
        int posicion_y = (int) (nodo_actual.position.y);
        
        for(int i = 0; i < vecinos_x.length; i++) {
        	if(grid[posicion_x + vecinos_x[i]][posicion_y + vecinos_y[i]].isEmpty()) {
        		vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        	}
        	else {
        		switch(grid[posicion_x + vecinos_x[i]][posicion_y + vecinos_y[i]].get(0).itype) {
        		case 5:
        			vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        			break;
        		case 6:
        			Node gema = new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i]));
        			vecinos.add(gema);
        			//System.out.print(gema.toString());
        			break;
        		case 10:
        			vecinos.add(new Node(new Vector2d(posicion_x + vecinos_x[i], posicion_y + vecinos_y[i])));
        			break;
        		}
        	}
        }
        return vecinos;
    }
    

    
    
}
