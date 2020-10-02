El problema de la barra de sushi.
---------------------------------------------------------------------
Suponga que existe una barra de sushi con 5 asientos, si un cliente llega y hay asientos disponibles se puede sentar a comer,
 en caso de que los 5 asientos estén en uso significa que las personas están comiendo juntas y deberá esperar a que todos los 
asientos se desocupen.


Este problema se puede resolver mediante semáforos y mutex con el patrón "Pass the Baton". Un mutex controlará el acceso del hilo al bar y el semáforo 
controlará los hilos que deben esperar por un asiento.


## ¿Se puede presentar alguna situación de deadlock o starvation?
Este ejercicio podría presentar un problema de starvation en el caso de que hayan más de 5 hilos en espera
y cuando ingresen al bar no se controle el orden, según la velocidad de procesamiento podría darse la situación 
de que un hilo quede esperando eternamente, es por esto la importancia del semáforo que controle el acceso a los hilos que esperan.

## Agregue alguna ilustración o pseudocódigo que permita visualizar la solución al problema.

Se encuentra en el documento llamado "ilustración.pdf".
