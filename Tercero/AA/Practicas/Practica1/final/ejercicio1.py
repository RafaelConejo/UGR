#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 24 21:06:02 2020

@author: rafa
"""

#############################
#####     LIBRERIAS     #####
#############################

import math
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from tabulate import tabulate #para poder pintar la tabla

#-------------------------------------------------------------------------------#
#------------- Ejercicio sobre la búsqueda iterativa de óptimos ----------------#
#-------------------------------------------------------------------------------#


#------------------------------Ejercicio 1 -------------------------------------#

# Fijamos la semilla

#Función E(u,v)
def E(w): 
	return (w[0]*math.exp(w[1]) - 2*w[1]*math.exp(-w[0]))**2
			 
# Derivada parcial de E respecto de u
def Eu(w):
	return 2*math.exp(-2*w[0])*(math.exp(w[1] + w[0]) + 2*w[1])*(w[0]*math.exp(w[1] + w[0]) - 2*w[1])

# Derivada parcial de E respecto de v
def Ev(w):
	return 2*math.exp(-2*w[0])*(w[0]*math.exp(w[0] + w[1]) - 2)*(w[0]*math.exp(w[0] + w[1]) - 2*w[1])
	
# Gradiente de E
# recibe como parámetro un array de numpy con las coordenadas 2D de un punto
# devuelve un array de numpy con el vector de gradiente en ese punto
def gradE(w):
    return np.array([Eu(w), Ev(w)], np.float64)

#quizas quitar x



def gd(w, lr, grad_fun, fun, epsilon, max_iters = 10000):		
    itera = 0 #Si no supera el umbral
    
    #caso de función E
    if(fun == 0):
        for i in range(max_iters):
            w = w - lr * grad_fun(w)
            if(E(w) < epsilon):
                itera = i + 1               #porque mepieza en 0 le sumo 1
                break
        
        return itera, w
    
    #caso de función F
    elif(fun == 1):
        it = 0
        iteracciones = []
        funcion = []
        while  it <= max_iters:
            iteracciones.append(it)
            funcion.append(f(w))
            w = w - lr * gradf(w)
            it += 1
        return w, iteracciones, funcion
    

#Establezco las variables para el ejercicio 2
lr = 0.1   #tasa de aprendizaje
objetivo = 10**(-14)
x = np.array([0,0], np.float64)
w = np.array([1.0,1.0], np.float64)
num_ite, w = gd(w, lr, gradE, 0, objetivo, 10000)

print ('\nGRADIENTE DESCENDENTE')
print ('\nEjercicio 1\n')
print ('Numero de iteraciones: ', num_ite)
input("\n--- Pulsar tecla para continuar ---\n")
print ('Coordenadas obtenidas: (', w[0], ', ', w[1],')')


ver = input ('Si desea ver el valor en esas coordenadas introduzca "s" por teclado\n')
if ver == "s":
    print(E(w))
    input("\n--- Pulsar tecla para continuar ---\n")








#------------------------------Ejercicio 2 -------------------------------------#

def f(w):   
	return (w[0] - 2)**2 + 2*(w[1] + 2)**2 + 2 * np.sin(2 * np.pi * w[0]) * np.sin(2 * np.pi * w[1])
	
# Derivada parcial de f respecto de x
def fx(w):
	return 2*(2 * math.pi * math.cos(2 * math.pi * w[0]) * math.sin(2 * math.pi * w[1]) + w[0] - 2)

# Derivada parcial de f respecto de y
def fy(w):
	return 4*(math.pi*math.sin(2 * math.pi * w[0])*math.cos(2 * math.pi * w[1]) + w[1] + 2)
	
# Gradiente de f
def gradf(w):
	return np.array([fx(w), fy(w)], np.float64)
	
# a) Usar gradiente descendente para minimizar la función f, con punto inicial (1,1)
# tasa de aprendizaje 0.01 y max 50 iteraciones. Repetir con tasa de aprend. 0.1
x = np.array([0,0], np.float64)
punto_inicial = np.array([1.0, -1.0], np.float64)

#Primer caso lr = 0,01
lr1 = 0.01
w1, i1, f1 = gd(punto_inicial, lr1, gradf, 1, 0, 50)


#Segundo caso lr = 0,1
lr2 = 0.1
w2, i2, f2 = gd(punto_inicial, lr2, gradf, 1, 0, 50)



def gd_grafica(it, fun):		
	plt.plot(it, fun)
	plt.xlabel('Iteraciones')
	plt.ylabel('f(x,y)')
	plt.show()	

print("Ejercicio 3: Apartado a)")

print ('\nGrafica con learning rate igual a 0.01')
gd_grafica(i1, f1)
print ('\nValor mínimo learning rate igual a 0.01')
print(f(w1))
print ('\nGrafica con learning rate igual a 0.1')
gd_grafica(i2, f2)
print ('\nValor mínimo learning rate igual a 0.1')
print(f(w2))


print ('\nA continuación presento Grafica comparativa de las anteriores\n')
ver = input ('Si desea verlas introduzca "s" por teclado\n')
if ver == "s":
    X_AXIS = range(0, 51)
    plt.plot(X_AXIS, f1, label='n=0.01')
    plt.plot(X_AXIS, f2, label='n=0.1')
    plt.xlabel("Iteraciones")
    plt.ylabel("f(x,y)")
    plt.legend()
    plt.show()
    input("\n--- Pulsar tecla para continuar ---\n")





print("Ejercicio 3: Apartado b)")

# b) Obtener el valor minimo y los valores de (x,y) con los
# puntos de inicio siguientes:

def gd(w, lr, max_iters = 50):
    it = 0
    iteracciones = []
    funcion = []
    while  it <= max_iters:
        iteracciones.append(it)
        funcion.append(f(w))
        w = w - lr * gradf(w)
        it += 1
    return w, iteracciones, funcion

#Intuyo que por defecto la tasa de aprendizaje es 0.01
lr_ejer3 = 0.01
iteraccion = 50

print ('Punto de inicio: (2.1, -2.1)\n')
punto_inicio = np.array([2.1, -2.1]) 
w, iterac, func = gd(punto_inicio, lr_ejer3, iteraccion)
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

plt.figure(1)
plt.title('Comparación de las funciones')
plt.plot(iterac, func, 'g-', label= '(2.1, -2.1)')

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (3.0, -3.0)\n')
punto_inicio = np.array([3.0, -3.0]) 
w, iterac, func = gd(punto_inicio, lr_ejer3, iteraccion)
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

plt.plot(iterac, func, 'y-', label= '(3.0, -3.0)')

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (1.5, 1.5)\n')
punto_inicio = np.array([1.5, 1.5]) 
w, iterac, func = gd(punto_inicio, lr_ejer3, iteraccion)
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor minimo: ',f(w))

plt.plot(iterac, func, 'r-', label= '(1.5, 1.5)')

input("\n--- Pulsar tecla para continuar ---\n")

print ('Punto de inicio: (1.0, -1.0)\n')
punto_inicio = np.array([1.0, -1.0]) 
w, iterac, func = gd(punto_inicio, lr_ejer3, iteraccion)
print ('(x,y) = (', w[0], ', ', w[1],')\n')
print ('Valor mínimo: ',f(w))

plt.plot(iterac, func, 'b-', label= '(1.0, -1.0)')

print ('\nEjercicios terminados, lo siguiente es gráfica comparativa y tabla de los valores\n')
input("\n--- Pulsar tecla para continuar ---\n")

plt.legend()
plt.xlabel("Iteraciones")
plt.ylabel("f(x,y)")
plt.show()

input("\n--- Pulsar tecla para continuar ---\n")

#Creamos una tabla para almacenar: punto inicio, x, y, valor de la función
tabla = []
#recorremos los puntos de inicio
puntos_inicio = ([2.1, -2.1], [3.0, -3.0], [1.5, 1.5], [1.0, -1.0])
for a in puntos_inicio:
    #w son las coordenadas del gradiente
    w, iterac, func = gd(a, lr_ejer3, iteraccion)
    #en fila guardamos lo que vamos a mostrar en una fila de la tabla:
    #a, el punto de inicio; las coordenadas del gradiente; el valor de la función en ese punto
    fila = ([a, w[0], w[1], f(w)])
    tabla.append(fila)
    
print("Tabla de valores:")
print(tabulate(tabla, headers=['Punto de inicio', 'Coordenada x', 'Coordenada y',
'Valor de la función'], tablefmt='grid'))