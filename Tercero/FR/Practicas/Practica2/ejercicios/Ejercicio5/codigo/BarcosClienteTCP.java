import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class BarcosClienteTCP{

	public static void main(String[] args) {

      String buferEnvio = null;
		String buferRecepcion = null;

		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;

		// Socket para la conexión TCP
		Socket socketServicio=null;


		try {
			// Creamos un socket que se conecte a "hist" y "port":
			socketServicio = new Socket(host,port);

			PrintWriter outPrinter = new PrintWriter (socketServicio.getOutputStream(), true);
         BufferedReader inReader = new BufferedReader(new InputStreamReader (socketServicio.getInputStream()));

			System.out.println("\tBienvenido al simulador Hundir la flota");
			System.out.println("\tPrueba tu suerte intentando adivinar dónde estarán los barcos");

         //Introducir el tamanio de la matriz
			//1 parte
         BufferedReader tamanio = new BufferedReader(new InputStreamReader(System.in));
         boolean salir = true;
			int tam_tablero = 0;
			buferEnvio = "1000";
			outPrinter.println(buferEnvio);
         while(salir){
            System.out.println("\nPor favor ingrese el tamanio del tablero");
				buferEnvio = tamanio.readLine();
				outPrinter.println(buferEnvio);
				buferRecepcion = inReader.readLine();		//respuesta del servidor a ese tamanio
            if (buferRecepcion.equals("1001")){
					System.out.println("Error: Tamanio no permitido");
               tamanio = new BufferedReader(new InputStreamReader(System.in));
            }
            else if(buferRecepcion.equals("1002")){
					buferRecepcion = inReader.readLine();
					System.out.println("Tamaño OK");
					System.out.print("Bien, el tablero será de "+buferRecepcion);
               System.out.println("x"+buferRecepcion);
               salir = false;
					tam_tablero = Integer.parseInt(buferRecepcion); //Almaceno el tamanio que tendrá el tablero  para más adelante
            }
         }


			//2 parte. Mostrar matriz
			System.out.println("\nRecibido: ");
			for(int i = 0 ; i<tam_tablero;i++){
				System.out.println(inReader.readLine());
			}


			//3 parte. Creaccion de tamanio
			buferEnvio = "2000";
			outPrinter.println(buferEnvio);
			System.out.println("\nCómo desea la generaración del número de barcos: ");
			System.out.println("Si la desea aleatoria escriba 'ale'");
			System.out.println("Si la desea fija escribe 'fija'");
			System.out.println("Si introduce otra palabra se hará por defecto");

			BufferedReader genera = new BufferedReader(new InputStreamReader(System.in));
			buferEnvio = genera.readLine();	//leo la decisión
			if(buferEnvio.equals("ale")){
				buferEnvio = "2010";
			}
			else if(buferEnvio.equals("fija")){
				buferEnvio = "2020";
			}
			else{
				buferEnvio = "2030";
			}
			outPrinter.println(buferEnvio);	//se la envio al servidor para que la procese
			buferRecepcion = inReader.readLine();	//recibo el procesamiento del tipo
			if(buferRecepcion.equals("aleatoria")){		//caso de aleatoriedad
				System.out.println();
				System.out.println("Tiramos un dado para obtener el numero de barcos");
			}
			else if(buferRecepcion.equals("fija")){
			   BufferedReader num_barcos = new BufferedReader(new InputStreamReader(System.in));
			   salir = true;
			   while(salir){
			      System.out.println("Introduzca el número de barcos que desea: ");
			      buferEnvio = num_barcos.readLine();
					outPrinter.println(buferEnvio);
					buferRecepcion = inReader.readLine();		//respuesta del servidor a ese tamanio
	            if (buferRecepcion.equals("2021")){
						System.out.println("Error: Cantidad no permitida.");
	               num_barcos = new BufferedReader(new InputStreamReader(System.in));
	            }
	            else if(buferRecepcion.equals("2022")){
						System.out.println("Cantidad permitida.");
	               salir = false;
			   	}
			   //outPrinter.println(buferEnvio);
				}
			}
			else if(buferRecepcion.equals("por defecto")){
				System.out.println("Se implementa por defecto");
				System.out.println("Creamos la mitad de barcos respecto al numero de huecos disponibles");
			}

			//servidor me da el numero de barcos
			buferEnvio = "2040";
			outPrinter.println(buferEnvio);
			buferRecepcion = inReader.readLine();
			System.out.println("\nBarcos creados: " + buferRecepcion);


			//4 parte. Adivinar posiciones
			buferEnvio = "3000";
			outPrinter.println(buferEnvio);
			System.out.println("\nRecuerda que las posiciones empiezan a contar desde 0");
			System.out.println("Puedes comenzar a adivinar la posicion de los barcos");
			System.out.println();
			BufferedReader fila = new BufferedReader(new InputStreamReader(System.in));
			BufferedReader columna = new BufferedReader(new InputStreamReader(System.in));
			BufferedReader fin = new BufferedReader(new InputStreamReader(System.in));
			salir = true;
			boolean salir_fila = true;
			boolean salir_col = true;
			while(salir){
					buferRecepcion = null;
					System.out.println("Si desea finalizar el juego introduzca 'fin'");
					System.out.println("Si desea seguir introduzca cualquier otra cosa por teclado");
					buferEnvio = fin.readLine();
					if(buferEnvio.equals("fin")){
						buferEnvio = "3001";
					}
					outPrinter.println(buferEnvio);
					buferRecepcion = inReader.readLine();
					//si el cliente decide acabar se saldrá de todos los bucles
					if(buferRecepcion.equals("3002")){
						salir = false;
						salir_fila = false;
						System.out.println("\nFin de adivinar posiciones");
					}

					else if(buferRecepcion.equals("3003")){
						System.out.println("\nContinuemos");
						while(salir_fila){
							System.out.println("Introduzca la fila donde desea disparar:");
							buferEnvio = fila.readLine();
							outPrinter.println(buferEnvio);
							buferRecepcion = inReader.readLine();
							if(buferRecepcion.equals("3004")){
								System.out.println("Error: Fila incorrecta");
								fila = new BufferedReader(new InputStreamReader(System.in));
							}
							else if(buferRecepcion.equals("3005")){
								System.out.println("Fila valida");
								salir_col = true;
								salir_fila = false;
							}
						}

						buferEnvio = "3010";
						outPrinter.println(buferEnvio);
						while(salir_col){
							System.out.println("\nIntroduzca la columna donde desea disparar:");
							buferEnvio = columna.readLine();
							outPrinter.println(buferEnvio);
							buferRecepcion = inReader.readLine();
							if(buferRecepcion.equals("3011")){
								System.out.println("Error: Columna incorrecta");
								columna = new BufferedReader(new InputStreamReader(System.in));
							}
							else if(buferRecepcion.equals("3012")){
								System.out.println("Columna valida");
								salir_col = false;
								salir_fila = true;
							}
						}

						buferEnvio = "3020";
						outPrinter.println(buferEnvio);
						buferRecepcion = inReader.readLine();
						if(buferRecepcion.equals("hundido")){
							System.out.println("\nBingo!! Le has dado");
						}
						else if(buferRecepcion.equals("no hundido")){
							System.out.println("\nLo siento no has acertado el disparo");
						}
						else if(buferRecepcion.equals("repetido")){
							System.out.println("\nYa habías disparado en esa posicion");
						}

						buferRecepcion = inReader.readLine();
						if(buferRecepcion.equals("3021")){
							System.out.println("\nHas hundido Todos los barcos!!");
							salir = false;
						}
						else if(buferRecepcion.equals("3022")){
							System.out.println("\nAún quedan barcos!!");
						}
					}
					fin = new BufferedReader(new InputStreamReader(System.in));
					fila = new BufferedReader(new InputStreamReader(System.in));
					columna = new BufferedReader(new InputStreamReader(System.in));

					System.out.println("\nRecibido: ");
					for(int i = 0 ; i<tam_tablero;i++){
						System.out.println(inReader.readLine());
					}
					System.out.println();
			}



			// Aunque le indiquemos a TCP que queremos enviar varios arrays de bytes, sólo
			// los enviará efectivamente cuando considere que tiene suficientes datos que enviar...
			// Podemos usar "flush()" para obligar a TCP a que no espere para hacer el envío:
			outPrinter.flush();

			// Leemos la respuesta del servidor. Para ello le pasamos un array de bytes, que intentará
			// rellenar. El método "read(...)" devolverá el número de bytes leídos.



			// MOstremos la cadena de caracteres recibidos:


			// Una vez terminado el servicio, cerramos el socket (automáticamente se cierran
			// el inpuStream  y el outputStream)
			socketServicio.close();

			// Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
