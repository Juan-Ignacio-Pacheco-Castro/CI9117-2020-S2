# Actividad 6: Demostración de la función omp_get_max_threads()

`omp_get_max_threads()` es una función que retorna la máxima cantidad de hilos que se van a utilizar para una región paralela que no esté utilizando la directiva de `num_threads`. Esta función por debajo retorna la variable de entorno `OMP_NUM_THREADS`. Si se modifica con `omp_set_num_threads()` entonces se retornará la nueva cantidad.

Los siguientes resultados fueron probando como entrada a = 1, b = 2, n y threads correspondientes. Procesador: Intel i5-8600K (6 Cores, 6 Threads) @ 4.300GHz
|             |Area           |        1        | 16        | 32          | 64          |  128        |
|-------------|---------------|-----------------|-----------|-------------|-------------|-------------|
|1            |2.0000000000   |   0.000014s     |0.001239s  | 0.002215s   | 0.002379s   | 0.008859s   |
|1000         |3.3318335000   |   0.000067s     |0.000575s  | 0.002161s   | 0.004227s   | 0.007602s   |
|1000000      |3.3333318333   |   0.040461s     |0.013479s  | 0.012469s   | 0.014243s   | 0.018415s   |
|1000000000   |3.3333333318   |   10.474789s    |1.843816s  | 1.836464s   | 1.828272s   | 1.836874s   |
|10000000000  |3.3333333332   |   104.335492s   |18.371097s | 17.949467s  | 17.942122s  | 17.968117s  |
|100000000000 |3.3333333333   |   1049.967850s  |184.797518s| 184.118730s | 184.916796s | 179.331424s |

Con lo que logré optimizar, parece ser que con la biblioteca de Pthreads se puede llegar a obtener mejores tiempos de ejecución que con OpenMP:

|               |Area 			|1              | 16 	  	| 32 			| 64 			|
|---------------|---------------|---------------|-----------|---------------|---------------|
|1				| 2.0000000000 	|   0.000225s 	|0.001041s	| 0.002080s 	| 0.005082s		|
|1000  			| 3.3318335000 	|   0.049510s 	|0.001575s	| 0.002502s 	| 0.005427s		|
|1000000 		| 3.3333318333 	|   0.009415s 	|0.010056s	| 0.002080s 	| 0.007297s		|
|1000000000   	| 3.3333333318  |   4.445919s 	|3.899332s	| 1.677298s 	| 1.144738s		|
|10000000000 	| 3.3333333332	|   44.365540s 	|42.499882s	| 16.397504s 	| 9.435163s		|
|100000000000	| 3.3333333333	|   443.553242s |362.453346s| 167.922970s 	| 107.009755s	|
