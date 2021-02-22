#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 17:27:33 2020

@author: rafa
"""


#############################
#####     LIBRERIAS     #####
#############################

import numpy as np
import matplotlib.pyplot as plt
import random
import math


#-------------------------------------------------------------------------------#
#---------------------- Ejercicio sobre regresión lineal -----------------------#
#-------------------------------------------------------------------------------#



#------------------------------Ejercicio 1 -------------------------------------#


# Funcion para leer los datos
def readData(file_x, file_y):
	# Leemos los ficheros	
	datax = np.load(file_x)
	datay = np.load(file_y)
	y = []
	x = []
	
	# Solo guardamos los datos cuya clase sea la 1 o la 5
	for i in range(0,datay.size):
		if datay[i] == 5 or datay[i] == 1:
			if datay[i] == 5:
				y.append(1)
			else:
				y.append(-1)
			x.append(np.array([1, datax[i][0], datax[i][1]]))
			
	x = np.array(x, np.float64)
	y = np.array(y, np.float64)
	
	return x, y



	
# Error cuadrático dado un modelo lineal y un conjunto de datos etiquetado
def Err(w,x,y):
    y = y.reshape(-1, 1)
    #dot me permite realizar el producto de dos matrices, en general de dos vectores
    v = (x.dot(w) - y) ** 2
    #obtengo la media aritmetica gracias a mean() función de numpy
    media_arit = np.mean(v, axis = 0)
    #pongo -1 porque así calcula el tamaño adecuado del array
    media_arit = media_arit.reshape(-1, 1)
    
    return media_arit




# Gradiente del error cuadrático
# Se le pasa como parámetros el vector de pesos de nuestro modelo lineal,
# los datos y sus respectivas etiquetas (valores verdaderos)
def grad_err(w,x,y):
    y = y.reshape(-1,1)
    v = (x.dot(w)-y)*x
    #De nuevo realizo la media aritmetica
    media_arit = np.mean(v * 2, axis = 0)
    #pongo -1 porque así calcula el tamaño adecuado del array
    media_arit = media_arit.reshape(-1, 1)
    
    return media_arit



# Implementación del Gradiente Descendiente Estocástico
def sgd(grad_fun, error, w, x, y, lr=0.01, max_iters = 10000, epsilon = 10**(-20), tam_minibatch = 32):
    it = 0
    #Selecciono un batch de forma aleatoria
    #La función choice elige un valor al azar en un conjunto de elemento dado
    batch = np.random.choice(y.size, tam_minibatch, replace=False)
    x_batch = x[batch,:]
    y_batch = y[batch]
    
    while it <= max_iters and error(w, x, y) > epsilon:
        w -= lr * grad_fun(w, x_batch, y_batch)
        it += 1

    return w, it
	



# Algoritmo pseudoinversa	
def pseudoinverse(x, y):
    return np.dot(np.linalg.pinv(x),y)



	
# Lectura de los datos de entrenamiento
x, y = readData('datos/X_train.npy', 'datos/y_train.npy')
# Lectura de los datos para el test
x_test, y_test = readData('datos/X_test.npy', 'datos/y_test.npy')







print ('EJERCICIO SOBRE REGRESION LINEAL\n')
print ('Ejercicio 1\n')

# Vector con valores aleatorios de pesos inicial
np.random.seed(49137372)
w = np.zeros((3,1))

lr = 0.01
tam_minibatch = 32
max_iters = 6000



# Errores de la regresión con gradiente descendiente estocástico
w, itera = sgd(grad_err, Err, w, x, y, lr, max_iters, 10 ** (-20), tam_minibatch)


print ('Bondad del resultado para grad. descendente estocastico:\n')
print ("Ein: ", Err(w , x, y))
print ("Eout: ",  Err(w, x_test, y_test))

print ('\nGrafica de resultados\n')
ver = input ('Si desea verla introduzca "s" por teclado\n')
if ver == "s":
    recta_x = np.linspace(0, 1, y.size)
    recta_y = (-w[0] - w[1] * recta_x) / w[2]
    
    plt.scatter(x[:,1], x[:,2], c=y)
    plt.plot(recta_x, recta_y, 'r-', linewidth=2, label='SGD')
    
    plt.title('\nRegresión lineal con el SGD en el Train')
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetria')
    plt.legend()
    plt.show()
    
    
    recta_x = np.linspace(0, 1, y_test.size)
    recta_y = (-w[0] - w[1] * recta_x) / w[2]
    
    plt.scatter(x_test[:,1], x_test[:,2], c=y_test)
    plt.plot(recta_x, recta_y, 'r-', linewidth=2, label='SGD')
    
    plt.title('\nRegresión lineal con el SGD en el Test')
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetria')
    plt.legend()
    plt.show()

input("\n--- Pulsar tecla para continuar ---\n")






# Algoritmo Pseudoinversa

w = pseudoinverse(x, y)
w = w.reshape((3,1))

print ('\nBondad del resultado para el algoritmo de la pseudoinversa:\n')
print ("Ein: ", Err(w , x, y))
print ("Eout: ", Err(w, x_test, y_test))

print ('\nGrafica de resultados\n')
ver = input ('Si desea verla introduzca "s" por teclado\n')
if ver == "s":
    recta_x = np.linspace(0, 1, y.size)
    recta_y = (-w[0] - w[1] * recta_x) / w[2]
    
    plt.scatter(x[:,1], x[:,2], c=y)
    plt.plot(recta_x, recta_y, 'b-', linewidth=2, label='Pseudoinversa')
    
    plt.title('\nRegresión lineal con Pseudo-inversa en el Train')
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetria')
    plt.legend()
    plt.show()
    
    recta_x = np.linspace(0, 1, y_test.size)
    recta_y = (-w[0] - w[1] * recta_x) / w[2]
    
    plt.scatter(x_test[:,1], x_test[:,2], c=y_test)
    plt.plot(recta_x, recta_y, 'b-', linewidth=2, label='Pseudoinversa')
    
    plt.title('\nRegresión lineal con el Pseudoinversa en el Test')
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetria')
    plt.legend()
    plt.show()





print ('\nA continuación presento gráficas para el desarrollo de la memoria\n')
ver = input ('Si desea verlas introduzca "s" por teclado\n')
if ver == "s":
    #Gráfica de la pseudoinversa de training
    label1 = -1
    label5 = 1
    
    print("Gráfica de Training: \n")
    plt.title("Gráfica de training")
    et_1 = []
    et_1 = x[y == label1]
    et_5 = []
    et_5 = x[y == label5]
    
    plt.scatter(et_1[:, 1], et_1[:, 2], c='red', label='Número 1', linewidth='1')
    plt.scatter(et_5[:, 1], et_5[:, 2], c='blue', label='Número 5', linewidth='1')
    
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetría') 
    plt.legend()
    plt.show()
    
    print("Gráfica de Test:\n")
    plt.title("Gráfica de test")
    et_1 = []
    et_1 = x_test[y_test == label1]
    et_5 = []
    et_5 = x_test[y_test == label5]
    
    plt.scatter(et_1[:, 1], et_1[:, 2], c='red', label='Número 1', linewidth='1')
    plt.scatter(et_5[:, 1], et_5[:, 2], c='blue', label='Número 5', linewidth='1')
    plt.xlabel('Intensidad promedio')
    plt.ylabel('Simetría')
    
    plt.legend()
    plt.show()


    print ('\nContinuar con ejercicio 2\n')
    input("\n--- Pulsar tecla para continuar ---\n")










#------------------------------Ejercicio 2 -------------------------------------#

# Simula datos en un cuadrado [-size,size]x[-size,size]
def simula_unif(N, d, size):
	return np.random.uniform(-size,size,(N,d))

# Función para añadir una columna de 1
def add_col_1(data):
    unos = np.full((len(data),1),1.0)
    unir = np.concatenate((unos,data),axis=1)
    
    return unir
	

print ('Ejercicio 2\n')
print ('Muestra N = 1000, cuadrado [-1,1]x[-1,1]')
# EXPERIMENTO	
# a) Muestra de entrenamiento N = 1000, cuadrado [-1,1]x[-1,1]	

print ('\nApartado a:\n')
muestra = simula_unif(1000, 2, 1.0)

plt.clf()
plt.scatter(muestra[:,0], muestra[:,1], c="Blue")
plt.title("Muestra aleatoria uniforme de 1000 elementos")
plt.xlabel("x1")
plt.ylabel("x2")
plt.show()






print ('\nApartado b:\n')
input("--- Pulsar tecla para continuar ---\n")

# b) 
#funcion para generar etiquetas para la muestra (1 o -1)
def asignarEtiqueta(x1, x2):
    f = (x1 - 0.2)**2 + x2**2 - 0.6
    return math.copysign(1,f)

# Generamos las etiquetas para la muestra, vectorizando la función anterior
etiquetas_en_vector = np.vectorize(asignarEtiqueta)
muestra_antes_ruido = etiquetas_en_vector(muestra[:,0],muestra[:,1])

print ('Representamos la muestra antes del ruido\n')
plt.clf()
plt.scatter(muestra[muestra_antes_ruido==1,0], muestra[muestra_antes_ruido==1,1], c="Blue", label='y = 1')
plt.scatter(muestra[muestra_antes_ruido==-1,0], muestra[muestra_antes_ruido==-1,1], c="Red", label='y = -1')
plt.title("Muestra anterior con etiquetas asignadas")
plt.xlabel("x1")
plt.ylabel("x2")
plt.legend()
plt.show()

# Introducimos ruido aleatorio, es decir, cambiamos el signo de las etiquetas de un 10% de la muestra
for i in range(0,len(muestra_antes_ruido)//10, 1):
    rand_i = random.randint(0,len(muestra_antes_ruido)-1)
    muestra_antes_ruido[rand_i] = -muestra_antes_ruido[rand_i]


print ('A continuación representamos la muestra después del ruido\n')
input("--- Pulsar tecla para continuar ---\n")
# Pintamos la muestra con ruido
plt.clf()
plt.scatter(muestra[muestra_antes_ruido==1,0], muestra[muestra_antes_ruido==1,1], c="Blue", label='y = 1')
plt.scatter(muestra[muestra_antes_ruido==-1,0], muestra[muestra_antes_ruido==-1,1], c="Red", label='y = -1')
plt.xlabel("x1")
plt.ylabel("x2")
plt.title("Muestra aleatoria uniforme de 1000 elementos etiquetados con ruido")
plt.legend()
plt.show()









print ('\nApartado c:\n')
input("--- Pulsar tecla para continuar ---\n")
# -------------------------------------------------------------------
#c)
#añadimos un 1 a todos los vectores de características
muestra_unos = add_col_1(muestra)
# Ajustamos un modelo de regresión lineal con el 
# Gradiente descendiente Estocástico
w_inicial = np.zeros((3,1))
max_iters = 1000
lr = 0.01
tam_minibatch = 32


w, it = sgd(grad_err, Err, w_inicial, muestra_unos, muestra_antes_ruido, lr, max_iters, 10**(-20), tam_minibatch)
Ein = Err(w, muestra_unos, muestra_antes_ruido)

print("Ajuste obtenido con SGD para la muestra aleatoria de 1000 elementos:")
print(w)
print ("Ein: ", Ein)








print ('\nApartado d:\n')
input("--- Pulsar tecla para continuar ---\n")

# d) Ejecutar el experimento 1000 veces

print("Experimento: 1000 muestras aleatorias de 1000")

# Listas que guardará los errores dentro y
# fuera de la muestra correspondientemente
E_in_lista = []
E_out_lista = []

# Repetimos el experimento anterior 1000 veces

for i in range(0,1000,1):
    print("Iteracion %d ..." % i)
    # Generamos training
    x_tra = simula_unif(1000, 2, 1.0)
    y_tra = etiquetas_en_vector(x_tra[:,0],x_tra[:,1])
    x_tra_unos = add_col_1(x_tra)
    
    #cambios de etiqueta en un 10%
    for j in range(0,len(y_tra)//10, 1):
        #aleatorios
        rand_j = random.randint(0,len(y_tra) - 1)
        y_tra[rand_j] = -y_tra[rand_j]
        
    w, it = sgd(grad_err, Err, w_inicial, x_tra_unos, y_tra, lr, max_iters, 10**(-20), tam_minibatch)
    ein = Err(w, x_tra_unos, y_tra)
    E_in_lista.append(ein)
    
    # Repetimos proceso con el test
    x_test = simula_unif(1000, 2, 1.0)
    y_test = etiquetas_en_vector(x_test[:,0],x_test[:,1])
    x_tst_unos = add_col_1(x_test)
    
    for j in range(0,len(y_test)//10,1):
        rand_j = random.randint(0, len(y_test) - 1)
        y_test[rand_j] = -y_test[rand_j]
    
    eout = Err(w, x_tst_unos, y_test)
    E_out_lista.append(eout)

#convierto lista en array
errores_in = np.array(E_in_lista)
errors_out = np.array(E_out_lista)
#realizo las medias
media_in = np.mean(errores_in)
media_out = np.mean(errors_out)




print ('Errores Ein y Eout medios tras 1000reps del experimento:\n')
print ("Ein media: ", media_in)
print ("Eout media: ", media_out)

input("\n--- Pulsar tecla para continuar ---\n")




print ('\nApartado c con segundas caracteristicas:\n')
input("--- Pulsar tecla para continuar ---\n")
 
#defino el vector de nuevas caracteristicas 
def nuevas_carac(x1, x2):
    unos = np.full((len(x1),1),1.0)
    unir = np.column_stack((unos, x1, x2, x1*x2, x1*x1, x2*x2))
    
    return unir


# Ajustamos un modelo de regresión lineal con el 
# Gradiente descendiente Estocástico

max_iters = 1000
lr = 0.01

nueva = nuevas_carac(muestra[:,0], muestra[:,1])

w_inicial = np.zeros((nueva.shape[1], 1))


w, it = sgd(grad_err, Err, w_inicial, nueva, muestra_antes_ruido, lr, max_iters, 10**(-20), 32)
Ein = Err(w, nueva, muestra_antes_ruido)

print("Ajuste obtenido con SGD para la muestra aleatoria de 1000 elementos:")
print(w)
print ("Ein: ", Ein)




print ('\nApartado d con segundas caracteristicas:\n')
input("--- Pulsar tecla para continuar ---\n")

# d) Ejecutar el experimento 1000 veces

print("Experimento: 1000 muestras aleatorias de 1000")

# Listas que guardará los errores dentro y
# fuera de la muestra correspondientemente
E_in_lista = []
E_out_lista = []

# Repetimos el experimento anterior 1000 veces
for i in range(0,1000,1):
    print("Iteracion %d ..." % i)
    # Generamos training
    x_tra = simula_unif(1000, 2, 1.0)
    y_tra = etiquetas_en_vector(x_tra[:,0],x_tra[:,1])
    x_tra_nueva = nuevas_carac(x_tra[:,0], x_tra[:,1])
    
    #cambios de etiqueta en un 10%
    for j in range(0,len(y_tra)//10, 1):
        #aleatorios
        rand_j = random.randint(0,len(y_tra) - 1)
        y_tra[rand_j] = -y_tra[rand_j]
        
    w, it = sgd(grad_err, Err, w_inicial, x_tra_nueva, y_tra, lr, max_iters, 10**(-20), tam_minibatch)
    ein = Err(w, x_tra_nueva, y_tra)
    E_in_lista.append(ein)
    
    # Repetimos proceso con el test
    x_test = simula_unif(1000, 2, 1.0)
    y_test = etiquetas_en_vector(x_test[:,0],x_test[:,1])
    x_tst_nueva = nuevas_carac(x_test[:,0], x_test[:,1])
    
    for j in range(0,len(y_test)//10,1):
        rand_j = random.randint(0, len(y_test) - 1)
        y_test[rand_j] = -y_test[rand_j]
    
    eout = Err(w, x_tst_nueva, y_test)
    E_out_lista.append(eout)

#convierto lista en array
errores_in = np.array(E_in_lista)
errors_out = np.array(E_out_lista)
#realizo las medias
media_in = np.mean(errores_in)
media_out = np.mean(errors_out)




print ('Errores Ein y Eout medios tras 1000reps del experimento:\n')
print ("Ein media: ", media_in)
print ("Eout media: ", media_out)

input("\n--- Pulsar tecla para continuar ---\n")


















