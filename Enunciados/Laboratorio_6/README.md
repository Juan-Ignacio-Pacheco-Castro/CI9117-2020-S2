# CI0117 - Laboratorio 6

### Instrucciones generales

**Fecha de entrega:** Viernes 30 de Octubre 11:50pm.

Este laboratorio se podrá realizar en grupos de máximo 2 personas.
Las soluciones a los ejercicios de este laboratorio deberán ser subidas a su repositorio *CI0117-2020-S2* creado para este curso en Github. Dentro de dicho repositorio se deberá crear un folder con el nombre *laboratorios*. La solución a cada laboratorio deberá estar contenida dentro de un folder con el respectivo número dentro del folder *laboratorios*. A su vez, la solución al ejercicio deberá subirse en un folder con el nombre indicado entre paréntesis cuadrados. 
Agregue un archivo *Makefile* para cada uno de los ejercicios con los comandos necesarios para compilar el codigo fuente, de forma que el ejecutable tenga el mismo nombre dado en el ejercicio.
Asegúrese de subir al repositorio únicamente archivos .c, .md y Makefile. Cree un archivo .gitignore para evitar subir archivos ejecutables.

Únicamente uno de los integrantes del grupo requiere subir la solución. Indique en el archivo README.md el nombre de ambos integrantes.

## Ejercicio 1 [riemann_openmp]

Modifique su solución al ejercicio [riemann_serial] del laboratorio 5, de forma que reciba un argumento adicional que indica la cantidad de threads en la línea de comandos.
Modifique la lógica que calcula el área bajo la curva con directivas de OpenMP para que sea ejecutada por la cantidad especificada de hilos. El cálculo de la aproximación debe realizarse de forma coordinada entre los hilos creados por OpenMP.

Haga uso de las siguientes directivas y cláusulas en su solución:

```
for
num_threads
default(none)
reduction
```

Investigue sobre dichas directivas haciendo uso de la [especificación y manual de referencia de OpenMP](https://www.openmp.org/specifications/).

Ejecute el programa con diferentes valores de *n* (*n = 1*, *n = 1,000* y *n = 1,000,000*, ..., *n = 1,000,000,000*) y utilizando valores fijos de *a* y *b* en cada ejecución. Realice ejecuciones usando 1, 16, 32, 64 y 128 hilos.
Apunte la duración de cada ejecución y asegúrese de obtener las mismas áreas estimadas en cada prueba. Agregue los datos de la comparación al archivo .md de este ejercicio, por ej:
| n | area | 1 | 16  | 32 | 64 | 128 |
|---|---|---|---|---|---|---|
| 1  |   |   |   |   |   |   |
| 10  |   |   |   |   |   |   |
| 1000  |   |   |   |   |   |   |
| ...  |   |   |   |   |   |   |

### Material de apoyo:

* Aproximación de Riemann simple usando rectángulos - https://youtu.be/W1IPtvVWCr8
* Manual de referencia OpenMP: https://www.openmp.org/resources/refguides/ 
