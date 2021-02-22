import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Random;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Arrays;

public class ProcesadorBarcos {
	// Referencia a un socket para enviar/recibir las peticiones/respuestas
	private Socket socketServicio;
	// stream de lectura (por aquí se recibe lo que envía el cliente)
	private InputStream inputStream;
	// stream de escritura (por aquí se envía los datos al cliente)
	private OutputStream outputStream;

	// Para que la respuesta sea siempre diferente, usamos un generador de números aleatorios.
	private Random random;

	char matriz[][] = new char['-']['-'];
	char matriz_aux[][] = new char['-']['-'];
	String tablero;
	int tam;
	int num_barcos;
	int pos_fil, pos_col;

	// Constructor que tiene como parámetro una referencia al socket abierto en por otra clase
	public ProcesadorBarcos(Socket socketServicio) {
		this.socketServicio=socketServicio;
		random=new Random();
	}


	// Aquí es donde se realiza el procesamiento realmente:
	void procesa(){

		String datosRecibidos;

		try {
			// Obtiene los flujos de escritura/lectura
			PrintWriter outPrinter = new PrintWriter(socketServicio.getOutputStream(), true);
			BufferedReader inReader = new BufferedReader(new InputStreamReader (socketServicio.getInputStream()));

			//1 parte
			boolean salir = true;
			datosRecibidos = inReader.readLine();
			if(datosRecibidos.equals("1000")){	//permite el servidor ingresar tamanio
				while(salir){
					datosRecibidos = inReader.readLine();
					tam = Integer.parseInt(datosRecibidos);
					if(tam <= 0){
						datosRecibidos = "1001";	//error al ingresar tamanio
					}
					else{
						datosRecibidos = "1002";		//tamanio correcto y lo envia
						outPrinter.println(datosRecibidos);
						datosRecibidos = Integer.toString(tam);
						salir = false;
					}
					outPrinter.println(datosRecibidos);
				}
			}
			System.out.print("Se ha establecido el tablero: "+tam);
			System.out.println("x"+tam);
			generarTablero(tam);

			mostrarTablero();
			System.out.println();


			//2 parte
			StringBuffer tablero = new StringBuffer("");
			tablero = mostrarTableroCliente(tablero);
			//envio al cliente
			datosRecibidos = tablero.toString();
			outPrinter.print(datosRecibidos);
			outPrinter.flush();



			//3 parte
			datosRecibidos = inReader.readLine();
			if(datosRecibidos.equals("2000")){
				datosRecibidos = inReader.readLine();	//recibe el tipo de generacion
				String tipo = null;	//string para mostrar el tipo de genereccion
				if(datosRecibidos.equals("2010")){
					datosRecibidos = "aleatoria";
					outPrinter.println(datosRecibidos);	//se lo envio al cliente
					num_barcos = (int) (Math.random()*(tam*tam+1));;	//genero numero aleatorio
					tipo = "aleatoria";
				}
				else if(datosRecibidos.equals("2020")){
					datosRecibidos = "fija";
					outPrinter.println(datosRecibidos);	//se lo envio al cliente
					salir = true;
					while(salir){
						datosRecibidos = inReader.readLine();
						num_barcos = Integer.parseInt(datosRecibidos);
						if(num_barcos <= 0 || num_barcos > (tam*tam)){
							datosRecibidos = "2021";
						}
						else{
							datosRecibidos = "2022";		//numero correcto y lo envia
							salir = false;
						}
						outPrinter.println(datosRecibidos);
					}
					tipo = "fija";
				}
				else if(datosRecibidos.equals("2030")){
					datosRecibidos = "por defecto";
					outPrinter.println(datosRecibidos);//se lo envio al cliente
					num_barcos = (tam*tam)/2;
					tipo = "por defecto";

				}

				System.out.println("Numero de barcos generado: "+num_barcos);
				System.out.println("De forma: "+tipo);
				//para el cliente
				datosRecibidos = inReader.readLine();
				if(datosRecibidos.equals("2040")){						//compruebo y si es correcto envio
					datosRecibidos = Integer.toString(num_barcos);	//el numero de barcos fimal
					outPrinter.println(datosRecibidos);
				}

				//creo en el servvidor el tablero con los barcos en la matriz_aux
				generarBarcosTablero(num_barcos);
				mostrarTableroAux();
			}

			//parte 4. comienza a hundir barcos
			datosRecibidos = inReader.readLine();
			if(datosRecibidos.equals("3000")){
				String hundir;
				boolean todos_Hundidos;
				boolean salir_fila = true, salir_col = true;
				salir = true;
				while(salir){
					//datosRecibidos = null;
					datosRecibidos = inReader.readLine();

					if(datosRecibidos.equals("3001")){
						salir = false;
						datosRecibidos = "3002";
						outPrinter.println(datosRecibidos);
					}
					else{
						datosRecibidos = "3003";
						outPrinter.println(datosRecibidos);
						while(salir_fila){
							datosRecibidos = inReader.readLine();
							pos_fil = Integer.parseInt(datosRecibidos);
							if(pos_fil < 0 || pos_fil >= tam){	//comprueba fila
								datosRecibidos = "3004";
								outPrinter.println(datosRecibidos);
							}
							else{
								datosRecibidos = "3005";
								outPrinter.println(datosRecibidos);
								salir_fila = false;
								salir_col = true;
							}
						}
						datosRecibidos = inReader.readLine();
						if(datosRecibidos.equals("3010")){
							while(salir_col){
								datosRecibidos = inReader.readLine();
								pos_col = Integer.parseInt(datosRecibidos);
								if(pos_col < 0 || pos_col >= tam){
									datosRecibidos = "3011";
									outPrinter.println(datosRecibidos);
								}
								else{
									datosRecibidos = "3012";
									outPrinter.println(datosRecibidos);
									salir_col = false;
									salir_fila = true;
								}
							}
						}
						//compruebo primero que no he repedido la posicion
						//miro la posicion en la matriz aux y compruebo si le ha dado a un barco
						datosRecibidos = inReader.readLine();
						if(datosRecibidos.equals("3020")){	//comprubo si he hundido
							hundir = hundirBarcos(pos_fil, pos_col, tablero);
							datosRecibidos = hundir;
							outPrinter.println(datosRecibidos);
							//compruebo si he hundido todos los barcos
							todos_Hundidos = todosHundidos();
							if(todos_Hundidos){
								datosRecibidos = "3021";
								outPrinter.println(datosRecibidos);
								salir = false;
							}
							else{
								datosRecibidos = "3022";
								outPrinter.println(datosRecibidos);
							}
						}
						//muestro la matriz auxiliar en el servidor
						System.out.println();
						mostrarTableroAux();
					}
					//le envio el estado del tablero al cliente
					//tablero = mostrarTableroCliente(tablero);
					datosRecibidos = tablero.toString();
					outPrinter.print(datosRecibidos);
					outPrinter.flush();

				}
			}





		} catch (IOException e) {
			System.err.println("Error al obtener los flujos de entrada/salida.");
		}

	}

	// Genero la matriz:
	private void generarTablero(int tam) {
		for(int i = 0; i < tam; i++){
			for (int j = 0; j < tam; j++){

				matriz[i][j] = '-';
				matriz_aux[i][j] = '-';
			}
		}
	}
	//muestro el tablero en el servidor
	private void mostrarTablero() {
		for (int i = 0; i < tam; i++) {
		  System.out.print("|");
		  for (int j = 0; j < tam; j++) {
			 System.out.print (matriz[i][j]);
			 if (j!=matriz[i].length-1) System.out.print("\t");
		  }
		  System.out.println("|");
		}
	}
	//muestro el tablero al cliente
	private StringBuffer mostrarTableroCliente(StringBuffer tablero){
		for (int i = 0; i < tam; i++) {
		  int contador = 0;
		  for (int j = 0; j < tam+1; j++) {
			  contador++;
			  if(contador == tam+1){
				  tablero.insert(j,(char)10);
			  }
			  else
			  	  tablero.insert(j, "-");
		  }
		}
		return tablero;
	}
	//coloco los barcos de forma aleatoria en el tablero
	private void generarBarcosTablero(int num_barcos){
		for(int i = 0; i < num_barcos; i++){
			int num_random_fil = (int) (Math.random()*tam);
			int num_random_col = (int) (Math.random()*tam);
			if(matriz_aux[num_random_fil][num_random_col] != '*')
				matriz_aux[num_random_fil][num_random_col] = '*';
			else
				i--;
		}
	}
	//para ir mostrando el tablero en el servidor
	private void mostrarTableroAux(){
		for (int i = 0; i < tam; i++) {
		  System.out.print("|");
		  for (int j = 0; j < tam; j++) {
			 System.out.print (matriz_aux[i][j]);
			 if (j!=matriz_aux[i].length-1) System.out.print("\t");
		  }
		  System.out.println("|");
		}
	}
	//modifica el tablero del cliente
	private void modificarTableroCliente(int pos_fil, int pos_col, StringBuffer tablero){
		int pos_modificar = pos_fil*(tam+1)+pos_col;
		//tablero.delete(pos_modificar, pos_modificar);
		//tablero.insert(pos_modificar, 'X');
		tablero.setCharAt(pos_modificar, 'X');
	}
	private boolean todosHundidos(){
		boolean todos_Hundidos = true;
		for(int i = 0; i < tam; i++){
			for (int j = 0; j < tam; j++){
				if(matriz_aux[i][j] == '*'){
					todos_Hundidos = false;
					return todos_Hundidos;
				}
			}
		}
		return todos_Hundidos;
	}
	//estado del tablero
	private String hundirBarcos(int pos_fil, int pos_col, StringBuffer tablero){
		String hundir;
		if(matriz_aux[pos_fil][pos_col] == '*'){	//pongo X donde disparo y hay barco
			matriz[pos_fil][pos_col] = 'X';
			modificarTableroCliente(pos_fil, pos_col, tablero);
			matriz_aux[pos_fil][pos_col] = 'X';	//tambien actualizo la matriz del servidor
			hundir = "hundido";
		}
		else if(matriz_aux[pos_fil][pos_col] == '?' || matriz_aux[pos_fil][pos_col] == 'X'){
			hundir = "repetido";
		}
		else{		//pongo * donde disparo y no hay barcos
			matriz[pos_fil][pos_col] = '*';
			if(matriz_aux[pos_fil][pos_col] != 'X')
				matriz_aux[pos_fil][pos_col] = '?';
			hundir = "no hundido";
		}	//la matriz_aux marca como ? donde ya ha disparado y no habia barco
		return hundir;
	}

}
