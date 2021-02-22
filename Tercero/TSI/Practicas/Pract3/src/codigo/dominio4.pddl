; Ejercicio 4

(define (domain dominio4)
   (:requirements :typing :strips)
   (:types
      unidades edificios localizaciones
   )
   (:constants
      VCE - unidades
      CentroDeMando - edificios
      Barracones - edificios
      Extractor - edificios
      Minerales - recursos
      Gas - recursos
      ;Añado
      Marine Segador - unidades
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
      ;Añado creación de unidad
      (creaUnidad ?u - unidades ?e - edificios)
      (recursoNecesarioUnidad ?r - recursos ?u - unidades)
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
            (tipoUnidad ?u ?tu)
            (tipoEdificio ?e ?te)
            (localizacionEdificio ?e ?l)
            (not (exists (?l0 - localizaciones) (localizacionUnidad ?u ?l0)))
            (creaUnidad ?tu ?te)
            (forall (?t - recursos)
               (imply (recursoNecesarioUnidad ?t ?tu)
                  (recursosObtenidos ?t)
               )
            )
         )
      :effect
         (and
            (localizacionUnidad ?u ?l)
         )
   )
)
