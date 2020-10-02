# Laboratorio 4 #
Nelson Álvarez Cedeño
Dean Vargas Delgado

## Building H2O ##

### Descripción del problema ###
Hay dos tipos de hilos, oxígeno e hidrógeno. El objetivo es agrupar estos hilos para formar moléculas de agua.  Se debe utilizar un mecanismo de tal forma que cada hilo deba esperar hasta que se pueda formar una molécula completa.  Estos hilos deben pasar el mecanismo en grupos de tres y deben poder unirse inmediatamente entre sí para formar una molécula de agua. Debe garantizar que todos los hilos de una molécula se unan antes que cualquier otro hilo de la siguiente molécula.

### ¿Se puede presentar alguna situación de deadlock o starvation? ###

En una solución con mutex se puede presentar un deadlock si presenta problemas en su implementación, por ejemplo, si se le da al oxígeno la tarea de desbloquear los mutex de los hidrógenos y al mismo tiempo se necesitara de los hidrógenos para desbloquear los mutex de los oxígenos.
En otra solución se podría generar una dependencia en la que el oxígeno esté esperando a que los hidrógenos estén listos pero la señal de que están listos ocurrió poquísimo antes y se genera starvation.

### Ilustración de la solución ###

Una posible visualización más sencilla de la solución al problema:

    • Si un hilo de oxígeno llega cuando no hay hilos de hidrógeno presentes, tiene que esperar dos hilos de hidrógeno.
      
    •  Si un hilo de hidrógeno llega cuando no hay otros hilos presentes, tiene que esperar un hilo de oxígeno y otro hilo de hidrógeno.

#### Pseudocódigo Oxígeno

```
lock mutex
if h_waiting < 2:
    o_waiting += 1
    wait for hydrogen ready
else:
    h_wainting -= 2
    signal oxygen ready
    signal oxygen ready
release oxygen
unlock mutex
```

#### Pseudocódigo Hidrogeno

```
lock mutex
if o_waiting > 0 and h_waiting >= 1:
    h_waiting -= 1
    signal hydrogen ready
    o_waiting -= 1
    signal oxygen ready
else:
    h_waiting += 1
    wait for oxygen ready
release hydrogen
unlock mutex
```

**Ejemplos input y output**

**Ejemplo 1:**
Input: "HOH"
Output: "HHO"
Aclaración: "HOH" y "OHH" son también outputs válidos.

**Ejemplo 2:**
Input: "OOHHHH"
Output: "HHOHHO"
Aclaración: "HOHHHO", "OHHHHO", "HHOHOH", "HOHHOH", "OHHHOH", "HHOOHH", "HOHOHH" y "OHHOHH" son también outputs válidos.
