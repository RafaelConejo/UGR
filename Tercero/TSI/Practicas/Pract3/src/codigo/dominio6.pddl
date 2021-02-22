; Ejercicio 6

(define (domain dominio6)
   (:requirements :typing :strips)
   (:types
      ;Añado nuevo tipo
      unidades edificios localizaciones investigaciones 
   )
   (:constants
      VCE - unidades
      CentroDeMando - edificios
      Barracones - edificios
      Extractor - edificios
      Minerales - recursos
      Gas - recursos
      Marine Segador - unidades
      BahiaDeIngenieria - edificios
      ImpulsorSegador - investigaciones
      Deposito - edificios
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
      (existeExtractor ?le - localizaciones)
      (creaUnidad ?u - unidades ?e - edificios)
      (recursoNecesarioUnidad ?r - recursos ?u - unidades)
      (investigacionCompleta ?i - investigaciones)
      (recursoNecesarioInvestigar ?r - recursos ?i - investigaciones)
      (tipoInvestigacion ?i ?ti - investigaciones)
      ;Añado un predicado para controlar que un alma
      (almacenRecursoLleno ?r - recursos)
   )
   (:functions
     (cuentaRecursos ?r - recursos)
     (cuentaTrabajadores ?u - unidades)
     (cuentaLimite ?r - recursos)
     ;Añado los costes
     (costeEdificios ?e - edificios ?r - recursos)
     (costeUnidad ?u - unidades ?r - recursos)
     (costeInvestigacion ?i - investigaciones ?r - recursos)
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
            (tipoUnidad ?u VCE)
            (not (ocupadoTrabajando ?u))
            (localizacionUnidad ?u ?l)
            (asignarNodoRecurso ?r ?l)
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
            (increase (cuentaTrabajadores VCE) 1)
            (ocupadoTrabajando ?u)
            (extrayendo ?u ?r)
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
            (forall (?t - recursos)
               (imply (recursoNecesario ?t ?te)
                  (>= (cuentaRecursos ?t) (costeEdificios ?te ?t))
               )
            )
         )
      :effect
         (and
            (localizacionEdificio ?e ?l)
            (when (tipoEdificio ?e Extractor)
               (existeExtractor ?l)
            )
            (forall (?t - recursos)
               (when (tipoEdificio ?e Deposito)
                  (and
                     (increase (cuentaLimite ?t) 100)
                     (not (almacenRecursoLleno ?t))
                  )
               )
            )
            (forall (?t - recursos)
               (when (recursoNecesario ?t ?te)
                  (and
                     (decrease (cuentaRecursos ?t) (costeEdificios ?te ?t))
                     (not (almacenRecursoLleno ?t))
                  )
               )
            )
         )
   )
   (:action Reclutar
      :parameters (?u ?tu - unidades ?e ?te - edificios ?l - localizaciones)
      :precondition
         (and
            (tipoEdificio ?e ?te)
            (localizacionEdificio ?e ?l)
            (not (exists (?l0 - localizaciones) (localizacionUnidad ?u ?l0)))
            (creaUnidad ?tu ?te)
            (forall (?t - recursos)
               (imply (recursoNecesarioUnidad ?t ?tu)
                  (>= (cuentaRecursos ?t) (costeUnidad ?tu ?t))
               )
            )
            (tipoUnidad ?u ?tu)
            (or
               (not (tipoUnidad ?u Segador))
               (and
                  (tipoUnidad ?u Segador)
                  (exists (?i - investigaciones)
                     (and
                        (investigacionCompleta ?i)
                        (tipoInvestigacion ?i ImpulsorSegador)
                     )
                  )
               )
            )
         )
      :effect
         (and
            (localizacionUnidad ?u ?l)
            (forall (?t - recursos)
               (when (recursoNecesarioUnidad ?t ?tu)
                  (and
                     (decrease (cuentaRecursos ?t) (costeUnidad ?tu ?t))
                     (not (almacenRecursoLleno ?t))
                  )
               )
            )
         )
   )
   (:action Investigar
      :parameters(?e - edificios ?i ?ti - investigaciones ?l - localizaciones)
      :precondition
         (and
            (tipoEdificio ?e BahiaDeIngenieria)
            (localizacionEdificio ?e ?l)
            (not (investigacionCompleta ?i))
            (forall (?t - recursos)
               (imply (recursoNecesarioInvestigar ?t ?ti)
                  (>= (cuentaRecursos ?t) (costeInvestigacion ?ti ?t))
               )
            )
         )
      :effect
         (and
            (investigacionCompleta ?i)
            (forall (?t - recursos)
               (when (recursoNecesarioInvestigar ?t ?ti)
                  (and
                     (decrease (cuentaRecursos ?t) (costeInvestigacion ?ti ?t))
                     (not (almacenRecursoLleno ?t))
                  )
               )
            )
         )
   )
   (:action Recolectar
      :parameters (?u ?tu - unidades ?r ?tr - recursos ?l - localizaciones)
      :precondition
         (and
            (tipoUnidad ?u VCE)
            (ocupadoTrabajando ?u)
            (extrayendo ?u ?r)
            (localizacionUnidad ?u ?l)
            (asignarNodoRecurso ?r ?l)
            (tipoRecurso ?r ?tr)
            (not (almacenRecursoLleno ?tr))
            (< (cuentaRecursos ?tr) (cuentaLimite ?tr))
         )
      :effect
         (and
            (increase (cuentaRecursos ?tr) (* (cuentaTrabajadores ?tu) 50))
            (when (>= (cuentaRecursos ?tr) (cuentaLimite ?tr))
               (and
                  (assign (cuentaRecursos ?tr) (cuentaLimite ?tr))
                  (almacenRecursoLleno ?tr)
               )
            )
         )
   )
   (:action Desasignar
      :parameters (?u - unidades ?r ?tr - recursos ?l - localizaciones)
      :precondition
         (and
            (tipoUnidad ?u VCE)
            (ocupadoTrabajando ?u)
            (localizacionUnidad ?u ?l)
            (asignarNodoRecurso ?r ?l)
            (tipoRecurso ?r ?tr)
         )
      :effect
         (and
            (not (ocupadoTrabajando ?u))
            (not(extrayendo ?u ?r))
            (decrease (cuentaTrabajadores VCE) 1)
         )
   )
)
