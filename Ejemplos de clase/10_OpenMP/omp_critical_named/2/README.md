### ¿En qué consiste la opción de agregar un nombre a la directiva de exclusión mutua omp critical(name)?

Se puede agregar un nombre a la directiva critical para identificarlo.
Ahora cada vez que usemos ese nombre para identificar una zona critica ambos
van a estar sincronizados.

### ¿Cuál es la diferencia con respecto a utilizarla sin agregar dicho nombre?

Un hilo espera al comienzo de una región crítica identificada por un nombre determinado hasta que ningún otro hilo del programa esté ejecutando una región crítica con ese mismo nombre. Las secciones críticas no nombradas específicamente por la invocación de la directiva crítica omp se asignan al mismo nombre no especificado.
Texto tomado de *[este sitio](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/prag_omp_critical.htm)*

Osea la exclusión mutua no aplica a zonas criticas con nombre distintos.
