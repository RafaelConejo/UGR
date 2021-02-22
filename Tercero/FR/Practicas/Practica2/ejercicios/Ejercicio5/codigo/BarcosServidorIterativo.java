import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;


//
// BarcosServidorIterativo
// Rafael Vázquez Conejo
//
public class BarcosServidorIterativo {

	public static void main(String[] args) {
		// Puerto de escucha
		int port=8989;
		// array de bytes auxiliar para recibir o enviar datos.
		byte []buffer=new byte[256];
		// Número de bytes leídos
		int bytesLeidos=0;

        	ServerSocket socketServidor;

		try {
			// Abrimos el socket en modo pasivo, escuchando el en puerto indicado por "port"
			socketServidor = new ServerSocket(port);

			System.out.println("\tServidor similador de hundir la flota\n");
			// Mientras ... siempre!
			do {
				// Aceptamos una nueva conexión con accept()
				Socket socketServicio = null;
                try{
                    socketServicio = socketServidor.accept();
                } catch (IOException e){
                    System.out.println("Error: no se pudo aceptar la conexión solicitada");
                }

				// Creamos un objeto de la clase ProcesadorBarcos, pasándole como
				// argumento el nuevo socket, para que realice el procesamiento
				// Este esquema permite que se puedan usar hebras más fácilmente.
				ProcesadorBarcos procesador=new ProcesadorBarcos(socketServicio);
				procesador.procesa();

			} while (true);

		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}

	}

}