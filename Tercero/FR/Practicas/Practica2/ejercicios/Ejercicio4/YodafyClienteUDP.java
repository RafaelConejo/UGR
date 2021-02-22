//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.SocketException;

public class YodafyClienteUDP {

	public static void main(String[] args) {

		byte []buferEnvio= new byte[256];
		byte []buferRecepcion=new byte[256];
		int bytesLeidos=0;

		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;


		try {
			// Socket para la conexión UDP
			DatagramSocket socketServicio = new DatagramSocket();
			InetAddress direccion = InetAddress.getByName(host);
			// Creamos un socket que se conecte a "host" y "port":
			//////////////////////////////////////////////////////
			System.out.println("Conectando...");
			//////////////////////////////////////////////////////

			// Si queremos enviar una cadena de caracteres por un OutputStream, hay que pasarla primero
			// a un array de bytes:
			buferEnvio="Al monte del volcán debes ir sin demora".getBytes();

			//creamos el paquete
			//////////////////////////////////////////////////////
			DatagramPacket paquete = new DatagramPacket(buferEnvio,0,buferEnvio.length, direccion, port);
			//////////////////////////////////////////////////////
			//Enviamos el paquete
			//////////////////////////////////////////////////////
			socketServicio.send(paquete);
			//////////////////////////////////////////////////////

			// Leemos la respuesta del servidor. Para ello le pasamos un array de bytes, que intentará
			//////////////////////////////////////////////////////
			DatagramPacket paquete2 = new DatagramPacket(buferRecepcion, buferRecepcion.length);
			socketServicio.receive(paquete2);

			//////////////////////////////////////////////////////

			// MOstremos la cadena de caracteres recibidos:
			System.out.print("Recibido: ");
			String mensaje = new String(buferRecepcion).trim();
			System.out.println(mensaje);
			// Una vez terminado el servicio, cerramos el socket (automáticamente se cierran
			// el inpuStream  y el outputStream)
			//////////////////////////////////////////////////////
			socketServicio.close();
			//////////////////////////////////////////////////////

			// Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
