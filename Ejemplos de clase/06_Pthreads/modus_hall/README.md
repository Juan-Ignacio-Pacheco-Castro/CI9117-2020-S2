Laboratorio #4
Problema Asignado: Modus Hall
Steven Nuñez Murillo B95614
Esteban Quesada Quesada B96157

Modus Hall
En qué consiste: 
Existe un camino donde deben transitar varios hilos, los cuales pueden ser de distintos bandos o equipos. En el camino solo puede circular uno a la vez, mas si dos hilos del mismo bando se encuentran, uno se hará al lado para que se pueda acomodar uno de esos dos hilos y poder continuar. Sin embargo, ocurre una pelea si dos hilos de distintos bandos se encuentran en una determinada sección del camino. Si ocurre este hecho el bando con mayor hilos en el camino ganará la pelea y el hilo con menor número de compañeros tendrá que cederles el espacio.
Por lo tanto si lo enfocamos más al problema en la programación lo que ocurre es que el bando con mayor número de hilos con intención de acceder a una sección crítica ganará la posición en la sección crítica. La solución al problema de Modus Hall nos permite evitar el problema denominado Starvation.
Se evita el problema de Starvation puesto que mientras que una facción controla la sección crítica, los miembros de la otra facción se acumulan en una cola hasta que alcanzan ser la mayoría y desplazar así al bando enemigo con un menor número de hilos.
Por lo tanto se puede prohibir la entrada de nuevos enemigos de un bando, mientras se está trabajando en la sección crítica, y esperan el momento en que sean derrocados de la sección crítica.
La solución de este problema permite mover subprocesos en lotes, lo que permite la máxima concurrencia en la sección crítica.


Aspectos para el Patrón de Sincronización acompañado del Pseudocódigo:
Variables a utilizar para resolver el problema:
Heathens: Contador de hilos pertenecientes al bando Heathens.
Prudes: Contador de hilos pertenecientes al bando Prudes.
Status: Puede ser neutral, heathens rule(heathens mandan), prudes rule(prudes mandan), transition to heathens, transitions to prudes
Mutex: Se usa un mutex para proteger esta casilla.
Heathen Turn: Semaphore (1)
Prude Turn: Semaphore1(1)
Heathen Queue: Semaphore (0)
Prude Queue: Semaphore

HeathenTurn y PrudeTurn controlan el acceso al campo para que podamos prohibir un lado o el otro durante una transición. 
HeathenQueue y PrudeQueue son donde los hilos esperan después de registrarse y antes de salir al campo.


Starvation:
Starvation es el problema que ocurre cuando los procesos de alta prioridad siguen ejecutándose y los procesos de baja prioridad se bloquean por tiempo indefinido. En el problema del Modus Hall puede ocurrir debido a que un grupo de un bando de hilos puede bloquear indefinidamente a ciertos hilos que se pueden encontrar en espera, mas con la solución al problema podemos evitar este bloqueo mencionado, ya que mientras que una facción controla la sección crítica, los miembros de la otra facción se acumulan en una cola hasta que alcanzan ser la mayoría y desplazar así al bando enemigo con un menor número de hilos. Es decir, si existe un límite que no permite que determinados hilos controlen indefinidamente la sección crítica y no dejen que otros hilos ejecuten sus procesos.



Pseudocódigo de los Heathens, para los Prudes se debe seguir el mismo lineamiento.
![image](https://github.com/Estebanq3/-CI0117-2020-S2/blob/master/4/Pseudo.png)


A medida que cada hilo se registra se deben considerar los siguientes casos:
1.Si el campo está vacío, el hilo que llega de determinado bando reclama la sección crítica como territorio de su bando.
2.Si los heathens están actualmente a cargo, pero el recién llegado ha inclinado el equilibrio, se bloquea a los Prudes y el sistema cambia a modo de transición.
3. Si los Prudes están a cargo, pero el recién llegado no inclina la balanza, se une a la cola.
4.Si el sistema está pasando a un control de Heathens, el recién llegado se une a la cola.
5. De lo contrario, llegamos a la conclusión de que o los Heathens están a cargo o el sistema está pasando al control de los Prudes. En cualquier caso, este hilo puede continuar.

De manera similar, a medida que cada estudiante revisa, debe considerar varios casos:
Si el hilo es el último Heathen en irse, debe considerar lo siguiente:
1.Si el sistema está en transición, eso significa que el los Prudes están bloqueados, por lo que tiene que abrirles el paso, es decir desbloquearlos.
2.Si hay Prudes esperando, les indica y actualiza el estado para que los Prudes estén a cargo. De lo contrario, el nuevo estado es "neutral".
3.Si ella no es la última heathen en salir, todavía tiene que comprobar la posibilidad de que su partida incline la balanza. En ese caso, cierra a los heathens y comienza la transición.
