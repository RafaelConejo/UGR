#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 24 21:06:02 2020

@author: rafa
"""

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 24 17:57:43 2020

@author: rafa
"""

#############################
#####     LIBRERIAS     #####
#############################

import math
import numpy as np
import random
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

#-------------------------------------------------------------------------------#
#------------- Ejercicio sobre la búsqueda iterativa de óptimos ----------------#
#-------------------------------------------------------------------------------#


#------------------------------Ejercicio 1 -------------------------------------#

# Fijamos la semilla




#Implemento el algoritmo de gradiente descendiente
def gd(w, lr = 0.01, limitador, max_iters = 100000):		
    punto_actual = punto_inicio.copy()
    iteracciones = 0
    valor_gradiente = w(punto_actual)
    
    #creo listas para guardar los puntos y gradintes
    puntos = []
    gradientes = []
    puntos.append(punto_actual.copy())
    gradientes.append(valor_gradiente.copy())
    
    #Criterio de parada: maximo número de iteracciones o que la norma de la diferencia 
    #entre el punto actual y el de la iteración anterior disminuya hasta un cierto valor
    while iteracciones < max_iters and pasos > precision:
        punto_anterior = punto_actual.copy()
        punto_actual = punto_actual - (lr * valor_gradiente)
        valor_gradiente = w(punto_actual)
        #mirar que hace esto
        pasos = np.linalg.norm(punto_actual - punto_anterior)
        #Añadimos a las listas
        puntos.append(punto_actual.copy())
        gradientes.append(valor_gradiente.copy())
        
        iteracciones += 1
        
    #Transformamos las listas en arrays gracias a numpy
    array_puntos = np.array(puntos)
    array_gradientes = np.array(gradientes)
    
    return punto_actual, iteracciones, array_puntos, array_gradientes


#Ejercicio 2. Usar gradiente descendente para encontrar un mínimo de la funcion
#E(u, v), comenzando desde el punto (u, v) = (1, 1) y usando una tasa de aprendizaje η = 0,01.




# a) Calcula analíticamente y mostrar la expresión del gradiente de la función E(u,v)
			 


#Punto de partida para E
p_inicio = np.array([1.0, 1.0])

#Ejecuto el gradiente descendiente para ver con cuántas
# iteraciones y en qué punto se consigue que E(u,v) valga menos de 10^(-14)
w, ite, points, grands = gd(gradE, p_inicio.copy(), 0.01, 10**(-8), 20)
num_ite = ite + 1
print("Mímimo de E(u,v) encontrado: %.15f" % E(w[0],w[1]))

while E(w[0], w[1]) >= 10**(-14):
    w, ite, points, grands = gd(gradE, p_inicio.copy(), 0.01, 10**(-8), num_ite)
    num_ite = ite + 1
    print("Para un máximo de %d iteraciones:" % num_ite)
    print("Mímimo de E(u,v) encontrado: %.15f" % E(w[0],w[1]))
    print("-------------------------------------------------")



print ('\nGRADIENTE DESCENDENTE')
print ('\nEjercicio 1\n')
print ('Numero de iteraciones: ', num_ite)
input("\n--- Pulsar tecla para continuar ---\n")
print ('Coordenadas obtenidas: (', w[0], ', ', w[1],')')

input("\n--- Pulsar tecla para continuar ---\n")

#------------------------------Ejercicio 2 -------------------------------------#

def f(w):   
	return """ Funcion """
	
# Derivada parcial de f respecto de x
def fx(w):
	return """ Derivada parcial """

# Derivada parcial de f respecto de y
def fy(w):
	return """ Derivada parcial """
	
# Gradiente de f
def gradf(w):
	return np.array([fx(w), fy(w)])
	
# a) Usar gradiente descendente para minimizar la función f, con punto inicial (1,1)
# tasa de aprendizaje 0.01 y max 50 iteraciones. Repetir con tasa de aprend. 0.1
def gd_grafica(w, lr, grad_fun, fun, max_iters = 1000):
				
	plt.plot(range(0,max_iters), graf, 'bo')
	plt.xlabel('Iteraciones')
	plt.ylabel('f(x,y)')
	plt.show()	

print ('Resultados ejercicio 2\n')
print ('\nGrafica con learning rate igual a 0.01')
print ('\nGrafica con learning rate igual a 0.1')

input("\n--- Pulsar tecla para continuar ---\n")


# b) Obtener el valor minimo y los valores de (x,y) con los
# puntos de inicio siguientes:

def gd(w, lr, grad_fun, fun, max_iters = 100):		
	return w

print ('Punto de inicio: (2.1, -2.1)\n')
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (3.0, -3.0)\n')
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (1.5, 1.5)\n')
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (1.0, -1.0)\n')
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor mínimo: ',f(w))

input("\n--- Pulsar tecla para continuar ---\n")