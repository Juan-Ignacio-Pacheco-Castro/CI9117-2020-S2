## Descripción del problema / patrón de sincronización

Supongamos que hay n pasajeros y una montaña rusa. Los pasajeros esperan
repetidamente para viajar en el coche, que puede contener el máximo de pasajeros
 C, donde C < n. Sin embargo, el coche puede ir alrededor de la pista sólo
 cuando está lleno. Después de terminar un paseo, cada pasajero vaga alrededor
 del parque de diversiones antes de regresar a la montaña rusa para otro paseo.
 Debido a razones de seguridad, el coche sólo monta tiempos T y luego se
 dispara.

Supongamos que el coche y cada pasajero están representados por un hilo.
¿Cómo escribirías un programa, usando sólo semáforos, que pueda simular este
sistema y cumplir con los siguientes requisitos:

> 1. El coche siempre viaja con exactamente C pasajeros;
> 1. Ningún pasajero saltará del coche mientras el coche esté en marcha;
> 1. Ningún pasajero saltará del coche mientras el coche esté en marcha;
> 1. Ningún pasajero solicitará otro viaje antes de poder bajar del coche.

## ¿Se puede presentar alguna situación de deadlock o starvation?
Si no se cumple la condición de que el número de pasajeros es mayor a la
capacidad minima del carro entonces el carro esperaria para siempre.

## Diagrama de la solución
![Diagrama de flujo](./DIAGRAM-roller-1.jpg)
  
Tomado de *[este sitio](https://pages.mtu.edu/~shene/NSF-3/e-Book/SEMA/TM-example-roller.html)*
