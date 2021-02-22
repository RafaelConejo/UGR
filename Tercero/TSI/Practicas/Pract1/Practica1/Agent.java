package Practica1;

import java.util.ArrayList;
import java.util.Stack;

import core.game.Observation;
import core.game.StateObservation;
import ontology.Types;
import tools.ElapsedCpuTimer;
import tools.Vector2d;
import core.player.AbstractPlayer;

public class Agent extends utiles_Agent{	//extends Agent_utiles
	PathFinder pathfinder;
	Vector2d ultima_posicion;
	Vector2d escala;		//Escala del mapa
	ArrayList<Node> camino;		//camino desarrollandose
	Stack<Types.ACTIONS> acciones;
	Types.ACTIONS ultima_accion;
	ArrayList<Vector2d> gemas;
	boolean estoyEscapando;			//True si el avatar esta huyendo de un escorpion
	boolean voy_a_gemas;
	boolean hay_escorpiones;
	
	
	/**
	 * Calculo de la posición del avatar en el mapa
	 * 
	 */
	
	/**
	 * Inicializo al agente
	 * @param estado observamos el estado actual
	 * @param elapsedTimer Temporizador
	 */
	public Agent(StateObservation estado, ElapsedCpuTimer elapsedTimer) {
		super(estado, elapsedTimer);
		pathfinder = new PathFinder(estado);
		escala = new Vector2d(estado.getWorldDimension().width / estado.getObservationGrid().length , 
				estado.getWorldDimension().height / estado.getObservationGrid()[0].length); 
		ultima_posicion = new Vector2d(estado.getAvatarPosition().x / escala.x, estado.getAvatarPosition().y / escala.y);
		camino = new ArrayList<Node>();
		acciones = new Stack<Types.ACTIONS>();
		ultima_accion = Types.ACTIONS.ACTION_NIL;
		gemas = new ArrayList<Vector2d>();
		estoyEscapando = false;
		hay_escorpiones = false;
		voy_a_gemas = false;
	}
	
	
	/**
	 * Calcular el camino hasta una ssalida(portal) o gema
	 * @param
	 * @return camino una lista de los nodos hasta el destino
	 */
	public ArrayList<Node> nuevoCamino(Vector2d pos_personaje, int n_gemas, StateObservation estado){
		
		ArrayList<Observation>[] posicion_gemas = estado.getResourcesPositions(estado.getAvatarPosition());
		
		//Si tenemos 10 gemas calculamos el camino hasta la salida
		if(n_gemas == 10 || posicion_gemas == null) {
			//busco donde está el portal
			Vector2d portal;
			ArrayList<Observation>[] posicion_portal = estado.getPortalsPositions(estado.getAvatarPosition());
			portal = posicion_portal[0].get(0).position;
			
			//Realizo la escala para situar las coordenadas
			portal.x = portal.x / escala.x;
			portal.y = portal.y / escala.y;
			
			//Aplico A* para calcular el camino hasta el portal desde el avatar
			camino = pathfinder.a_estrella.algoritmoAestrella(new Node(pos_personaje), new Node(portal), 0);
		}
		
		else {
			//Buscamos las posiciones de las gemas
			Vector2d gema;
			posicion_gemas = estado.getResourcesPositions(estado.getAvatarPosition());
			boolean encontrado_camino = false;
			
			//Comenzando por la más cercana y recorremos las gemas
			for(int i = 0; i < posicion_gemas[0].size(); i++) {
				gema = posicion_gemas[0].get(i).position;
				gema.x = gema.x / escala.x;
				gema.y = gema.y / escala.y;
				
				
				camino = pathfinder.a_estrella.algoritmoAestrella(new Node(pos_personaje), new Node(gema), 1);
					
				//Comrpobamos si hemos encontrado camino, si es así finalizamos
				if(camino != null) {
					encontrado_camino = true;
					break;
				}
				
				if(encontrado_camino) {
					break;
				}
				else
					continue;
			}
			
			if(encontrado_camino == false) {
				esperar();
			}
			
		}
		
		return camino;
	}
	
	
	
	//COMENZAMOS COMPORTAMIENTOS REACTIVOS
	
	/**
	 * No hacer nada
	 */
	public void esperar() {
		acciones.push(Types.ACTIONS.ACTION_NIL);
		//acciones.push(Types.ACTIONS.ACTION_NIL);
	}
	
	
	/**
	 * Comportamiento reactivo para huir si hay peligro a la derecha
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroDerechaArriba(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_izq.isEmpty()) {
			if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
				if(!casilla_arriba.isEmpty()) {
					if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
						if(!casilla_abajo.isEmpty()) {
							if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_DOWN);
								acciones.push(Types.ACTIONS.ACTION_DOWN);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_DOWN);
							acciones.push(Types.ACTIONS.ACTION_DOWN);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_UP);
						acciones.push(Types.ACTIONS.ACTION_UP);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_UP);
					acciones.push(Types.ACTIONS.ACTION_UP);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_LEFT);
				acciones.push(Types.ACTIONS.ACTION_LEFT);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_LEFT);
			acciones.push(Types.ACTIONS.ACTION_LEFT);
		}
	}
	
	/**
	 * Comportamiento reactivo para huir si hay peligro a la derecha
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroDerechaAbajo(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_izq.isEmpty()) {
			if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
				if(!casilla_abajo.isEmpty()) {
					if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
						if(!casilla_arriba.isEmpty()) {
							if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_UP);
								acciones.push(Types.ACTIONS.ACTION_UP);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_UP);
							acciones.push(Types.ACTIONS.ACTION_UP);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_DOWN);
						acciones.push(Types.ACTIONS.ACTION_DOWN);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_DOWN);
					acciones.push(Types.ACTIONS.ACTION_DOWN);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_LEFT);
				acciones.push(Types.ACTIONS.ACTION_LEFT);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_LEFT);
			acciones.push(Types.ACTIONS.ACTION_LEFT);
		}
	}
	
	/**
	 * 
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroIzquierdaArriba(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_der.isEmpty()) {
			if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
				if(!casilla_arriba.isEmpty()) {
					if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
						if(!casilla_abajo.isEmpty()) {
							if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_DOWN);
								acciones.push(Types.ACTIONS.ACTION_DOWN);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_DOWN);
							acciones.push(Types.ACTIONS.ACTION_DOWN);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_UP);
						acciones.push(Types.ACTIONS.ACTION_UP);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_UP);
					acciones.push(Types.ACTIONS.ACTION_UP);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_RIGHT);
				acciones.push(Types.ACTIONS.ACTION_RIGHT);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_RIGHT);
			acciones.push(Types.ACTIONS.ACTION_RIGHT);
		}
	}
	
	public void peligroIzquierdaAbajo(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_der.isEmpty()) {
			if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
				if(!casilla_abajo.isEmpty()) {
					if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
						if(!casilla_arriba.isEmpty()) {
							if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_UP);
								acciones.push(Types.ACTIONS.ACTION_UP);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_UP);
							acciones.push(Types.ACTIONS.ACTION_UP);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_DOWN);
						acciones.push(Types.ACTIONS.ACTION_DOWN);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_DOWN);
					acciones.push(Types.ACTIONS.ACTION_DOWN);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_RIGHT);
				acciones.push(Types.ACTIONS.ACTION_RIGHT);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_RIGHT);
			acciones.push(Types.ACTIONS.ACTION_RIGHT);
		}
	}
	
	/**
	 * 
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroArribaDerecha(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_abajo.isEmpty()) {
			if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
				if(!casilla_der.isEmpty()) {
					if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
						if(!casilla_izq.isEmpty()) {
							if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_LEFT);
								acciones.push(Types.ACTIONS.ACTION_LEFT);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_LEFT);
							acciones.push(Types.ACTIONS.ACTION_LEFT);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_RIGHT);
						acciones.push(Types.ACTIONS.ACTION_RIGHT);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_RIGHT);
					acciones.push(Types.ACTIONS.ACTION_RIGHT);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_DOWN);
				acciones.push(Types.ACTIONS.ACTION_DOWN);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_DOWN);
			acciones.push(Types.ACTIONS.ACTION_DOWN);
		}
	}
	/**
	 * 
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroArribaIzquierda(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_abajo = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+1];
		
		if(!casilla_abajo.isEmpty()) {
			if(casilla_abajo.get(0).itype == 10 || casilla_abajo.get(0).itype == 0 || casilla_abajo.get(0).itype == 5) {
				if(!casilla_izq.isEmpty()) {
					if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
						if(!casilla_der.isEmpty()) {
							if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_RIGHT);
								acciones.push(Types.ACTIONS.ACTION_RIGHT);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_RIGHT);
							acciones.push(Types.ACTIONS.ACTION_RIGHT);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_LEFT);
						acciones.push(Types.ACTIONS.ACTION_LEFT);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_LEFT);
					acciones.push(Types.ACTIONS.ACTION_LEFT);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_DOWN);
				acciones.push(Types.ACTIONS.ACTION_DOWN);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_DOWN);
			acciones.push(Types.ACTIONS.ACTION_DOWN);
		}
	}
	
	
	/**
	 * 
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroAbajoDerecha(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		
		if(!casilla_arriba.isEmpty()) {
			if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
				if(!casilla_der.isEmpty()) {
					if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
						if(!casilla_izq.isEmpty()) {
							if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_LEFT);
								acciones.push(Types.ACTIONS.ACTION_LEFT);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_LEFT);
							acciones.push(Types.ACTIONS.ACTION_LEFT);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_RIGHT);
						acciones.push(Types.ACTIONS.ACTION_RIGHT);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_RIGHT);
					acciones.push(Types.ACTIONS.ACTION_RIGHT);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_UP);
				acciones.push(Types.ACTIONS.ACTION_UP);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_UP);
			acciones.push(Types.ACTIONS.ACTION_UP);
		}
	}
	
	
	/**
	 * 
	 * @param estado
	 * @param pos_personaje
	 */
	public void peligroAbajoIzquierda(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> casilla_der = estado.getObservationGrid()[(int)pos_personaje.x+1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_izq = estado.getObservationGrid()[(int)pos_personaje.x-1][(int)pos_personaje.y];
		ArrayList<Observation> casilla_arriba = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-1];
		
		if(!casilla_arriba.isEmpty()) {
			if(casilla_arriba.get(0).itype == 10 || casilla_arriba.get(0).itype == 0 || casilla_arriba.get(0).itype == 5) {
				if(!casilla_izq.isEmpty()) {
					if(casilla_izq.get(0).itype == 10 || casilla_izq.get(0).itype == 0 || casilla_izq.get(0).itype == 5) {
						if(!casilla_der.isEmpty()) {
							if(casilla_der.get(0).itype == 10 || casilla_der.get(0).itype == 0 || casilla_der.get(0).itype == 5) {
								esperar();
								camino.clear();
							}
							else {
								acciones.push(Types.ACTIONS.ACTION_RIGHT);
								acciones.push(Types.ACTIONS.ACTION_RIGHT);
							}
						}
						else {
							acciones.push(Types.ACTIONS.ACTION_RIGHT);
							acciones.push(Types.ACTIONS.ACTION_RIGHT);
						}
					}
					else {
						acciones.push(Types.ACTIONS.ACTION_LEFT);
						acciones.push(Types.ACTIONS.ACTION_LEFT);
					}
				}
				else {
					acciones.push(Types.ACTIONS.ACTION_LEFT);
					acciones.push(Types.ACTIONS.ACTION_LEFT);
				}
			}
			else {
				acciones.push(Types.ACTIONS.ACTION_UP);
				acciones.push(Types.ACTIONS.ACTION_UP);
			}
		}
		else {
			acciones.push(Types.ACTIONS.ACTION_UP);
			acciones.push(Types.ACTIONS.ACTION_UP);
		}
	}
	
	
	
	
	
	
	/**
	 * 
	 * @param estado
	 * @param Vector2d
	 * @return
	 */
	public boolean escorpionCerca(StateObservation estado, Vector2d pos_personaje) {
		ArrayList<Observation> arriba_1 = estado.getObservationGrid()[(int) pos_personaje.x][(int) pos_personaje.y-1];
		ArrayList<Observation> izquierda_1 = estado.getObservationGrid()[(int) pos_personaje.x-1][(int) pos_personaje.y];
		ArrayList<Observation> derecha_1 = estado.getObservationGrid()[(int) pos_personaje.x+1][(int) pos_personaje.y];
		ArrayList<Observation> abajo_1 = estado.getObservationGrid()[(int) pos_personaje.x][(int) pos_personaje.y+1];
		
		ArrayList<Observation> izq_abaj = estado.getObservationGrid()[(int) pos_personaje.x-1][(int) pos_personaje.y+1];
		ArrayList<Observation> der_abaj = estado.getObservationGrid()[(int) pos_personaje.x+1][(int) pos_personaje.y+1];
		ArrayList<Observation> izq_arr = estado.getObservationGrid()[(int) pos_personaje.x-1][(int) pos_personaje.y-1];
		ArrayList<Observation> der_arr = estado.getObservationGrid()[(int) pos_personaje.x+1][(int) pos_personaje.y-1];
		
				
		if(pos_personaje.y <= 6) {
			if(derecha_1.isEmpty() == false) {
				if(derecha_1.get(0).itype == 10) {
					peligroDerechaAbajo(estado, pos_personaje);
					return true;
				}
			}
		}
		else{
			if(derecha_1.isEmpty() == false) {
				if(derecha_1.get(0).itype == 10) {
					peligroDerechaArriba(estado, pos_personaje);
					return true;
				}
			}
		}
		
		if(pos_personaje.y <= 6) {
			if(izquierda_1.isEmpty() == false) {
				if(izquierda_1.get(0).itype == 10) {
					peligroIzquierdaAbajo(estado, pos_personaje);
					return true;
				}
			}
		}
		else {
			if(izquierda_1.isEmpty() == false) {
				if(izquierda_1.get(0).itype == 10) {
					peligroIzquierdaArriba(estado, pos_personaje);
					return true;
				}
			}
		}
		
		//por abajo
		if(pos_personaje.x <= 15) {
			if(abajo_1.isEmpty() == false) {
				if(abajo_1.get(0).itype == 10) {
					peligroAbajoDerecha(estado, pos_personaje);
					return true;
				}
			}
		}
		else {
			if(abajo_1.isEmpty() == false) {
				if(abajo_1.get(0).itype == 10) {
					peligroAbajoIzquierda(estado, pos_personaje);
					return true;
				}
			}
		}
			
		if(pos_personaje.x <= 15) {
			if(arriba_1.isEmpty() == false) {
				if(arriba_1.get(0).itype == 10) {
					peligroArribaDerecha(estado, pos_personaje);
					return true;
				}
			}
		}
		else {
			if(arriba_1.isEmpty() == false) {
				if(arriba_1.get(0).itype == 10) {
					peligroArribaIzquierda(estado, pos_personaje);
					return true;
				}
			}
		}
		
		//Casos de laterales
		if(izq_abaj.isEmpty() == false) {
			if(izq_abaj.get(0).itype == 10) {
				peligroIzquierdaArriba(estado, pos_personaje);
				return true;
			}
		}
		if(der_abaj.isEmpty() == false) {
			if(der_abaj.get(0).itype == 10) {
				peligroDerechaArriba(estado, pos_personaje);
				return true;
			}
		}
		if(izq_arr.isEmpty() == false) {
			if(izq_arr.get(0).itype == 10) {
				peligroIzquierdaAbajo(estado, pos_personaje);
				return true;
			}
		}
		if(der_arr.isEmpty() == false) {
			if(der_arr.get(0).itype == 10) {
				peligroDerechaAbajo(estado, pos_personaje);
				return true;
			}
		}
		
		
		//Tengo en cuenta los bordes de los mapas suponiendo que son 12x25
		ArrayList<Observation> izquierda_2;
		ArrayList<Observation> derecha_2;
		ArrayList<Observation> abajo_2;
		ArrayList<Observation> arriba_2;
		
		ArrayList<Observation> izq_abaj_2;
		ArrayList<Observation> der_abaj_2;
		ArrayList<Observation> izq_arr_2;
		ArrayList<Observation> der_arr_2;
		
		ArrayList<Observation> izq_abaj_3;
		ArrayList<Observation> der_abaj_3;
		ArrayList<Observation> izq_arr_3;
		ArrayList<Observation> der_arr_3;
		ArrayList<Observation> izq_abaj_4;
		ArrayList<Observation> der_abaj_4;
		ArrayList<Observation> izq_arr_4;
		ArrayList<Observation> der_arr_4;
		
		
		if(pos_personaje.y <= 6) {
			if(pos_personaje.x-2 >= 0) {
				izquierda_2 = estado.getObservationGrid()[(int)pos_personaje.x-2][(int)pos_personaje.y];
				if(izquierda_2.isEmpty() == false) {
					if(izquierda_2.get(0).itype == 10) {
						if(izquierda_1.isEmpty()) {
							peligroIzquierdaAbajo(estado, pos_personaje);	//no voy a la izquierda porque voy de frente a él
							return true;
						}
					}
				}
			}
		}
		else {
			if(pos_personaje.x-2 >= 0) {
				izquierda_2 = estado.getObservationGrid()[(int)pos_personaje.x-2][(int)pos_personaje.y];
				if(izquierda_2.isEmpty() == false) {
					if(izquierda_2.get(0).itype == 10) {
						if(izquierda_1.isEmpty()) {
							peligroIzquierdaArriba(estado, pos_personaje);	//no voy a la izquierda porque voy de frente a él
							return true;
						}
					}
				}
			}
		}
		
		if(pos_personaje.y <= 6) {
			if(pos_personaje.x+2 <= 25) {
				derecha_2 = estado.getObservationGrid()[(int)pos_personaje.x+2][(int)pos_personaje.y];
				if(derecha_2.isEmpty() == false) {
					if(derecha_2.get(0).itype == 10) {
						if(derecha_1.isEmpty()) {
							peligroDerechaAbajo(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		else {
			if(pos_personaje.x+2 <= 25) {
				derecha_2 = estado.getObservationGrid()[(int)pos_personaje.x+2][(int)pos_personaje.y];
				if(derecha_2.isEmpty() == false) {
					if(derecha_2.get(0).itype == 10) {
						if(derecha_1.isEmpty()) {
							peligroDerechaArriba(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		if(pos_personaje.x <= 15) {
			if(pos_personaje.y-2 >= 0) {
				arriba_2 = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-2];
				if(arriba_2.isEmpty() == false) {
					if(arriba_2.get(0).itype == 10) {
						if(arriba_1.isEmpty()) {
							peligroArribaDerecha(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		else {
			if(pos_personaje.y-2 >= 0) {
				arriba_2 = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y-2];
				if(arriba_2.isEmpty() == false) {
					if(arriba_2.get(0).itype == 10) {
						if(arriba_1.isEmpty()) {
							peligroArribaIzquierda(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		if(pos_personaje.x <= 15) {
			if(pos_personaje.y+2 <= 12) {
				abajo_2 = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+2];
				if(abajo_2.isEmpty() == false) {
					if(abajo_2.get(0).itype == 10) {
						if(abajo_1.isEmpty()) {
							peligroAbajoDerecha(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		else {
			if(pos_personaje.y+2 <= 12) {
				abajo_2 = estado.getObservationGrid()[(int)pos_personaje.x][(int)pos_personaje.y+2];
				if(abajo_2.isEmpty() == false) {
					if(abajo_2.get(0).itype == 10) {
						if(abajo_1.isEmpty()) {
							peligroAbajoIzquierda(estado, pos_personaje);
							return true;
						}
					}
				}
			}
		}
		
		//casos laterales
		if(pos_personaje.x-2 >= 0 && pos_personaje.y-2 >= 0) {
			izq_arr_2 = estado.getObservationGrid()[(int) pos_personaje.x-2][(int) pos_personaje.y-2];
			if(izq_arr_2.isEmpty() == false) {
				if(izq_arr_2.get(0).itype == 10) {
					if(izquierda_1.isEmpty()) {
						peligroIzquierdaAbajo(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x-2 >= 0 && pos_personaje.y+2 <= 12) {
			izq_abaj_2 = estado.getObservationGrid()[(int) pos_personaje.x-2][(int) pos_personaje.y+2];
			if(izq_abaj_2.isEmpty() == false) {
				if(izq_abaj_2.get(0).itype == 10) {
					if(izquierda_1.isEmpty()) {
						peligroIzquierdaArriba(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x+2 <= 25 && pos_personaje.y+2 <= 12) {
			der_abaj_2 = estado.getObservationGrid()[(int) pos_personaje.x+2][(int) pos_personaje.y+2];
			if(der_abaj_2.isEmpty() == false) {
				if(der_abaj_2.get(0).itype == 10) {
					if(derecha_1.isEmpty()) {
						peligroDerechaArriba(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		
		if(pos_personaje.x+2 <= 25 && pos_personaje.y-2 >= 0) {
			der_arr_2 = estado.getObservationGrid()[(int) pos_personaje.x+2][(int) pos_personaje.y-2];
			if(der_arr_2.isEmpty() == false) {
				if(der_arr_2.get(0).itype == 10) {
					if(derecha_1.isEmpty()) {
						peligroDerechaAbajo(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		
		
		if(pos_personaje.x-1 >= 0 && pos_personaje.y-2 >= 0) {
			izq_arr_3 = estado.getObservationGrid()[(int) pos_personaje.x-1][(int) pos_personaje.y-2];
			if(izq_arr_3.isEmpty() == false) {
				if(izq_arr_3.get(0).itype == 10) {
					if(arriba_1.isEmpty()) {
						peligroArribaDerecha(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x-2 >= 0 && pos_personaje.y-1 >= 0) {
			izq_arr_4 = estado.getObservationGrid()[(int) pos_personaje.x-2][(int) pos_personaje.y-1];
			if(izq_arr_4.isEmpty() == false) {
				if(izq_arr_4.get(0).itype == 10) {
					if(izquierda_1.isEmpty()) {
						peligroIzquierdaAbajo(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		
		
		
		if(pos_personaje.x-2 >= 0 && pos_personaje.y+1 <= 12) {
			izq_abaj_3 = estado.getObservationGrid()[(int) pos_personaje.x-2][(int) pos_personaje.y+1];
			if(izq_abaj_3.isEmpty() == false) {
				if(izq_abaj_3.get(0).itype == 10) {
					if(izquierda_1.isEmpty()) {
						peligroIzquierdaArriba(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x-1 >= 0 && pos_personaje.y+2 <= 12) {
			izq_abaj_4 = estado.getObservationGrid()[(int) pos_personaje.x-1][(int) pos_personaje.y+2];
			if(izq_abaj_4.isEmpty() == false) {
				if(izq_abaj_4.get(0).itype == 10) {
					if(abajo_1.isEmpty()) {
						peligroAbajoDerecha(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		
		
		
		if(pos_personaje.x+1 <= 25 && pos_personaje.y+2 <= 12) {
			der_abaj_3 = estado.getObservationGrid()[(int) pos_personaje.x+1][(int) pos_personaje.y+2];
			if(der_abaj_3.isEmpty() == false) {
				if(der_abaj_3.get(0).itype == 10) {
					if(abajo_1.isEmpty()) {
						peligroAbajoIzquierda(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x+2 <= 25 && pos_personaje.y+1 <= 12) {
			der_abaj_4 = estado.getObservationGrid()[(int) pos_personaje.x+2][(int) pos_personaje.y+1];
			if(der_abaj_4.isEmpty() == false) {
				if(der_abaj_4.get(0).itype == 10) {
					if(derecha_1.isEmpty()) {
						peligroDerechaArriba(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		

		if(pos_personaje.x+2 <= 25 && pos_personaje.y-1 >= 0) {
			der_arr_3 = estado.getObservationGrid()[(int) pos_personaje.x+2][(int) pos_personaje.y-1];
			if(der_arr_3.isEmpty() == false) {
				if(der_arr_3.get(0).itype == 10) {
					if(derecha_1.isEmpty()) {
						peligroDerechaAbajo(estado, pos_personaje);
						return true;
					}
				}
			}
		}
		if(pos_personaje.x+1 <= 25 && pos_personaje.y-2 >= 0) {
			der_arr_4 = estado.getObservationGrid()[(int) pos_personaje.x+1][(int) pos_personaje.y-2];
			if(der_arr_4.isEmpty() == false) {
				if(der_arr_4.get(0).itype == 10) {
					if(arriba_1.isEmpty()) {
						peligroArribaIzquierda(estado, pos_personaje);
						return true;
					}
				}
			}
		}

		
		
		
		return false;
	}
	

	
	/**
	 * Decidiremos acá la acción que realizará el avatar
	 * @param estado
	 * @param elapsedTimer
	 * @return sig_accion Siguiente accion que realizará
	 */
	@Override
	public Types.ACTIONS act(StateObservation estado, ElapsedCpuTimer elapsedTimer){
		//Actualizamos el estado del pathfinder
		pathfinder.updateStateObs(estado);
		ArrayList<Observation> grid[][] = estado.getObservationGrid();
		
		//Obtenemos la posición del avatar
		Vector2d pos_avatar = new Vector2d(estado.getAvatarPosition().x / escala.x, estado.getAvatarPosition().y / escala.y);
		
		//Actualizo el plan de camino quitando la ultima decision realizada
		if(camino != null && ((pos_avatar.x != ultima_posicion.x) || (pos_avatar.y != ultima_posicion.y))) {
			if(!camino.isEmpty())
				camino.remove(0);
		}
		
		//Obtengo el número de gemas
		int n_gemas = 0;
		if(!estado.getAvatarResources().isEmpty()) {
			n_gemas = estado.getAvatarResources().get(6);
		}
		
		//Caso de comportamiento deliberativo, no reactivo
		ArrayList<Observation>[] hay_gemas = estado.getResourcesPositions(estado.getAvatarPosition());	//caso en el que no hay gemas solo escorpiones
		if(estoyEscapando == false || hay_gemas != null) {
			//Calculo camino si no tengo
			if(camino == null || camino.isEmpty()) {
				camino = nuevoCamino(pos_avatar, n_gemas, estado);
			}
		}
		
		if(escorpionCerca(estado, pos_avatar)) {
				voy_a_gemas = false;
		}
		else {
			voy_a_gemas = true;
		}
		
		
		acciones.clear();			//para tomar siempre la acción con preferencia

		estoyEscapando = false;
		
		if(escorpionCerca(estado, pos_avatar)) {
			//Tomo la última acción en el comportamiento reactivo
			Types.ACTIONS accion = acciones.pop();
			if(acciones.isEmpty()) {
				estoyEscapando = false;
				camino.clear();
			}
			hay_escorpiones = true;
			estoyEscapando = true;
			ultima_posicion = pos_avatar;
			ultima_accion = accion;
			if(camino != null)
				camino.clear();
			return accion;
		}
		
		
		//Hay un camino, es decir un plan
		else if(camino != null && !camino.isEmpty() && voy_a_gemas) {
			Types.ACTIONS sig_accion;
			Node sig_pos = camino.get(0);		//Tomo el primer nodo del camino
			//obtengo la casilla en el mapa que corresponde a ese nodo siguiente
			ArrayList<Observation> sig_casilla = grid[(int)sig_pos.position.x][(int)sig_pos.position.y];
			
			//La siguiente posicion va a variar la posicion en el eje x, es decir izq o derecha.
			if(sig_pos.position.x != pos_avatar.x) {
				//Si el camino nos lleva a la derecha
				if(sig_pos.position.x > pos_avatar.x) {
					sig_accion = Types.ACTIONS.ACTION_RIGHT;
				}
				else {
					sig_accion = Types.ACTIONS.ACTION_LEFT;
				}
			}
			else {
				// Si el avatar tiene que ir hacia abajo
				if(sig_pos.position.y > pos_avatar.y) {
					sig_accion = Types.ACTIONS.ACTION_DOWN;
				}
				else {
					sig_accion = Types.ACTIONS.ACTION_UP;
				}
			}
			
			ultima_accion = sig_accion;
			ultima_posicion = pos_avatar;
			return sig_accion;
		}
		else {	
			ultima_accion = Types.ACTIONS.ACTION_NIL;
			return Types.ACTIONS.ACTION_NIL;
		}
	}
	
}
