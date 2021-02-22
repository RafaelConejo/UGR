; Ejercicio 2

(define (domain dominio2)
   (:requirements :typing :strips)
   (:types
      unidades edificios localizaciones
   )
   (:constants
      VCE - unidades
      CentroDeMando - edificios
      Barracones - edificios
      ;Añado
      Extractor - edificios
      Minerales - recursos
      Gas - recursos
   )
   (:predicates
      (localizacionEdificio ?e - edificios ?le - localizaciones)
      (localizacionUnidad ?u - unidades ?lu - localizaciones)
      (hayCamino ?l1 - localizaciones ?l2 - localizaciones)
      (asignarNodoRecurso ?r - recursos ?lr - localizaciones)
      (extrayendo ?u - unidades ?r - recursos)
      (recursoNecesario ?r - recursos ?e - edificios)
      (ocupadoTrabajando ?u - unidades)
      (tipoUnidad ?u ?tu - unidades)
      (tipoEdificio ?e ?te - edificios)
      (tipoRecurso ?r ?tr - recursos)
      (recursosObtenidos ?r - recursos)
      ;Añado predicado para controlar el extractor
      (existeExtractor ?le - localizaciones)
   )
   (:action Navegar
      :parameters (?u - unidades ?l1 ?l2 - localizaciones)
      :precondition
         (and
            (localizacionUnidad ?u ?l1)
            (hayCamino ?l1 ?l2)
         )
      :effect
         (and
            (localizacionUnidad ?u ?l2)
            (not (localizacionUnidad ?u ?l1))
         )
   )
   (:action Asignar
      :parameters (?u - unidades ?r ?tr - recursos ?l - localizaciones)
      :precondition
         (and
            ;Añado
            (tipoUnidad ?u VCE)
            ;
            (not (ocupadoTrabajando ?u))
            (localizacionUnidad ?u ?l)
            (asignarNodoRecurso ?r ?l)
            ;Añado
            (tipoRecurso ?r ?tr)
            (or
               (tipoRecurso ?r Minerales)
               (and
                  (tipoRecurso ?r Gas)
                  (existeExtractor ?l)
               )
            )
         )
      :effect
         (and
            (ocupadoTrabajando ?u)
            (extrayendo ?u ?r)
            (recursosObtenidos ?tr)
         )
   )
   (:action Construir
      :parameters (?u - unidades ?tr - recursos ?e ?te - edificios ?l - localizaciones)
      :precondition
         (and
            ;Añado
            (tipoUnidad ?u VCE)
            (or
               (not (tipoEdificio ?e Extractor))
               (and
                  (tipoEdificio ?e Extractor)
                  (exists (?r - recursos)
                     (and
                        (tipoRecurso ?r Gas)
                        (asignarNodoRecurso ?r ?l)
                     )
                  )
               )
            )
            (not (ocupadoTrabajando ?u))
            (not (localizacionEdificio ?e ?l))
            (localizacionUnidad ?u ?l)
            (tipoEdificio ?e ?te)
            (tipoUnidad ?u VCE)
            (recursoNecesario ?tr ?te)
            (recursosObtenidos ?tr)
         )
      :effect
         (and
            (localizacionEdificio ?e ?l)
            (when (tipoEdificio ?e Extractor)
               (existeExtractor ?l)
            )
         )
   )
)
