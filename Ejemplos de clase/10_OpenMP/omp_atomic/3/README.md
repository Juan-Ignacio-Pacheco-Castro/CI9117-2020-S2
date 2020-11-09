# Actividad 6: omp_atomic

Las directivas omp_atomic y omp_critical se usan para evitar que multiples hilos tengan acceso simultaneamente a una seccion del programa. En general, el resultado de utilizar cualquiera de las dos va a ser el mismo. Sin embargo lo que sucede internamente es diferente. Las siguientes dos implementaciones deberian tener el mismo resultado:

	#pragma omp_atomic
	count++;

	#pragma omp_critical
	count++;


La principal diferencia para el programador, es que atomic solo permite serializar una sola instruccion. Por otro lado, critical permite serializar una seccion arbitraria de codigo. Atomic es significativamente mas eficiente.

