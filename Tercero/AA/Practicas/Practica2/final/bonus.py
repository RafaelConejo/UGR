#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 17 18:11:27 2020

@author: rafa
"""
import numpy as np
import matplotlib.pyplot as plt

# Fijamos la semilla
np.random.seed(1)

# Funcion para leer los datos
def readData(file_x, file_y, digits, labels):
	# Leemos los ficheros	
	datax = np.load(file_x)
	datay = np.load(file_y)
	y = []
	x = []	
	# Solo guardamos los datos cuya clase sea la digits[0] o la digits[1]
	for i in range(0,datay.size):
		if datay[i] == digits[0] or datay[i] == digits[1]:
			if datay[i] == digits[0]:
				y.append(labels[0])
			else:
				y.append(labels[1])
			x.append(np.array([1, datax[i][0], datax[i][1]]))
			
	x = np.array(x, np.float64)
	y = np.array(y, np.float64)
	
	return x, y


#Definimos funcion para obtener el error
def Err(x, y, w):
    return sum(np.sign(np.dot(x, w.T)) != y) / len(y)

#Definimos pseudoinversa
def pseudoinversa(x, y):
    return np.dot(np.linalg.pinv(x),y)





# Lectura de los datos de entrenamiento
x, y = readData('datos/X_train.npy', 'datos/y_train.npy', [4,8], [-1,1])
# Lectura de los datos para el test
x_test, y_test = readData('datos/X_test.npy', 'datos/y_test.npy', [4,8], [-1,1])

print ('\nDatos Training\n')
#mostramos los datos leidos
fig, ax = plt.subplots()
ax.plot(np.squeeze(x[np.where(y == -1),1]), np.squeeze(x[np.where(y == -1),2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x[np.where(y == 1),1]), np.squeeze(x[np.where(y == 1),2]), 'o', color='blue', label='8')
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='Digitos Manuscritos (Training)')
ax.set_xlim((0, 1))
plt.legend()
plt.show()

input("\n--- Pulsar tecla para continuar ---\n")
print ('\nDatos Test\n')
#repetimos con los test
fig, ax = plt.subplots()
ax.plot(np.squeeze(x_test[np.where(y_test == -1),1]), np.squeeze(x_test[np.where(y_test == -1),2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x_test[np.where(y_test == 1),1]), np.squeeze(x_test[np.where(y_test == 1),2]), 'o', color='blue', label='8')
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='Digitos Manuscritos (Test)')
ax.set_xlim((0, 1))
plt.legend()
plt.show()



input("\n--- Pulsar tecla para continuar ---\n")
print ('Comenzamos con el Modelo de Regresión Lineal (pseudoinversa)\n')
w = pseudoinversa(x, y)

#obtengo errores
Ein = Err(x, y, w)
Eout = Err(x_test, y_test, w)

print ('Bondad del resultado para pseudoinversa:\n')
print ("Ein: ", Ein)
print ("Eout: ",  Eout)



input("\n--- Pulsar tecla para continuar ---\n")

print("Graficas, pseudoinversa sobre datos de Training\n")
#calculo de la pendiente
a1 = -(w[0] / w[2]) / (w[0] / w[1])
#calculo del termino independiente
b1 = -w[0] / w[2]
x1 = np.linspace(0, 0.5, 100)
y1 = a1 * x1 + b1

## gráfica de la pseudoinversa con datos entrenamiento
fig, ax = plt.subplots()
ax.plot(np.squeeze(x[np.where(y == -1) ,1]), np.squeeze(x[np.where(y == -1) ,2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x[np.where(y == 1) ,1]), np.squeeze(x[np.where(y == 1) ,2]), 'o', color='blue', label='8')
plt.plot(x1,y1, "k", label="%f.2x + %f.2"  % (a1,b1))
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='Regresion Linear (Pseudoinversa) sobre datos Training')
ax.set_xlim((0, 1))
plt.legend()
plt.show()



input("\n--- Pulsar tecla para continuar ---\n")

print("Graficas, pseudoinversa sobre datos de Test\n")
#calculo de la pendiente
a2 = -(w[0] / w[2]) / (w[0] / w[1])
#calculo del termino independiente
b2 = -w[0] / w[2]
x2 = np.linspace(0, 0.5, 100)
y2 = a2 * x2 + b2
# grafica de la pseudoinversa con datos test
fig, ax = plt.subplots()
ax.plot(np.squeeze(x_test[np.where(y_test == -1),1]), np.squeeze(x_test[np.where(y_test == -1),2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x_test[np.where(y_test == 1),1]), np.squeeze(x_test[np.where(y_test == 1),2]), 'o', color='blue', label='8')
plt.plot(x2,y2, "k", label="%f.2x + %f.2"  % (a2,b2))
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='Regresion Linear (Pseudoinversa) sobre datos Test')
ax.set_xlim((0, 1))
plt.legend()
plt.show()







input("\n--- Pulsar tecla para continuar ---\n")
print ('\nA continuación PLA-Pocket\n')

def PLA_pocket(x, y, vini, max_iter, epsilon = 0.01):
    it = 0
    w = w_anterior = w_final = vini
    epsilon_min = 9999.0
    
    while it < max_iter:
        for i in range(len(y)):
            if np.sign(np.dot(np.transpose(w), x[i])) != y[i]:
                w += y[i] * x[i]
                
        if Err(x, y, w) < epsilon_min:
            epsilon_min = Err(x, y, w)
            w_final = w
            
        diferencia = np.linalg.norm(w_anterior - w)
        if diferencia < epsilon:
            break
        
        w_anterior = np.copy(w)
        it += 1
    
    return w_final


max_iter = 1000

w_2 = PLA_pocket(x, y, w, max_iter, epsilon = 0.01)

#obtengo errores
Ein = Err(x, y, w_2)
Eout = Err(x_test, y_test, w_2)

print ('Bondad del resultado para PLA-Pocket:\n')
print ("Ein: ", Ein)
print ("Eout: ",  Eout)
        

input("\n--- Pulsar tecla para continuar ---\n")

print("Graficas, PLA-Pocket sobre datos de Training\n")
#calculo de la pendiente
a3 = -(w_2[0] / w_2[2]) / (w_2[0] / w_2[1])
#calculo del termino independiente
b3 = -w_2[0] / w_2[2]
x3 = np.linspace(0, 0.5, 100)
y3 = a3 * x3 + b3

## gráfica de la pseudoinversa con datos entrenamiento
fig, ax = plt.subplots()
ax.plot(np.squeeze(x[np.where(y == -1) ,1]), np.squeeze(x[np.where(y == -1) ,2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x[np.where(y == 1) ,1]), np.squeeze(x[np.where(y == 1) ,2]), 'o', color='blue', label='8')
plt.plot(x3, y3, "k", label="%f.2x + %f.2"  % (a3,b3))
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='PLA-Pocket sobre datos Training')
ax.set_xlim((0, 1))
plt.legend()
plt.show()


input("\n--- Pulsar tecla para continuar ---\n")

print("Graficas, PLA-Pocket sobre datos de Test\n")
#calculo de la pendiente
a4 = -(w_2[0] / w_2[2]) / (w_2[0] / w_2[1])
#calculo del termino independiente
b4 = -w_2[0] / w_2[2]
x4 = np.linspace(0, 0.5, 100)
y4 = a4 * x4 + b4
# grafica de la pseudoinversa con datos test
fig, ax = plt.subplots()
ax.plot(np.squeeze(x_test[np.where(y_test == -1),1]), np.squeeze(x_test[np.where(y_test == -1),2]), 'o', color='red', label='4')
ax.plot(np.squeeze(x_test[np.where(y_test == 1),1]), np.squeeze(x_test[np.where(y_test == 1),2]), 'o', color='blue', label='8')
plt.plot(x4, y4, "k", label="%f.2x + %f.2"  % (a4,b4))
ax.set(xlabel='Intensidad promedio', ylabel='Simetria', title='PLA-Pocketsobre datos Test')
ax.set_xlim((0, 1))
plt.legend()
plt.show()



input("\n--- Pulsar tecla para continuar ---\n")

print("Obtener Cotas\n")
#Funcion para el cálculo de cota sobre el error
def calcular_cota(error, tam, dim, tolerancia ):
    return error + np.sqrt((8 / tam) * np.log((4 * ((2 * tam) ** dim + 1)) / tolerancia))

## Acotacion de los errores
Ein_cota = calcular_cota(Ein,x.shape[0], 3 ,0.05)
Eout_cota = calcular_cota(Eout,x_test.shape[0], 3, 0.05)

print("Cota de Ein: ", Ein_cota)
print("Cota de Eout: ", Eout_cota)




















#