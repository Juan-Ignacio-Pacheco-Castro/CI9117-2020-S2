# CI0117 - Actividad 6

##### Programación Concurrente

* ##### B82957 - Marco Ferraro
* ##### B86524 - Gabriel Revillat

## omp_for

Divide el trabajo hecho en un ciclo for dentro de una región paralela entre los hilos.

### Parámetros

*Cláusulas*

*     private
*     firstprivate
*     lastprivate
*     reduction
*     ordered
*     schedule
*     nowait

*Instrucción for*

Un ciclo *for*. Si el código del usuario cambia la variable índice dentro del ciclo, puede causar comportamiento indefinido.

