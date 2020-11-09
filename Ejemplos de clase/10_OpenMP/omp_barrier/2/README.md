# Funcionamiento del barrier
### Por Fabiola Jimenez y Karen Zamora

Los barriers son herramientas de sincronizacion que limitan la ejecucion de una seccion paralela, haciendo que los hilos esperen a todos los demas antes de continuar la ejecucion del bloque paralelo. 

```
#pragma omp parallel 
   {

        #pragma omp barrier    /* Aqui los hilos esperan a que los demas lleguen   */

        printf("Hello we're here");
   }
```
