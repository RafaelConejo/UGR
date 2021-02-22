#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  8 20:41:36 2020

@author: rafa
"""


#Regresión

import numpy as np
import pandas as pd
from sklearn.preprocessing import MaxAbsScaler, normalize
from sklearn import svm, metrics, linear_model, neighbors
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt


# Fijamos la semilla
np.random.seed(49137372)

cabeceras = ['state' , 'county' , 'community' , 'comm name' , 'fold' , 'population' , 'householdsize' , 'racepctblack' , 'racePctWhite' , 'racePctAsian' , 'racePctHisp' , 'agePct12t21' , 'agePct12t29' , 'agePct16t24' , 'agePct65up' , 'numbUrban' , 'pctUrban' , 'medIncome' , 'pctWWage' , 'pctWFarmSelf' , 'pctWInvInc' , 'pctWSocSec' , 'pctWPubAsst' , 'pctWRetire' , 'medFamInc' , 'perCapInc' , 'whitePerCap' , 'blackPerCap' , 'indianPerCap' , 'AsianPerCap' , 'OtherPerCap' , 'HispPerCap' , 'NumUnderPov' , 'PctPopUnderPov' , 'PctLess9thGrade' , 'PctNotHSGrad' , 'PctBSorMore' , 'PctUnemployed' , 'PctEmploy' , 'PctEmplManu' , 'PctEmplProfServ' , 'PctOccupManu' , 'PctOccupMgmtProf' , 'MalePctDivorce' , 'MalePctNevMarr' , 'FemalePctDiv' , 'TotalPctDiv' , 'PersPerFam' , 'PctFam2Par' , 'PctKids2Par' , 'PctYoungKids2Par' , 'PctTeen2Par' , 'PctWorkMomYoungKids' , 'PctWorkMom' , 'NumIlleg' , 'PctIlleg' , 'NumImmig' , 'PctImmigRecent' , 'PctImmigRec5' , 'PctImmigRec8' , 'PctImmigRec10' , 'PctRecentImmig' , 'PctRecImmig5' , 'PctRecImmig8' , 'PctRecImmig10' , 'PctSpeakEnglOnly' , 'PctNotSpeakEnglWell' , 'PctLargHouseFam' , 'PctLargHouseOccup' , 'PersPerOccupHous' , 'PersPerOwnOccHous' , 'PersPerRentOccHous' , 'PctPersOwnOccup' , 'PctPersDenseHous' , 'PctHousLess3BR' , 'MedNumBR' , 'HousVacant' , 'PctHousOccup' , 'PctHousOwnOcc' , 'PctVacantBoarded' , 'PctVacMore6Mos' , 'MedYrHousBuilt' , 'PctHousNoPhone' , 'PctWOFullPlumb' , 'OwnOccLowQuart' , 'OwnOccMedVal' , 'OwnOccHiQuart' , 'RentLowQ' , 'RentMedian' , 'RentHighQ' , 'MedRent' , 'MedRentPctHousInc' , 'MedOwnCostPctInc' , 'MedOwnCostPctIncNoMtg' , 'NumInShelters' , 'NumStreet' , 'PctForeignBorn' , 'PctBornSameState' , 'PctSameHouse85' , 'PctSameCity85' , 'PctSameState85' , 'LemasSwornFT' , 'LemasSwFTPerPop' , 'LemasSwFTFieldOps' , 'LemasSwFTFieldPerPop' , 'LemasTotalReq' , 'LemasTotReqPerPop' , 'PolicReqPerOffic' , 'PolicPerPop' , 'RacialMatchCommPol' , 'PctPolicWhite' , 'PctPolicBlack' , 'PctPolicHisp' , 'PctPolicAsian' , 'PctPolicMinor' , 'OfficAssgnDrugUnits' , 'NumKindsDrugsSeiz' , 'PolicAveOTWorked' , 'LandArea' , 'PopDens' , 'PctUsePubTrans' , 'PolicCars' , 'PolicOperBudg' , 'LemasPctPolicOnPatr' , 'LemasGangUnitDeploy' , 'LemasPctOfficDrugUn' , 'PolicBudgPerPop' , 'ViolentCrimesPerPop']
innecesario = ['state' , 'county' , 'community' , 'comm name' , 'fold']
incompletos = ['LemasSwornFT', 'LemasSwFTPerPop', 'LemasSwFTFieldOps', 'LemasSwFTFieldPerPop', 'LemasTotalReq', 'LemasTotReqPerPop', 'PolicReqPerOffic', 'PolicPerPop', 'RacialMatchCommPol', 'PctPolicWhite', 'PctPolicBlack', 'PctPolicHisp', 'PctPolicAsian', 'PctPolicMinor', 'OfficAssgnDrugUnits', 'NumKindsDrugsSeiz', 'PolicAveOTWorked',  'PolicCars', 'PolicOperBudg', 'LemasPctPolicOnPatr', 'LemasGangUnitDeploy', 'PolicBudgPerPop']



df = pd.read_csv('datos/communities.data', names = cabeceras)
#Para esta parte de eliminar las columnas no necesarias he recibido consejo de un compañero de clase
#eliminamos las 5 primeras columnas que no nos son útiles
df = df.drop(innecesario, 1)
#Quito los datos con muchas "?"
df = df.drop(incompletos, 1)
#Le asigno un valor medio para evitar la presencia de "?"
df.loc[130, 'OtherPerCap'] = 0.28
#Tras esto no nos quedan "?"

#paso a numpy los datos
datos = df.values

#Separo lo datos
x_datos, y_datos = np.split(datos, [-1], 1)

#Separo en mis conjuntos train y test
train_x, test_x, train_y, test_y = train_test_split(x_datos, y_datos, test_size=0.2, shuffle=True)




def RegresionLineal():
    # Creamos el regresor 
    regressor = linear_model.LinearRegression()
    # Lo entrenamos con los datos de entrenamiento
    regressor.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = regressor.predict(test_x)
    return esperado, predecido
    




def SGDRegressor(penality, max_iter, lr, epsilon):    
    # Creamos el regresor 
    regressor = linear_model.SGDRegressor(penalty = penality, max_iter = max_iter, alpha = lr, tol = epsilon)
    # Lo entrenamos con los datos de entrenamiento
    regressor.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = regressor.predict(test_x)

    return esperado, predecido




def LinearSVR(C, max_iter, epsilon):   
    # Creamos el regresor 
    regressor = svm.LinearSVR(C = C, max_iter = max_iter, tol = epsilon)
    # Lo entrenamos con los datos de entrenamiento
    regressor.fit(train_x, train_y.ravel())
    # Creamos dos variables: valor esperado y el que predecimos
    esperado = test_y
    predecido = regressor.predict(test_x)

    return esperado, predecido






print('\n\n\nREGRESION LINEAL')
esperado, predecido = RegresionLineal()
print("Mean Absolut Error: ", metrics.mean_absolute_error(esperado, predecido))
print("Mean Squared Error: ", metrics.mean_squared_error(esperado, predecido))
print("\nCoefficient of determination : ", metrics.r2_score(esperado, predecido))
input("\n--- Pulsar tecla para continuar ---\n")

print('\n\n\nGRADIENTE DESCENDENTE ESTOCASTICO')
esperado, predecido = SGDRegressor('l2', 20000, 0.001, 1e-4)
print("Mean Absolut Error: ", metrics.mean_absolute_error(esperado, predecido))
print("Mean Squared Error: ", metrics.mean_squared_error(esperado, predecido))
print("\nCoefficient of determination : ", metrics.r2_score(esperado, predecido))
input("\n--- Pulsar tecla para continuar ---\n")


print('\n\n\nSUPPORT VECTOR MACHINE (SVR)')
esperado, predecido = LinearSVR(1.0, 20000, 1e-4)
print("Mean Absolut Error: ", metrics.mean_absolute_error(esperado, predecido))
print("Mean Squared Error: ", metrics.mean_squared_error(esperado, predecido))
print("\nCoefficient of determination : ", metrics.r2_score(esperado, predecido))