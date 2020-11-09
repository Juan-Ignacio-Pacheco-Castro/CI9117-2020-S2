# omp_lock

OpenMP tambien tiene un mecanismo para poner un candado. Un candado es similar a la seccion crítica: garantiza que algunas instrucciones se van a procesar un hilo a la vez. Sin embargo, la seccion crítica actúa sobre el código, mientras que un candado es sobre los datos. Con un candado uno se asegura de que algunos elementos de información solo pueden ser tocados por un hilo a la vez.

The type omp_lock_t represents a simple lock:
1. void omp\_init\_lock(omp\_lock\_t* lock) ; inicializa el lock
2. void omp\_init\_lock\_with\_hint(omp\_lock\_t *lock, omp\_sync\_hint\_t hint) ; init and attaches a hint
3. void omp\_destroy\_lock(omp\_lock\_t *lock) ; destruye el lock
4. void omp\_set\_lock(omp\_lock\_t *lock) ; lock
5. void omp\_unset\_lock(omp\_lock\_t *lock) ; unlock
6. int omp\_test\_lock(omp\_lock\_t *lock) ; tests a simple lock and sets if it is available

## Equipo de Desarrollo
- David Xie (B88682)
- Juan Ignacio Pacheco (B85841)
