#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 15 19:41:30 2020

@author: rafa
"""

import numpy as np
import matplotlib.pyplot as plt
import math
import random as rnd


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





unif = simula_unif(50, 2, [-50, 50])
gaus = simula_gaus(50, 2, [5, 7])

#Elementos apartados 2a y 2b ejercicio anterior
muestra =  simula_unif(100, 2, [-50, 50])

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


#Divido los datos en positivos y negativos
#creo listas para separacion
datos_positivos = []
datos_negativos = []
#creo de etiquetas para usarlo en el apartado sigguiente
etiquetas_positivas = []
etiquetas_negativas = []

for i in muestra:
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

#concateno las clases para usarlas ahora
datos_conjunto = np.concatenate((datos_positivos,datos_negativos), axis=0)
etiquetas_conjunto = np.concatenate((etiquetas_positivas,etiquetas_negativas),axis=0)



#2b

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
datos_ruido = np.concatenate((datos_positivos, datos_negativos), axis = 0)
etiquetas_ruido = np.concatenate((etiquetas_positivas, etiquetas_negativas), axis = 0)

#Repito el proceso anterior de asignacion de etiquetas
datos_positivos_ruido = []
datos_negativos_ruido = []

j = 0
for i in datos_ruido:
    if etiquetas_ruido[j] >= 1:
        datos_positivos_ruido.append(np.array([i[0],i[1]]))
        
    else:
        datos_negativos_ruido.append(np.array([i[0],i[1]]))
        
    j += 1

#Convierto las listas en arrays
datos_positivos_ruido = np.array(datos_positivos_ruido, np.float64)
datos_negativos_ruido = np.array(datos_negativos_ruido, np.float64)













print("\nEjercicio 2. Modelos Lineales\n")

# Ejercicio 2.1
print("\nAlgoritmo Perceptron\n")


def ajusta_PLA(datos, label, max_iter, vini):
    it = 0
    vini_anterior = np.copy(vini)
    
    #condición de parada llegar al máximo de iteracciones dado
    while it < max_iter:
        it += 1
        
        for i in range(len(datos)):
            #Si no clasifica bien el elemento actualizamos w
            if np.sign(np.dot(np.transpose(vini), datos[i])) != label[i]:
                vini = vini + label[i] * datos[i]
            
        
        #Paramos la ejecución cuando nuestra w (vini) actual sea igual a vini_anterior
        if np.array_equal(vini, vini_anterior):
            break
        
        #actualizo w_anterior
        vini_anterior = np.copy(vini)
    
    return vini, it


print("\nAlgoritmo Perceptron. Ejercicio 1\n")
print("Apartado a\n")

#añadimos un termino a cada fila para poder ajustar el termino independiente
datos_conjunto_unos = np.c_[np.ones(datos_conjunto.shape[0]), datos_conjunto]

#primer caso vini[0, 0, 0]
vini = [0.0, 0.0, 0.0]
max_iter = 1000

#obtengo el vecto de pesos mediante el algoritmo
w, iteraciones_1 = ajusta_PLA(datos_conjunto_unos, etiquetas_conjunto, max_iter, vini)

#realizamos el calculo de la pendiente
a = -(w[0]/w[2])/(w[0]/w[1])
#calculo del termino independiente
b = -w[0]/w[2]

#como en el ejercicio anterior genero conjunto de puntos dentro del rango [-50, 50]
x = np.linspace(-50, 50, 100)
y = a*x + b

#Gráfica
plt.plot(x,y, 'k--', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos[:, 0], datos_positivos[:, 1], c='b', label="Positivos")
plt.scatter(datos_negativos[:, 0], datos_negativos[:, 1], c='r', label ="Negativos")
plt.title('Ajuste PLA. vini[0,0,0]')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()

print("Numero de iterationes (vector inicial a 0): ", iteraciones_1)

input("\n--- Pulsar tecla para continuar ---\n")
print("Si lo ejecuto 10 veces: ")
iteraciones_1 = []
w_1 = []
vini_1 = []

for i in range(0, 10):
    #Vector de pesos inicial aleatorio 
    vini = [0.0, 0.0, 0.0]
    vini_1.append(vini)
    #obtengo el vecto de pesos mediante el algoritmo
    w, iter_2 = ajusta_PLA(datos_conjunto_unos, etiquetas_conjunto, max_iter, vini)
    iteraciones_1.append(iter_2)
    w_1.append(w)
    #realizamos el calculo de la pendiente
    a = -(w[0]/w[2])/(w[0]/w[1])
    #calculo del termino independiente
    b = -w[0]/w[2]
    #como en el ejercicio anterior genero conjunto de puntos dentro del rango [-50, 50]
    x = np.linspace(-50, 50, 100)
    y = a*x + b
    
    plt.plot(x,y, 'k--')


#Gráfica  
plt.plot(x,y, 'k--', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos[:, 0], datos_positivos[:, 1], c='b', label="Positivos")
plt.scatter(datos_negativos[:, 0], datos_negativos[:, 1], c='r', label ="Negativos")
plt.title('Ajuste PLA. vini[0.0, 0.0, 0.0] 10 veces')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()
print("Numero de iterationes (vector 0): ", iteraciones_1)
print('Media del número de iteraciones: {}'.format(np.mean(np.asarray(iteraciones_1))))








input("\n--- Pulsar tecla para continuar ---\n")
print("Apartado b\n")
#inicializamos de forma aleatoria
iteraciones_2 = []
w_2 = []
vini_2 = []

for i in range(0, 10):
    #Vector de pesos inicial aleatorio 
    vini = np.random.uniform(0 , 1, 3)
    vini_2.append(vini)
    #obtengo el vecto de pesos mediante el algoritmo
    w, iter_2 = ajusta_PLA(datos_conjunto_unos, etiquetas_conjunto, max_iter, vini)
    iteraciones_2.append(iter_2)
    w_2.append(w)
    #realizamos el calculo de la pendiente
    a = -(w[0]/w[2])/(w[0]/w[1])
    #calculo del termino independiente
    b = -w[0]/w[2]
    #como en el ejercicio anterior genero conjunto de puntos dentro del rango [-50, 50]
    x = np.linspace(-50, 50, 100)
    y = a*x + b
    
    plt.plot(x,y, 'k--')


#Gráfica  
plt.plot(x,y, 'k--', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos[:, 0], datos_positivos[:, 1], c='b', label="Positivos")
plt.scatter(datos_negativos[:, 0], datos_negativos[:, 1], c='r', label ="Negativos")
plt.title('Ajuste PLA. vini[aleatorio]')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()
print("Numero de iterationes (vector inicial aleatorio): ", iteraciones_2)
print('Media del número de iteraciones: {}'.format(np.mean(np.asarray(iteraciones_2))))

ver = input ('Si desea ver los diferentes ajustes obtenidos y los puntos de inicio introduzca "s" por teclado\n')
if ver == "s":
    print("Valor inicial de w en cada caso: ", vini_2)
    print("\nAjuste obtenido: ", w_2)







input("\n--- Pulsar tecla para continuar ---\n")
print("\nAlgoritmo Perceptron. Ejercicio 2\n")
print("Apartado a\n")

#ahora el caso de la muestra con ruido
#añadimos un termino a cada fila para poder ajustar el termino independiente
datos_ruido_unos = np.c_[np.ones(datos_ruido.shape[0]), datos_ruido]

#primer caso vini[0, 0, 0]
vini = [0.0, 0.0, 0.0]
max_iter = 1000

#obtengo el vecto de pesos mediante el algoritmo
w, iteraciones_3 = ajusta_PLA(datos_ruido_unos, etiquetas_ruido, max_iter, vini)

#realizamos el calculo de la pendiente
a = -(w[0]/w[2])/(w[0]/w[1])
#calculo del termino independiente
b = -w[0]/w[2]

#como en el ejercicio anterior genero conjunto de puntos dentro del rango [-50, 50]
x = np.linspace(-50, 50, 100)
y = a*x + b

#Gráfica
plt.plot(x,y, 'k--', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos_ruido[:, 0], datos_positivos_ruido[:, 1], c='b', label="Positivos")
plt.scatter(datos_negativos_ruido[:, 0], datos_negativos_ruido[:, 1], c='r', label ="Negativos")
plt.title('Ajuste PLA. vini[0,0,0]')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()

print("Numero de iterationes (vector inicial a 0): ", iteraciones_3)



input("\n--- Pulsar tecla para continuar ---\n")
print("Apartado b\n")
#inicializamos de forma aleatoria
iteraciones_4 = []
w_4 = []
vini_4 = []

for i in range(0, 10):
    #Vector de pesos inicial aleatorio 
    vini = np.random.uniform(0 , 1, 3)
    vini_4.append(vini)
    #obtengo el vecto de pesos mediante el algoritmo
    w, iter_4 = ajusta_PLA(datos_ruido_unos, etiquetas_ruido, max_iter, vini)
    iteraciones_4.append(iter_4)
    w_4.append(w)
    #realizamos el calculo de la pendiente
    a = -(w[0]/w[2])/(w[0]/w[1])
    #calculo del termino independiente
    b = -w[0]/w[2]
    #como en el ejercicio anterior genero conjunto de puntos dentro del rango [-50, 50]
    x = np.linspace(-50, 50, 100)
    y = a*x + b
    
    plt.plot(x,y, 'k--')


#Gráfica  
plt.plot(x,y, 'k--', label=("y = %.2fx+%.2f" % (a, b)))
plt.scatter(datos_positivos_ruido[:, 0], datos_positivos_ruido[:, 1], c='b', label="Positivos")
plt.scatter(datos_negativos_ruido[:, 0], datos_negativos_ruido[:, 1], c='r', label ="Negativos")
plt.title('Ajuste PLA. vini[aleatorio]')
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.legend()
plt.show()
print("Numero de iterationes (vector inicial aleatorio): ", iteraciones_4)
print('Media del número de iteraciones: {}'.format(np.mean(np.asarray(iteraciones_4))))

ver = input ('Si desea ver los diferentes ajustes obtenidos y los puntos de inicio introduzca "s" por teclado\n')
if ver == "s":
    print("Valor inicial de w en cada caso: ", vini_4)
    print("\nAjuste obtenido: ", w_4)












input("\n--- Pulsar tecla para continuar ---\n")
print("\nRegresión Logística. Ejercicio 2\n")

#apartado a

#función para generar minibatches aleatorios de un tamaño dado
#x es el conjutno de datos
#y las etiquetas
def genera_minibatch(x, y, tam_minibatch):
    #concatenamos los datos x con etiquetas y
    datos = np.empty((len(y), len(x[0])+1))
    datos[:, 0:len(x[0])] = x.copy()
    datos[:, -1] = np.transpose(y)
    #barajamos los datos
    np.random.shuffle(datos)
    
    minibatches = []
    
    #obtenemos el número de minibatches
    n_minibatches = len(y) // tam_minibatch
    
    begin = 0
    end = tam_minibatch
    for i in range(0, n_minibatches, 1):
        batch = datos[begin:end]
        batch_x = batch[:, 0:len(x[0])]
        batch_y = batch[:, -1]
        #cada tupla es un minibatch
        #valores x en el primer elemento de la tupla, y las etiquetas en el segundo
        minibatches.append((batch_x, batch_y))
        
        begin += tam_minibatch
        end += tam_minibatch
        
    return minibatches

#implementamos el gradiente descendiente estocastico
def sgd(grad_fun, x, y, lr = 0.01, max_iter = 100, epsilon = 0.01, tam_minibatch = 1):
    it = 0
    w_actual = np.zeros(len(x[0]))
    pasos = epsilon + 9999999.9999
    
    while it < max_iter and pasos > epsilon:
        minibatch = genera_minibatch(x, y, tam_minibatch)
        #para comprobar precision al final
        w_anterior = w_actual.copy()
        i = 0
        
        while i < len(minibatch):
            minibatch_x = minibatch[i][0]
            minibatch_y = minibatch[i][1]
            
            w_actual -= lr * grad_fun(minibatch_x, minibatch_y, w_actual)
            
            i += 1
        
        it += 1
        pasos = np.linalg.norm(w_actual - w_anterior)
    
    return w_actual, it


#Error en regresion logistica
def ErrorLogistico(datos, etiquetas, w):
    tam = len(etiquetas)
    error = 0
    
    for i in range(0, tam, 1):
        #opuesto del neperiano de la verosimilitud entre el tamaño del conjunto
        error += math.log(1 + math.exp(-etiquetas[i] * np.dot(np.transpose(w), datos[i])))
    
    error = error / tam
    return error

#gradiente del error
def ErrorGradiente(datos, etiquetas, w):
    tam = len(etiquetas)
    gradiente = np.zeros(len(datos[0]))
    
    for i in range(0, tam, 1):
        #opuesto del neperiano de la verosimilitud
        gradiente += (etiquetas[i] * datos[i]) / (1 + math.exp(etiquetas[i] * np.dot(np.transpose(w), datos[i])))
        
    gradiente = -gradiente / tam
    return gradiente
    



print("Apartado b\n")
tam_muestra = 100
#Generamos datos en [0, 2]x[0,2]
muestra = simula_unif(tam_muestra, 2, [0, 2])

#selecciono 2 puntos al azar
puntos_recta = []
#punto_aleatorio = rnd.randint(0, tam_muestra-1)
punto_aleatorio = np.random.randint(0, tam_muestra-1)
puntos_recta.append(punto_aleatorio)
#El segundo punto no puede ser el mismo que el primero
while punto_aleatorio in puntos_recta:
    punto_aleatorio = np.random.randint(0, tam_muestra-1)
    #punto_aleatorio = rnd.randint(0, tam_muestra-1)
    
puntos_recta.append(punto_aleatorio)

#Generamos la recta que pasa por estos dos puntos
#calculamos la pendiente
p0 = muestra[puntos_recta[0]]
p1 = muestra[puntos_recta[1]]
a = (p1[1] - p0[1]) / (p1[0] - p0[0])

#termino indepentiemte
b = -a * p0[0] + p0[1]

x = np.linspace(0, 2, 100)

print ('\nSi desea observar el proceso de generación de la recta')
ver = input ('Si desea verla introduzca "s" por teclado\n')
if ver == "s":
    
    plt.scatter(muestra[:, 0], muestra[:, 1],color="Black")
    plt.title("Muestra de 100 elementos con distribución uniforme")
    plt.xlabel("Eje X")
    plt.ylabel("Eje Y")
    plt.show()
    
    input("\n--- Pulsar tecla para continuar ---\n")
    
    plt.scatter(muestra[:, 0], muestra[:, 1],color="Black",label="Muestra de 100 elementos con distribución uniforme")
    plt.scatter(p0[0], p0[1],color="Orange",label="Puntos aleatorios por los que pasa la recta")
    plt.scatter(p1[0], p1[1],color="Orange")
    plt.title("Dos puntos aleatorios seleccionados")
    plt.xlabel("Eje X")
    plt.ylabel("Eje Y")
    plt.legend()
    plt.show()
    
    input("\n--- Pulsar tecla para continuar ---\n")
    
    #la recta
    plt.axis([0,2,0,2]) 
    plt.scatter(muestra[:, 0], muestra[:, 1],color="Black",label="Muestra de 100 elementos con distribución uniforme")
    plt.scatter(p0[0], p0[1],color="Orange",label="Puntos aleatorios por los que pasa la recta")
    plt.scatter(p1[0], p1[1],color="Orange")
    plt.plot(x,x * a + b,color="Black", label=("Recta separadora %f.2x + %f.2" % (a,b)))
    plt.title("Puntos uniformemente distribuidos y recta separadora")
    plt.xlabel("Eje X")
    plt.ylabel("Eje Y")
    plt.legend()
    plt.show()
    
    input("\n--- Pulsar tecla para continuar ---\n")
    
    
    
#Etiquetamos los puntos respecto a la recta
#Divido los datos en positivos y negativos
#creo listas para separacion
datos_positivos = []
datos_negativos = []
#creo de etiquetas para usarlo en el apartado sigguiente
etiquetas_positivas = []
etiquetas_negativas = []

for i in muestra:
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


# Gráfica
plt.axis([0,2,0,2])
plt.plot(x, x * a + b,color = "Black", label=("Recta separadora %f.2x + %f.2" % (a,b)))
plt.scatter(datos_positivos[:,0], datos_positivos[:,1], c='b', label="Positivos")
plt.scatter(datos_negativos[:,0], datos_negativos[:,1], c='r', label ="Negativos")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribución uniforme de puntos con recta separadora")
plt.legend()
plt.show()

input("\n--- Pulsar tecla para continuar ---\n")

y = a * x + b

muestra_unos = np.c_[np.ones(muestra.shape[0]), muestra]

w_regresion, it = sgd(ErrorGradiente, muestra_unos, y, lr = 0.01, max_iter = 100, epsilon = 0.01, tam_minibatch = 1)

error_logistico = ErrorLogistico(muestra_unos, y, w_regresion)
print("Error de regresión logística en la muestra: %.3f" % error_logistico)

input("\n--- Pulsar tecla para continuar ---\n")

#Crearemos ahora una muestra más grande de N=2000
#objetivo Eout

#Funcion de probabilidad de un dato
#parte de los pesos ajustados por regresion logistica
def prediccion_log(datos, w):
    s = np.dot(np.transpose(w), datos)
    s = math.exp(s) / (1 + math.exp(s))
    return s

#Dependientdo de la probabilidad devuelve la etiqueta
#La frontera de decisión estara en el punto medio entre 0 y 1, es decir, 0.5
def etiquetas_probabilidad(probabilidad):
    if (probabilidad > 0.5):
        etiqueta = 1
    else:
        etiqueta = -1
    
    return etiqueta



tam_muestra = 2000
muestra_test = simula_unif(tam_muestra, 2, [0,2])
muestra_test_unos = np.c_[np.ones(muestra_test.shape[0]), muestra_test]

probabilidad = np.zeros(tam_muestra)

for i in range(0, len(muestra_test_unos)):
    probabilidad[i] = prediccion_log(muestra_test_unos[i], w_regresion)


etiquetas_probabilidad_vector = np.vectorize(etiquetas_probabilidad)
#asignamos etiquetas segun su probabilidad
y_test = etiquetas_probabilidad_vector(probabilidad)

error_logistico_test = ErrorLogistico(muestra_test_unos, y_test, w_regresion)
print("Error de regresión logística en los nuevos datos (test): %.3f" % error_logistico_test)

input("\n--- Pulsar tecla para continuar ---\n")


#Etiquetamos los puntos respecto a la recta
#Divido los datos en positivos y negativos
#creo listas para separacion
datos_positivos = []
datos_negativos = []
#creo de etiquetas para usarlo en el apartado sigguiente
etiquetas_positivas = []
etiquetas_negativas = []

for i in muestra_test:
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


# Gráfica
plt.axis([0,2,0,2])
plt.plot(x, x * a + b,color = "Black", label=("Recta separadora %f.2x + %f.2" % (a,b)))
plt.scatter(datos_positivos[:,0], datos_positivos[:,1], c='b', label="Positivos")
plt.scatter(datos_negativos[:,0], datos_negativos[:,1], c='r', label ="Negativos")
plt.xlabel("Eje X")
plt.ylabel("Eje Y")
plt.title("Distribución uniforme de puntos con recta separadora")
plt.legend()
plt.show()



















#