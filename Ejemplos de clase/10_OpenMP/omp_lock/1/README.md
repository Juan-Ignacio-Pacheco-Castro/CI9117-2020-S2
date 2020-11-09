# [omp_lock]

Es una rutina de propósito general, es similar a lo de sección crítica, ya que garantiza que algunas instrucciones puedan ser solo realizadas por un hilo cada vez. Es utilizado más al momento de acceder datos, aunque no tiene diferencia al `omp critical`.

- Uso:
```C
omp_lock_t lock;
omp_init_lock (&lock);

omp_set_lock (&lock);
//Código/datos a acceder
omp_unset_lock (&lock);
```
Es similar a como funcionan los mutex en pthread.

El `omp_set_lock` hace que el hilo que llega adquiera el lock y los demás deberán esperar hasta que el lock vuelva a estar disponible y esto ocurre cuando se utiliza el `omp_unset_lock`.

### Referencias:
- OpenMP - Manuales de Referencia: https://www.openmp.org/resources/refguides/
https://www.openmp.org/spec-html/5.0/openmpse31.html
- OpenMP topic: Synchronization - Locks: https://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-sync.html#Locks