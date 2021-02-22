#!/usr/bin/env python3
"""
Created on Tue Apr 14 19:34:38 2020

@author: rafa
"""

# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import math


# Fijamos la semilla
np.random.seed(1)


def simula_unif(N, dim, rango):
	return np.random.uniform(rango[0],rango[1],(N,dim))

def simula_gaus(N, dim, sigma):
    media = 0    
    out = np.zeros((N,dim),np.float64)        
    for i in range(N):
        # Para cada columna dim se emplea un sigma determinado. Es decir, para 
        # la primera columna se usará una N(0,sqrt(5)) y para la segunda N(0,sqrt(7))
        out[i,:] = np.random.normal(loc=media, scale=np.sqrt(sigma), size=dim)
        
    return out


def simula_recta(intervalo):
    points = np.random.uniform(intervalo[0], intervalo[1], size=(2, 2))
    x1 = points[0,0]
    x2 = points[1,0]
    y1 = points[0,1]
    y2 = points[1,1]
    # y = a*x + b
    a = (y2-y1)/(x2-x1) # Calculo de la pendiente.
    b = y1 - a*x1       # Calculo del termino independiente.
    
    return a, b


print("\nEjercicio 1. Complejidad de H y el ruido\n")

# Ejercicio 1.1
print("\nEjercicio 1.1\n")
print("Apartado a\n")

#Genero una muestra de puntos con simula_unif()
unif = simula_unif(50, 2, [-50, 50])

#mostramos la gráfica resultante
plt.scatter(unif[:, 0], unif[:, 1], c='b')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribuccion Uniforme: simula_unif(50, 2, [-50, 50])")
plt.show()

input("\n--- Pulsar tecla para continuar ---\n")
print("Apartado b\n")

#Genero muestra de puntos con simula_gaus()
gaus = simula_gaus(50, 2, [5, 7])

#mostramos la gráfica resultante
plt.scatter(gaus[:, 0], gaus[:, 1], c='r')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribuccion Gaussiana: simula_gaus(50, 2, [5, 7])")
plt.show()

print ('\nGrafica Combinadas')
ver = input ('Si desea verla introduzca "s" por teclado\n')
if ver == "s":
    plt.scatter(unif[:, 0],unif[:, 1],color="Blue",label="Distribuccion Uniforme: simula_unif(50, 2, [-50, 50])")
    plt.scatter(gaus[:, 0],gaus[:, 1],color="Red",label="Distribuccion Gaussiana: simula_gaus(50, 2, [5, 7])")
    plt.legend()
    plt.show()
    



    
# Ejercicio 1.2
print("\nEjercicio 1.2\n")
print("Apartado a\n")

#nueva muestra de puntos
nueva_unif =  simula_unif(100, 2, [-50, 50])

#No utilizo la funcion sign de numpy porque en el caso de sign(0)
#nos devuelve 0 y no 1, lo que no nos conviene
def signo(valor):
    if valor >= 0:
        return 1
    else:
        return -1

#Defino funcion para calcular la distancia respecto a la recta
def f(x, y, a , b):
    return signo(y - a*x - b)


#Uso la funcion simula_recta() para el termino independiente y la pendiente
a, b = simula_recta([-50, 50])

print("Parámetros de la recta aleatoria y = ax + b")
print("a = ", a)
print("b = ", b)

#Divido los datos en positivos y negativos
#creo listas para separacion
datos_positivos = []
datos_negativos = []
#creo de etiquetas para usarlo en el apartado sigguiente
etiquetas_positivas = []
etiquetas_negativas = []

for i in nueva_unif:
    etiqueta = f(i[0], i[1], a, b)
    
    if etiqueta >= 1:
        datos_positivos.append(np.array([i[0], i[1]]))
        etiquetas_positivas.append(etiqueta)
        
    else:
        datos_negativos.append(np.array([i[0], i[1]]))
        etiquetas_negativas.append(etiqueta)


#Convierto las listas en arrays
datos_positivos = np.array(datos_positivos, np.float64)
datos_negativos = np.array(datos_negativos, np.float64)


# genero un conjunto de datos uniformes dentro del rango[-50,50],
# calculo 'y' respectivas del conjunto generado
x = np.linspace(-50, 50, 100)
y = a * x + b

# Gráfica
plt.plot(x,y, 'k', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos[:,0], datos_positivos[:,1], c='b', label="positivos")
plt.scatter(datos_negativos[:,0], datos_negativos[:,1], c='r', label ="negativos")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribución uniforme de puntos con recta separadora")
plt.legend()
plt.show()





input("\n--- Pulsar tecla para continuar ---\n")
print("\nEjercicio 1.2\n")
print("Apartado b\n")

#funcion para introducir el ruido, un 10%
#cambio el valor de un 10% de las etiquetas
def ruido(valor):
    porcentaje = round(len(valor) * 10 /100)
    n = np.random.randint(0, len(valor) - 1, porcentaje)
    for i in range(np.int64(porcentaje)):
        x = n[i]
        if valor[x] == 1:
            valor[x] = -1
        else:
            valor[x] = 1
    
    return valor

#Introduzco ruido en las etiquetas anteriores
etiquetas_positivas = ruido(etiquetas_positivas)
etiquetas_negativas = ruido(etiquetas_negativas)

#Concateno los valores, tanto datos como etiquetas
datos = np.concatenate((datos_positivos, datos_negativos), axis = 0)
etiquetas = np.concatenate((etiquetas_positivas, etiquetas_negativas), axis = 0)

#Repito el proceso anterior de asignacion de etiquetas
datos_positivos_ruido = []
datos_negativos_ruido = []

j = 0
for i in datos:
    if etiquetas[j] >= 1:
        datos_positivos_ruido.append(np.array([i[0],i[1]]))
        
    else:
        datos_negativos_ruido.append(np.array([i[0],i[1]]))
        
    j += 1

#Convierto las listas en arrays
datos_positivos_ruido = np.array(datos_positivos_ruido, np.float64)
datos_negativos_ruido = np.array(datos_negativos_ruido, np.float64)

#Repetimos generaciones de conjunto de datos e "y" respectivas
x = np.linspace(-50, 50, 100)
y = a * x + b

# Gráfica
plt.plot(x,y, 'k', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribución uniforme de puntos con recta separadora y ruido en etiquetas")
plt.legend()
plt.show()




input("\n--- Pulsar tecla para continuar ---\n")
print("\nEjercicio 1.3\n")

#Defino las funciones tras calcular sus intersecciones cuando f(x, y) = 0
def f_1_1(x):
    return (20 - math.sqrt(300 + 20*x - x**2))

def f_1_2(x):
    return (20 + math.sqrt(300 + 20*x - x**2))


def f_2_1(x):
    return 1/2*( 40 - math.sqrt(2)*math.sqrt(-x**2 - 20*x + 700))

def f_2_2(x):
    return 1/2*( 40 + math.sqrt(2)*math.sqrt(-x**2 - 20*x + 700))


def f_3_1(x):
    return 1/2 * ( -40 - math.sqrt(2) * math.sqrt(x**2 - 20*x - 700))

def f_3_2(x):
    return 1/2 * ( -40 + math.sqrt(2) * math.sqrt(x**2 - 20*x - 700))


def f_4(x):
    return 20*x**2 + 5*x - 3




print("\nPrimera Funcion\n")
f1 = np.vectorize(f_1_1)
f1_2 = np.vectorize(f_1_2)

#En la memoria se explica como hemos obtenido el rango en el que pintar la
#funcion, este es de [-10, 30]
m = np.linspace(-10 ,30 ,100)

plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.plot(m, f1(m),color="Black",label="y = 20 +- sqrt(300 + 20*x - x^2)")
plt.plot(m, f1_2(m),color="Black")
plt.title("Distribución uniforme de puntos con curva separadora 1 y ruido en etiquetas")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()







input("\n--- Pulsar tecla para continuar ---\n")
print("\nSegunda Funcion\n")
f2 = np.vectorize(f_2_1)
f2_2 = np.vectorize(f_2_2)

# Rango en el cual el contenido de las raices cuadradas es positivo es [-38.28, 18,28]
m = np.linspace(-38.28, 18.28, 100)

plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.plot(m, f2(m),color="Black",label="y = 20 +- sqrt(300 + 20*x - x^2)")
plt.plot(m, f2_2(m),color="Black")
plt.title("Distribución uniforme de puntos con curva separadora 2 (elipse) y ruido en etiquetas")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()







input("\n--- Pulsar tecla para continuar ---\n")
print("\nTercera Funcion\n")
f3 = np.vectorize(f_3_1)
f3_2 = np.vectorize(f_3_2)

# Rangos son [-50, -18,29] y [38.29, 50]
m1 = np.linspace(-50.0, -18.29, 100)
m2 = np.linspace(38.29, 50.0, 100)

plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.plot(m1, f3(m1),color="Black",label="y = 1/2 ( -40 +-sqrt(2)*sqrt(x^2 - 20x - 700))")
plt.plot(m1, f3_2(m1),color="Black")
plt.plot(m2, f3(m2),color="Black")
plt.plot(m2, f3_2(m2),color="Black")
plt.title("Distribución uniforme de puntos con curva separadora 3 (hipérbola) y ruido en etiquetas")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()









input("\n--- Pulsar tecla para continuar ---\n")
print("\nCuarta Funcion\n")
f4 = np.vectorize(f_4)

# En este caso no hay límite de rango, así que pondré el mismo que en el que se generan los puntos
#tendré que poner limites de ejes X e Y para ver los puntos en mi gráfica
m = np.linspace(-50.0 ,50.0 ,100);


plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.plot(m, f4(m),color="Black",label="y = 20x^2 + 5x - 3")
plt.title("Distribución uniforme de puntos con curva separadora 4 (parábola) y ruido en etiquetas")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()






#Establecemos un dominio para poder ver la función dentro del rango de punto
#Obtenemos el rango [-1.76, 1.51]
m = np.linspace(-1.76 ,1.51 ,100);


plt.scatter(datos_positivos_ruido[:,0], datos_positivos_ruido[:,1], c='b', label="positivos")
plt.scatter(datos_negativos_ruido[:,0], datos_negativos_ruido[:,1], c='r', label ="negativos")
plt.plot(m, f4(m),color="Black",label="y = 20x^2 + 5x - 3")
plt.title("Distribución uniforme de puntos con curva separadora 4 (parábola) y ruido en etiquetas")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()

