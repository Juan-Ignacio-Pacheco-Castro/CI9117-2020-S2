# Lab 4: Semaphores
## Unisex bathroom problem

En una oficina hay un unico baño, que solo se puede usar bajo las siguientes condiciones:

- No puede haber hombres y mujeres simultaneamente
- No pueden haber mas de 3 personas a la vez

## Solucion usando semaphores

Se utilizan las variables

- Semaphore empty: 1 si vacio, 0 si no
- Semaphore maleMultiplex: se asegura de que no hayan mas de 3 hombres
- Semaphore femaleMultiplex: igual a femaleMultiplex
- Lightswitch maleSwitch: lockea empty cuando hay hombres
- Lightswitch femaleSwitch: igual a maleSwitch

## Pseudocodigo

Se libera el espacio en memoria para la cantidad de hilos necesarios

Para i=0 ... hasta la cantidad de threads:
    se genera un numero pseudo aleatorio

    Si es un numero par:
        se llama un pthread hombre
        si hay mujeres, espera a que todas salgan
        si es el primer hombre, lockea empty para que no entren mujeres
        si no es el primero, solo entra y aumenta el contador de la cantidad de hombres
        si hay 3 hombres, espera

        al salir, si es el ultimo, unlockea empty para que entre cualquiera

    Si no es par:
        se llama un pthread mujer
        si hay hombres, espera a que todos salgan
        si es la primer mujer, lockea empty para que no entren hombres
        si no es la primera, solo entra y aumenra el contador de la cantidad de mujeres
        si hay 3 mujeres, espera

        al salir, si es la ultima, unlockea empty para que entre cualquiera

Se espera a que terminen los hilos


## Deadlock

En caso de no tener cuidado a la hora de decrementar los contadores
se podria llegar a una situacion en la que no hay nadie adentro
pero los que estan haciendo fila siguen esperando que se libere espacio
pensado que ya hay 3 personas adentro

## Starvation

Si no se usa un turnstile, se puede formar una fila larga de mujeres u hombres
y el sexo opuesto tendria que esperar a que todos salgan, en vez de respetar el orden
en que llegaron
