# CI0117 - Actividad de clase 4

### Instrucciones generales

Las soluciones al ejercicio de esta actividad deberán ser subidas a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
CI0117-2020-S2/acividades/[número de actividad]/[nombre de ejercicio]/
```
Agregue un archivo *Makefile* con los comandos necesarios para compilar el codigo fuente, de forma que el ejecutable tenga el mismo nombre dado en el ejercicio. Cree un archivo .gitignore para evitar subir archivos ejecutables.

## Ejercicio 1 [running_track_obstacles]

Modifique el código de la carrera de hilos con interfaz implementada en clase ([running_track_ui](https://github.com/jocan3/CI9117-2020-S2/tree/master/Ejemplos%20de%20clase/07_Pthreads/running_track_ui)) de forma que los corredores tengan que pasar 5 obstáculos antes de llegar a la meta.
* Agregue a la pista de carreras una matriz de enteros de tamaño 8x5 (número de carriles x número de obstáculos) inicializada en 0s.
* Cuando inicia la carrera cada corredor debe recorrer los 5 obstáculos, de forma que el tiempo requerido para llegar a cada obstáculo es igual a *running_time*/5. Por ej. si el valor del *running_time* de un corredor es de 10 segundos, significa que tarda 2 segundos para llegar a cada obstáculo.
* Al llegar a un obstáculo, el corredor debe colocar un 1 en la entrada correspondiente de la matriz. El corredor debe asegurarse de volver a poner en 0 dicha entrada al avanzar.

Modifique la interfaz para que muestre el trayecto de cada corredor durante la carrera.
* Descargue un archivo de imagen que represente a un corredor y agréguelo al folder de su solución.
* Cree una matriz de *widgets* de la biblioteca GTK+ e inicialícelos como *widgets* de tipo [GTKImage](https://developer.gnome.org/gtk3/stable/GtkImage.html).
* Modifique el método *draw_running_track* para que muestre la imagen del corredor en cada punto de la carrera: Si la entrada de la matriz de obstáculos es 1, se dibuja el corredor, de lo contrario se limpia la imagen. Puede basarse en el siguiente código:

```c
gtk_image_set_from_file(GTK_IMAGE(images[i][j]), "view/runner.png"); // "Dibuja" la imagen

gtk_image_set_from_pixbuf(GTK_IMAGE(images[i][j]), 
  gdk_pixbuf_scale_simple(gtk_image_get_pixbuf(GTK_IMAGE(images[i][j])), 50, 50, GDK_INTERP_NEAREST)); // Ajusta el tamaño de la imagen a 50x50 pixeles
...
gtk_image_clear(GTK_IMAGE(images[i][j])); // Limpia la imagen
```

Agregue un mecanismo de sincronización que permita controlar el acceso a las estructuras de datos de forma segura, de modo que no existan condiciones de carrera entre la interfaz y el método principal del programa.

