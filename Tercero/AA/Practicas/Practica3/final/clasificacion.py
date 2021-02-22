#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  8 20:41:36 2020

@author: rafa
"""


#Clasificacion

import numpy as np
import pandas as pd
from sklearn.preprocessing import MaxAbsScaler, normalize
from sklearn import svm, metrics, linear_model, neighbors
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt


# Fijamos la semilla
np.random.seed(49137372)


# Leemos a partir del fichero
df = pd.read_csv('datos/optdigits.tra')

# Guardamos en una variable auxiliar los datos pero sin el
# valor de las etiquetas
df_aux = df.copy()
df_aux = df_aux.iloc[:, :-1]

# Asignamos un escalador y lo aplicamos al conjunto de características
scaled_df = df_aux.copy()
scaler = MaxAbsScaler()
scaled = scaler.fit_transform(df_aux)
scaled_df.loc[:,:] = scaled

# Metemos en la variables el nuevo conjunto escalado
train_x = np.array(scaled_df)[:,:-1]
train_y = np.array(df)[:,-1:]

#Repetimos
# Leemos a partir del fichero
df = pd.read_csv('datos/optdigits.tes')

# Guardamos en una variable auxiliar los datos pero sin el
# valor de las etiquetas
df_aux = df.copy()
df_aux = df_aux.iloc[:, :-1]

# Asignamos un escalador y lo aplicamos al conjunto de características
scaled_df = df_aux.copy()
scaler = MaxAbsScaler()
scaled = scaler.fit_transform(df_aux)
scaled_df.loc[:,:] = scaled

# Metemos en la variables el nuevo conjunto escalado
test_x = np.array(scaled_df)[:,:-1]
test_y = np.array(df)[:,-1:]


    
    
def SGDClassifier(penality, max_iter, lr, epsilon):
    # Creamos el clasificador
    classifier = linear_model.SGDClassifier(penalty=penality, max_iter=max_iter, alpha = lr, tol=epsilon)
    #Entrenamos nuestro clasificador con los datos de entrenamiento
    classifier.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = classifier.predict(test_x)

    return esperado, predecido
    




def Perceptron(penality, max_iter, epsilon):
    
    # Creamos el clasificador
    classifier = linear_model.Perceptron(penalty=penality, max_iter=max_iter, tol=epsilon)
    # Entrenamos nuestro clasificador con los datos de entrenamiento
    classifier.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = classifier.predict(test_x)

    return esperado, predecido





def RegresionLogistica(penality, max_iter):
    # Creamos el clasificador
    classifier = linear_model.LogisticRegression(penalty=penality, max_iter=max_iter)
    # Entrenamos nuestro clasificador con los datos de entrenamiento
    classifier.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = classifier.predict(test_x)

    return esperado, predecido








# Distribución de valores de las etiquetas
plt.hist(test_y, edgecolor='white', linewidth=1.2)
plt.title("Distribución de valores de etiquetas (digit)")
plt.xlabel("Dígito escrito")
plt.ylabel("Nº instancias")
plt.show()
    
input("\n--- Pulsar tecla para continuar ---\n")
    
    
print('\n\n\nGRADIENTE DESCENDENTE ESTOCASTICO (SGD)')
esperado, predecido = SGDClassifier('l2', 20000, 0.001, 1e-4)
print("\nInforme de Casillas: ")
print(metrics.classification_report(esperado, predecido))
print("\nMatriz de Confusion:")
print(metrics.confusion_matrix(esperado, predecido))
input("\n--- Pulsar tecla para continuar ---\n")


print('\n\n\nPERCEPTRON')
esperado, predecido = Perceptron('l2', 20000, 1e-4)
print("\nInforme de Casillas: ")
print(metrics.classification_report(esperado, predecido))
print("\nMatriz de Confusion:")
print(metrics.confusion_matrix(esperado, predecido))
input("\n--- Pulsar tecla para continuar ---\n")


print('\n\n\nREGRESION LOGISTICA')
esperado, predecido = RegresionLogistica('l2', 20000)
print("\nInforme de Casillas: ")
print(metrics.classification_report(esperado, predecido))
print("\nMatriz de Confusion:")
print(metrics.confusion_matrix(esperado, predecido))








