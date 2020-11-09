Katherine Gonzalez
Gianfranco Bagnarello

Las diferencias entre las directivas  atomic y critical radica en que, con critical se asegura que una ubicación de memoria específica se actualice automáticamente; esto evita la posibilidad de que varios subprocesos lean y escriban simultáneamente una ubicación de memoria específica.