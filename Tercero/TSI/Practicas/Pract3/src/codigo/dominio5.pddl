; Ejercicio 5

(define (domain dominio5)
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
      ;Añado nuevo edificio e investigacion
      BahiaDeIngenieria - edificios
      ImpulsorSegador - investigaciones
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
      (existeExtractor ?le - localizaciones)
      (creaUnidad ?u - unidades ?e - edificios)
      (recursoNecesarioUnidad ?r - recursos ?u - unidades)
      ;Añado para controlar las investigaciones
      (investigacionCompleta ?i - investigaciones)
      (recursoNecesarioInvestigar ?r - recursos ?i - investigaciones)
      (tipoInvestigacion ?i ?ti - investigaciones)
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
            (forall (?t - recursos)
               (imply (recursoNecesario ?t ?te)
                  (recursosObtenidos ?t)
               )
            )
         )
      :effect
         (and
            (localizacionEdificio ?e ?l)
            (when (tipoEdificio ?e Extractor)
               (existeExtractor ?l)
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
                  (recursosObtenidos ?t)
               )
            )
            ;Añado para solo dejar crear segadores si hay bahía
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
                  (recursosObtenidos ?t)
               )
            )
         )
      :effect
         (and
            (investigacionCompleta ?i)
         )
   )
)
