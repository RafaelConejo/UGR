package Practica1;

//Sustituyo la clase situada en core.game llamada Observation.java
//Esta clase nos permite saber el objeto que hay en el mapa.

public class Observation {
	private ObservationType tipo;
	
	//Posiciones en las cuadriculas x e y
	private int x;
	private int y;
	
	public Observation(int x, int y, ObservationType tipo) {
		this.x = x;
		this.y = y;
		this.tipo = tipo;
	}
	
	//Defino los objetos posibles de encontrar
	public Observation(core.game.Observation obs, int elemento) {
		int n_tipo = obs.itype;
		switch(n_tipo) {
		case 0:
			tipo = ObservationType.MURO;
			break;
		case 1:
			tipo = ObservationType.JUGADOR;
			break;
		case 5:
			tipo = ObservationType.SALIDA;
			break;
		case 6:
			tipo = ObservationType.GEMA;
			break;
		case 10:
			tipo = ObservationType.ESCORPION;
			break;
		}
		tools.Vector2d pos = obs.position;
		this.x = (int)(pos.x / elemento);
		this.y = (int)(pos.y / elemento);
	}
	
}
