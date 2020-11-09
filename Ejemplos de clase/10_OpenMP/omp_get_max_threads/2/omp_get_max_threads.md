----------------------------------------------------------
Programacion Paralela					 
/Actividad 6							 
/UCR	 							 
- Esteban Quesada Quesada B96157    
- Steven Nuñez Murillo B95614		 
-----------------------------------------------------------

+ omp_get_max_threads:
	
	Esta rutina se encarga de devolver un valor entero el cual
	es igual o mayor que el número de subprocesos que estarían 
	disponibles si se definiera una nueva región paralela sin
	num_threads en ese punto del código. Este valor devuelto 
	por omp_get_max_threads es el valor del primer elemento del 
	ICV nthreads-var de la tarea actual.
	
	Cabe destacar que si se utiliza omp_set_num_threads para 
	cambiar el número de subprocesos, las llamadas posteriores 
	a omp_get_max_threads devolverán el nuevo valor.
	
	El valor de retorno de esta rutina se puede utilizar para 
	asignar suficiente almacenamiento de forma dinámica para 
	todos los subprocesos del equipo formado en la región paralela
	activa posterior.
	
	
	Extra:
	
	ICV nthreads-var : controla el número de subprocesos solicitados 
	para las regiones paralelas encontradas. Hay una copia de 
	este ICV por entorno de datos.

	
