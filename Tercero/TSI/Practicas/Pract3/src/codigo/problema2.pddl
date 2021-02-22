
(define (problem problema2)
   (:domain dominio2)
   (:objects
      centroMando - edificios
      barracon - edificios
      extractor - edificios
      u1 u2 u3 - unidades
      m1 m2 m3 - recursos
      g1 g2 - recursos
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
      (localizacionUnidad u1 l_1_1) (localizacionUnidad u2 l_3_5) (localizacionUnidad u3 l_5_3)
      (localizacionEdificio centroMando l_3_3)
      (asignarNodoRecurso m1 l_1_3) (asignarNodoRecurso m2 l_4_1) (asignarNodoRecurso m3 l_4_5)
      (asignarNodoRecurso g1 l_3_1) (asignarNodoRecurso g2 l_1_5)
      ;tipos
      (tipoUnidad u1 VCE) (tipoUnidad u2 VCE) (tipoUnidad u3 VCE)
      (tipoEdificio centroMando CentroDeMando) (tipoEdificio barracon Barracones)
      ;Añado
      (tipoEdificio extractor Extractor)
      (tipoRecurso m1 Minerales) (tipoRecurso m2 Minerales) (tipoRecurso m3 Minerales)
      (tipoRecurso g1 Gas) (tipoRecurso g2 Gas)
      ;necesidad para construccion
      (recursoNecesario Minerales Barracones) (recursoNecesario Gas CentroDeMando)
      ;Añado
      (recursoNecesario Minerales Extractor)
   )
   (:goal
      (localizacionEdificio barracon l_4_3)
   )
)
