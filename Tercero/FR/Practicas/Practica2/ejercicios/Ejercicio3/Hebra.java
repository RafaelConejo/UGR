import java.net.Socket;

public class Hebra extends Thread{
	//Creo un objeto de la clase ProcesadorYodafy, con el nuevo socket de argumento
	ProcesadorYodafy procesador;

	Hebra(Socket socketConexion){
		procesador = new ProcesadorYodafy(socketConexion);
	}
	//este metodo se ejecutará tras la llmada a start() en YodaServidorConcurrente
	public void run(){
		procesador.procesa();
	}
}
