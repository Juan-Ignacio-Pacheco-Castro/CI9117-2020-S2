
# Laboratorio 4: Dining savages problem

## Equipo de desarrollo

* **Juan Ignacio Pacheco**
* **David Xie Li**

Este proyecto es parte del curso de CI-0117 Programación Paralela y Concurrente de la Universidad de Costa Rica. Realizado durante el segundo semestre del año 2020.

## Descripción del problema/patrón de sincronización.

Tenemos varios salvajes que son los hilos que van a pedir recursos (ocupan comer de una olla compartida) y tenemos nada más un cocinero (un solo hilo) que se dedica a meter todos los recursos (llenar la olla) y mandarle la señal a los salvajes que están esperando. Ahora bien, el cocinero no puede estar metiendo recursos cada vez que alguien llega a servirse, solo puede rellenar en caso de que la olla esté vacía. Y los salvajes no pueden comer de una olla vacía, sino que le dicen al cocinero que la rellena y mientras tanto se espera y si vienen más salvajes por detrás, hacen fila.

## ¿Se puede presentar alguna situación de deadlock o starvation? Explique.
Si no existiera el ```mutex.wait()``` en savages, varios salvajes entrarian al ```if (servings == 0)``` y todos le mandarían la señal al chef de que no hay comida. Pero el chef solo le enviaría a uno que se rellenó la olla. La solución que tenemos lo evita debido a que el último thread que llega cuando el cocinero ya se fue, tiene una condición que le permite saltarse todos los procesos y acabar su función.

En la solución que presentamos no se da deadlock, pero puede suceder si se manejara de forma diferente los mutex. Noten que esta solución libre de deadlock es casi que serial.

## Pseudocódigo de la solución al problema, basado en el libro de The Little book of Semaphores
### Cooker:
```
while True:
    emptyPot.wait()
    putServingsInPot(M)
    fullPot.signal()
```

### Savages:
```
while True:
    mutex.wait()
        if servings == 0:
            emptyPot.signal()
            fullPot.wait()
            servings = M
        servings -= 1
        getServingFromPot()
    mutex.signal()
eat()
```

