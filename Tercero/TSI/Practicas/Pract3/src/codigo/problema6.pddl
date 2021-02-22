
(define (problem problema6)
   (:domain dominio6)
   (:objects
      centroMando - edificios
      barracon - edificios
      extractor - edificios
      u1 u2 u3 - unidades
      ma1 ma2 - unidades
      s1 - unidades
      m1 m2 m3 - recursos
      g1 g2 - recursos
      ;Añado edificio e investigacion
      impulsor_S - investigaciones
      bahia - edificios
      deposito - edificios
      l_1_1 l_1_2 l_1_3 l_1_4 l_1_5 - localizaciones
      l_2_1 l_2_2 l_2_3 l_2_4 l_2_5 - localizaciones
      l_3_1 l_3_2 l_3_3 l_3_4 l_3_5 - localizaciones
      l_4_1 l_4_2 l_4_3 l_4_4 l_4_5 - localizaciones
      l_5_1 l_5_2 l_5_3 l_5_4 l_5_5 - localizaciones
   )
   (:init
      ;columnas
      (hayCamino l_1_1 l_2_1) (hayCamino l_1_2 l_2_2) (hayCamino l_1_3 l_2_3) (hayCamino l_1_4 l_2_4) (hayCamino l_1_5 l_2_5)
      (hayCamino l_2_1 l_1_1) (hayCamino l_2_2 l_1_2) (hayCamino l_2_3 l_1_3) (hayCamino l_2_4 l_1_4) (hayCamino l_2_5 l_1_5)
      (hayCamino l_2_1 l_3_1) (hayCamino l_2_2 l_3_2) (hayCamino l_2_3 l_3_3) (hayCamino l_2_4 l_3_4) (hayCamino l_2_5 l_3_5)
      (hayCamino l_3_1 l_2_1) (hayCamino l_3_2 l_2_2) (hayCamino l_3_3 l_2_3) (hayCamino l_3_4 l_2_4) (hayCamino l_3_5 l_2_5)
      (hayCamino l_3_1 l_4_1) (hayCamino l_3_2 l_4_2) (hayCamino l_3_3 l_4_3) (hayCamino l_3_4 l_4_4) (hayCamino l_3_5 l_4_5)
      (hayCamino l_4_1 l_3_1) (hayCamino l_4_2 l_3_2) (hayCamino l_4_3 l_3_3) (hayCamino l_4_4 l_3_4) (hayCamino l_4_5 l_3_5)
      (hayCamino l_4_1 l_5_1) (hayCamino l_4_2 l_5_2) (hayCamino l_4_3 l_5_3) (hayCamino l_4_4 l_5_4) (hayCamino l_4_5 l_5_5)
      (hayCamino l_5_1 l_4_1) (hayCamino l_5_2 l_4_2) (hayCamino l_5_3 l_4_3) (hayCamino l_5_4 l_4_4) (hayCamino l_5_5 l_4_5)
      ;filas
      (hayCamino l_1_1 l_1_2) (hayCamino l_1_2 l_1_1) (hayCamino l_1_2 l_1_3) (hayCamino l_1_3 l_1_2) (hayCamino l_1_3 l_1_4) (hayCamino l_1_4 l_1_3) (hayCamino l_1_4 l_1_5) (hayCamino l_1_5 l_1_4)
      (hayCamino l_2_1 l_2_2) (hayCamino l_2_2 l_2_1) (hayCamino l_2_2 l_2_3) (hayCamino l_2_3 l_2_2) (hayCamino l_2_3 l_2_4) (hayCamino l_2_4 l_2_3) (hayCamino l_2_4 l_2_5) (hayCamino l_2_5 l_2_4)
      (hayCamino l_3_1 l_3_2) (hayCamino l_3_2 l_3_1) (hayCamino l_3_2 l_3_3) (hayCamino l_3_3 l_3_2) (hayCamino l_3_3 l_3_4) (hayCamino l_3_4 l_3_3) (hayCamino l_3_4 l_3_5) (hayCamino l_3_5 l_3_4)
      (hayCamino l_4_1 l_4_2) (hayCamino l_4_2 l_4_1) (hayCamino l_4_2 l_4_3) (hayCamino l_4_3 l_4_2) (hayCamino l_4_3 l_4_4) (hayCamino l_4_4 l_4_3) (hayCamino l_4_4 l_4_5) (hayCamino l_4_5 l_4_4)
      (hayCamino l_5_1 l_5_2) (hayCamino l_5_2 l_5_1) (hayCamino l_2_2 l_2_3) (hayCamino l_5_3 l_5_2) (hayCamino l_5_3 l_5_4) (hayCamino l_5_4 l_5_3) (hayCamino l_5_4 l_5_5) (hayCamino l_5_5 l_5_4)
      ;sitúo en el mapa
      (localizacionUnidad u1 l_5_3)
      (localizacionEdificio centroMando l_3_3)
      (asignarNodoRecurso m1 l_1_3) (asignarNodoRecurso m2 l_4_1) (asignarNodoRecurso m3 l_4_5)
      (asignarNodoRecurso g1 l_3_1) (asignarNodoRecurso g2 l_1_5)
      ;tipos
      (tipoUnidad u1 VCE) (tipoUnidad u2 VCE) (tipoUnidad u3 VCE)
      (tipoUnidad ma1 Marine) (tipoUnidad ma2 Marine)
      (tipoUnidad s1 Segador)
      ;Añado la investigacion y el edificio
      (tipoInvestigacion impulsor_S ImpulsorSegador)
      (tipoEdificio bahia BahiaDeIngenieria)
      (tipoEdificio centroMando CentroDeMando) (tipoEdificio barracon Barracones)
      (tipoEdificio extractor Extractor)
      (tipoEdificio deposito Deposito)
      (tipoRecurso m1 Minerales) (tipoRecurso m2 Minerales) (tipoRecurso m3 Minerales)
      (tipoRecurso g1 Gas) (tipoRecurso g2 Gas)
      ;necesidad para construccion
      (recursoNecesario Minerales Barracones) (recursoNecesario Gas CentroDeMando)
      (recursoNecesario Minerales Extractor)
      (recursoNecesario Minerales CentroDeMando)
      (recursoNecesario Minerales BahiaDeIngenieria)
      (recursoNecesarioUnidad Minerales VCE) (recursoNecesarioUnidad Minerales Marine) (recursoNecesarioUnidad Gas Segador)
      (creaUnidad VCE CentroDeMando) (creaUnidad Marine Barracones) (creaUnidad Segador Barracones)
      ;Añado los recursos para Investigar
      (recursoNecesarioInvestigar Minerales ImpulsorSegador) (recursoNecesarioInvestigar Gas ImpulsorSegador)
      ;Añado los costes
      (= (costeEdificios CentroDeMando Minerales) 150) (= (costeEdificios CentroDeMando Gas) 50)
      (= (costeEdificios Barracones Minerales) 150) (= (costeEdificios Barracones Gas) 0)
      (= (costeEdificios Extractor Minerales) 75) (= (costeEdificios Extractor Gas) 0)
      (= (costeEdificios BahiaDeIngenieria Minerales) 125) (= (costeEdificios BahiaDeIngenieria Gas) 0)
      (= (costeUnidad VCE Minerales) 50) (= (costeUnidad Segador Gas) 0)
      (= (costeUnidad Marine Minerales) 50) (= (costeUnidad Segador Gas) 0)
      (= (costeUnidad Segador Minerales) 50) (= (costeUnidad Segador Gas) 50)
      (= (costeEdificios Deposito Minerales) 75) (= (costeEdificios Deposito Gas) 25)
      (= (costeInvestigacion ImpulsorSegador Minerales) 50) (= (costeInvestigacion ImpulsorSegador Gas) 200)
      (= (cuentaLimite Minerales) 100) (= (cuentaLimite Gas) 100)
      (= (cuentaRecursos Minerales) 0) (= (cuentaRecursos Gas) 0)
      (= (cuentaTrabajadores VCE) 0)
      ;(= (posibleRecoleccion Minerales) 0) (= (posibleRecoleccion Gas) 0)
   )
   (:goal
      (and
         (localizacionUnidad ma1 l_5_2)
         (localizacionUnidad ma2 l_5_4)
         (localizacionUnidad s1 l_5_4)
      )
   )
)
