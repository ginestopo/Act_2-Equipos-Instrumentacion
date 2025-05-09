
# 🔵 Actividad 2 - Equipos e Instrumentación Electrónica

En la presente actividad (actividad 2 de la asignatura), partiremos del proyecto desarrollado en la actividad 1 para, en base a los parámetros medidos en la boya meteorológica, ser capaces de aplicar una acción de control sobre actuadores situados en la misma. En este caso, actuadores resistivos, que a partir del conocimiento del estado del clima mediante la medición de sus indicadores fundamentales (temperatura, humedad, iluminación, calidad del aire, viento…) determinarán las acciones de control y actuación que garanticen que el sistema de baterías se mantenga alrededor de los parámetros deseados (25 grados de temperatura y humedad al 80%).





![foto_boya](images/boya.jpg)


## Estructura del proyecto e introducción

En primer lugar se llevará a cabo el desarrollo de los ejemplos propuestos en la actividad. Estos ejemplos se encuentran en la siguiente ruta:

```bash
  code
    |
    |
    ejemplos_propuestos
                    |
                    |-
                    |
                    |-
                    |
                    .
                    .
                    .
```

Finalmente se incorporarán todos los elementos en el proyecto final, que incluye los siguientes archivos:

```bash
  code
    |
    |
    |- diagram.json
    |
    |- libraries.txt
    |
    |- sketch.ino 
    |
    |- wokwi-project.txt

```


Antes de proceder he realizado una actualización del proyecto anterior por el siguiente motivo. En mi proyecto anterior hago uso de **delay()** para dar lugar al usuario a leer la información en pantalla. Resulta que esta instrucción es **bloqueante** por lo tanto resulta imposible hacer un control contínuo y en tiempo real de las variables de control y actuación que vamos a implementar en esta actividad. En el commit **08b91d1441e859ebd8cf3dd0dfaee7a83c15ee72** de la rama **main** puede observarse esta actualización, que nos permite empezar con la actividad.


## Made with ❤️ by 

- [@ginestopo](https://github.com/ginestopo)

