# Colas con semaforos

Andrea Alvarado Acón B90272
Roberto Méndez López B94788


- Descripción del problema / patrón de sincronización.

El patrón de sincronización por lo general son estructuras de programación que tienden a aparecer como respuestas naturales.
Los semáforos pueden representar una cola. Se inicia en cero y cuando posee un valor negativo significa que hay threads esperando. El valor nunca es positivo.

Se emplean cuando se tienen dos clases de hilos que deben proceder de a pares. También es referido a un *baile de salón*, donde hay un líder y un seguidor.


- Conteste la pregunta: ¿Se puede presentar alguna situación de deadlock o starvation? Explique.

Es muy difícil que se presente una situación de deadlock, ya que por la manera en que está diseñado este patrón no ocurre que dos threads esperan simultáneamente a un recurso que posea el otro. Sin embargo, puede ocurrir starvation si por ejemplo hay un número impar de threads. En este caso uno se podría quedar esperando a una pareja, sin que esta nunca llegue.


- Agregue alguna ilustración o pseudocódigo que permita visualizar la solución al problema.

Para este patron decidimos ejemplificar dos implementaciones diferentes. Primero, la cola con semáforos común:

```
followerQueue.post ()
leaderQueue.wait()
dance ()


leaderQueue.post ()
followerQueue.wait()
dance ()

```
Segundo, la cola con semáforos exclusiva:

```
//LEADER
mutex.wait()
if  followers  > 0:
    followers --
    followerQueue.signal ()
else:
    leaders ++
    mutex.signal ()
    leaderQueue.wait()

dance ()
rendezvous.wait()
mutex.signal ()


//FOLLOWER
mutex.wait()
if  leaders  > 0:
    leaders --
    leaderQueue.signal ()
else:
    followers ++
    mutex.signal ()
    followerQueue.wait()

dance ()
rendezvous.signal ()


```