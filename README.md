
# 🔵 Actividad 2 - Equipos e Instrumentación Electrónica

**NOTA: Todo el código del repositorio estará comentado en inglés para evitar caracteres como ñ y así errores de compilación.**

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
                    |- /ejemplo_1/
                    |
                    |- /ejemplo_2/
                    |
                    .
                    .
                    .
```

Finalmente se incorporarán todos los elementos en el proyecto final, que incluye los siguientes archivos:

```bash
  code
    |
    |- /ejemplos_propuestos/
    |              |
    |              |- /ejemplo_1/
    |              |- /ejemplo_2/ 
    |              |- ... 
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

A continuación, se desarrollarán los ejemplos de circuitos simples propuestos en la actividad que servirán para llevar a cabo nuestro objetivo: Realizar un control sobre los actuadores (actuadores resistivos para calentar las baterías durante la noche y control de válvulas para líquidos refrigerantes durante el día) para mantener las baterías de la boya meteorológica en buen estado.


## Desarrollo de ejemplo : Aplicación Servomotor 

El primer ejemplo interesante a implementar es el servo. Como en wokwi no disponemos de otro motor (además del stepper, que veremos más adelante), implementaremos el servo que puede ser usado para controlar una válvula que permita o impida el paos de líquido refrigerante para enfriar el sistema de baterías.

Para implementarlo el control del servo, en este ejemplo hago uso de la librería [Servo.h](https://www.arduinolibraries.info/libraries/servo). 

Este ejemplo consiste simplemente en conseguir movimiento por parte del servo. En el ejemplo final este movimiento será gobernado por una acción de control dependiente de la temperatura para el correcto funcionamiento de las baterías. 

A continuación, se observa el ejemplo de servo desarrollado. En este ejemplo el servo se mueve en ambas direcciones 180 grados.

![foto_servo](images/ejemplo1_servo.png)

## Desarrollo de ejemplo : Aplicación motor paso a paso

En este ejemplo se implementará el código y conexionado necesario para implementar un motor paso a paso en Arduino. Este motor tiene un comportamiento al anterior. Este motor se moverá tantas posiciones como pulsos se emitan. Por tanto, puede ser una forma alternativa de actuador sobre nuestro sistema de flujo refrigerante. La elección del motor paso a paso o servomotor para la aplicación de la boya meteorológica se justificará al final de este Readme.

Para usar el motor paso a paso necesitamos un integrado adicional, conocido como driver chip, que puede proporcionar grandes cantidades de corriente a la bobina del motor. Wokwi soporta el driver A4988, por lo que es el que usaremos en esta actividad. Realmente podríamos conectar directamente el motor a Arduino, puesto que el motor de Wokwi no tiene en cuenta simulación de corriente. Pero implmentaré el driver para darle mayor realismo. Además, con el driver, solo necesitamos usar un pin del Arduino. En caso contrario ocuparíamos 4 pines que pueden ser necesario para otros módulos. En este ejemplo, vemos que por cada pulso, el motor se mueve 1.8 grados (necesarios 200 pasos para una revolución completa).

![foto_stepper](images/ejemplo1_servo.png)


## Desarrollo de ejemplo : Control de leds con 74HC595

En este ejemplo vamos a aprender a controlar una serie de salidas (en este caso leds) a partir de una comunicación serie, en este caso SPI (Serial Peripheral Interface). A partir de tres pines (data, clock, latch) podemos enviar bytes de instrucción por serie al integrado para desempeñar distintas funciones. Esto tiene la principal ventaja de que se reduce considerablemente el número de puertos usados en el Arduino. 

En este ejemplo se implementa el integrado 74HC595 para poder encender y apagar secuencialmente los leds. Este ejemplo resultará útil para implementar en nuestro proyecto de la boya meteorológica, puesto que puede servir para mostrar los niveles de intensidad lumínica.

A continuación se muestra la implementación en Wokwi.

![foto_74HC595](images/ejemplo3_74HC595.png)


## Desarrollo de ejemplo : Control de leds con 74HC595 a partir de LDR

En este ejemplo usaremos la salida del LDR para poder hacernos una idea de la intensidad lumínica a partir de un array de LEDS (desarrollado en el ejemplo anterior).
En este ejemplo se realiza un mapeo de los posibles valores de salida del fotoresistor para mostrar un número de leds encendido en función de la intensidad de luz recibida. El mapeo puede entenderse como una regla de tres que, nos genera un número acotado en un rango deseado, dado un valor en un rango inicial. Se puede leer más sobre la función map aquí [map()](https://docs.arduino.cc/language-reference/en/functions/math/map/).

El código clave que nos permite mapear el valor de salida del LDR en su equivalente en LEDs viene dado por el siguiente código.


```C
    int luxValue = getLux();

    // constrain the value before mapping
    int constrainedLux = constrain(luxValue, 0, 10000); 

    // map to 0-8 because we have 8 leds
    int ledsToTurnOn = map(constrainedLux, 0, 10000, 0, 9);

    // avois getting out of range
    ledsToTurnOn = constrain(ledsToTurnOn, 0, 8); 

    // Turn on the necessary LEDs
    for (int i = 0; i < ledsToTurnOn; i++) {
        shiftWrite(i, HIGH);
    }

    // Turn off the rest
    for (int i = ledsToTurnOn; i < 8; i++) {
        shiftWrite(i, LOW);
    }

```

![foto_74HC595](images/ejemplo4_74HC595_LEDS.png)
## Made with ❤️ by 

- [@ginestopo](https://github.com/ginestopo) (Ginés Díaz Chamorro)


## License

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
