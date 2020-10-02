
# Dining Philosophers problem
### Estudiantes:

 - Abigail Gamboa Villalobos. B93144
 - Dominic Tarassenko. B97790

#### Descripción del problema:

Hay una mesa con 5 platos, 5 tenedores y una taza de espagueti. Se sientan 5 filósofos en la mesa y siguen el siguiente código:  
~~~
    while True:
    think ()
    get_forks ()
    eat()
    put_forks ()
~~~
Un filósofo necesita dos tenedores para comer, el que está a su izquierda y el que está a su derecha. Los filósofos están numerados de 0 a 4, y los tenedores también. El filósofo i tiene el tenedor i a su izquierda y el tenedor (i+1)%5 a su derecha.

El problema pide escribir una versión de get_forks y de put_forks que cumpla lo siguiente:

 - Solo un filósofo puede sostener un tenedor al mismo tiempo
    
-   Debe ser imposible que ocurra un deadlock
    
-   Debe ser imposible que haya starvation mientras un filósofo espera un tenedor
    
-   Debe ser posible que más de un filósofo coma al mismo tiempo
   
   
#### ¿Se puede presentar alguna situación de deadlock o starvation?

Para el problema de los dining philosophers es posible enfrentarse a una situación de deadlock si se diera el caso en que cada uno de los cinco filósofos tome un único tenedor de la mesa y, consecuentemente, todos se quedarían esperando infinitamente por un segundo tenedor para poder comer.

Este problema se puede solucionar mediante la utilización de un semáforo inicializado en 4, que funcione como “policía de la mesa”, es decir, se encargará de asegurarse que máximo haya cuatro filósofos sentados en ella.

Respecto al problema de starvation, es muy poco probable o, incluso imposible que se de, ya que solamente 2 filósofos estarán esperando por un mismo tenedor, sin embargo uno de ellos, cuando termine de comer, irá a pensar por un tiempo ‘x’, lo que dará tiempo al otro filósofo de tomar el tenedor.

## Pseudocódigo
~~~
struct table_t{  
	fork_count  
	mutex * fork
	semaphore footman  
}
  
struct philosopher_t{  
	id  
	table_t * table  
} 
  
think():  
	sleep  
  
eat():   
	sleep  
  
void get_forks(size_t i, table_t* table):  
	wait footman  
	lock table.fork[i]  
	lock table.fork[(i+1)%5)
  
void put_forks(size_t i, table_t* table):  
	unlock table.fork[i]
	unlock table.fork[(i+1)%5
	post footman 
  
void* dine(void* args):  
	while true  
	think()  
	get_forks(i,table)  
	eat()  
	put_forks(i,table) 
	 
main():  
	philosopher_count = 5  
	table_t table
	table.fork_count = 5
	initialize fork[]  //mutexes
	initialize footman = 4  //semaphore
	initialize philosopher[]  // Hilos
	initialize philosopherInfo[]  // Informacion de cada hilo
	for i < philosopher_count  
		pthread_mutex_init(table.fork[i], NULL)
		philosopherInfo[i].table = &table;
		pthread_create(&philosopher[i], NULL, dine, philosopherInfo[i])
  
	for i < philosopher_count  
		pthread_join  

~~~
