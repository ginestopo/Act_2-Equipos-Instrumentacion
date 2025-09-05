
# 🔵 Design of a Meteorological Buoy

**NOTES:**

* **The final project can be tested at [this link](https://wokwi.com/projects/431134122051145729), although its development is justified at the end of this readme.**

In this project, control actions are implemented on the actuators of the meteorological buoy based on the measured environmental parameters. Specifically, resistive actuators are used, which—by analyzing key indicators such as temperature, humidity, light, air quality, and wind—determine the actions needed to maintain the battery system within the target parameters (25 °C and 80% humidity).

![foto\_boya](images/boya.jpg)

## Project Structure and Introduction

First, we will develop the examples proposed in the activity. These examples are located at the following path:

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

Finally, all elements will be incorporated into the final project, which includes the following files:

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

Before proceeding, I updated the previous project for the following reason. In my previous project, I used **delay()** to allow the user to read information on the screen. This instruction is **blocking**, so it is impossible to perform continuous and real-time control of the variables we are going to implement in this activity. This update can be seen in commit **08b91d1441e859ebd8cf3dd0dfaee7a83c15ee72** of the **main** branch, which allows us to start this activity. These changes use the **millis()** function as shown below:

```C
unsigned long currentMillis = millis();
.
.
.
if (currentMillis - previousMillisDisplay >= displayInterval || batteryAction != "NONE") {
    previousMillisDisplay = currentMillis;
    .
    .
    .
}
```

This instruction, placed in the project loop, is non-blocking. Every time millis() is executed, we obtain a timestamp to check if the desired time has passed. If it has, we enter the *if*, otherwise we do not. In this way, we avoid stopping the program at a single line and allow other actions to occur simultaneously.

Next, we will develop the simple circuit examples proposed in the activity, which will serve our goal: to control the actuators (resistive actuators to heat the batteries at night and valve control for cooling fluids during the day) to keep the meteorological buoy’s batteries in good condition.

## Example Development: Servo Motor Application

The first interesting example to implement is the servo. Since Wokwi does not provide other motors (apart from the stepper, which we will see later), we will implement the servo, which can be used to control a valve that allows or blocks the passage of cooling fluid to regulate the battery system.

To control the servo in this example, I use the [Servo.h](https://www.arduinolibraries.info/libraries/servo) library.

This example simply achieves movement of the servo. In the final example, this movement will be governed by a control action dependent on temperature for the correct operation of the batteries.

Below is the developed servo example. In this example, the servo moves 180 degrees in both directions.

![foto\_servo](images/ejemplo1_servo.png)

## Example Development: Stepper Motor Application

In this example, we implement the code and wiring necessary to operate a stepper motor with Arduino. This motor behaves differently from the previous one. The stepper moves a number of positions corresponding to the pulses sent. Therefore, it can be an alternative actuator for our cooling fluid system. The choice between stepper motor and servo for the meteorological buoy application will be justified at the end of this Readme.

To use the stepper motor, we need an additional IC, known as a driver chip, which can provide high currents to the motor coil. Wokwi supports the A4988 driver, which is what we will use in this activity. Technically, we could connect the motor directly to Arduino, since Wokwi does not simulate current. However, I implemented the driver for realism. Moreover, using the driver requires only one Arduino pin; otherwise, four pins would be occupied, which may be needed for other modules. In this example, each pulse moves the motor 1.8 degrees (requiring 200 steps for a full revolution).

![foto\_stepper](images/ejemplo2_stepper.png)

## Example Development: LED Control with 74HC595

In this example, we will learn to control a series of outputs (in this case LEDs) via serial communication, specifically SPI (Serial Peripheral Interface). Using three pins (data, clock, latch), we can send instruction bytes to the IC to perform different functions. This greatly reduces the number of Arduino pins used.

The 74HC595 IC is implemented to sequentially turn LEDs on and off. This example will be useful for our meteorological buoy project, as it can indicate light intensity levels.

Below is the Wokwi implementation.

![foto\_74HC595](images/ejemplo3_74HC595.png)

## Example Development: LED Control with 74HC595 Based on LDR

In this example, we use the LDR output to estimate light intensity using an array of LEDs (developed in the previous example). The LDR output is mapped to a number of LEDs lit according to the light intensity. Mapping works like a proportion to generate a value in a desired range from an initial range. More on the map function can be found here: [map()](https://docs.arduino.cc/language-reference/en/functions/math/map/).

The key code mapping the LDR output to LEDs is:

```C
    int luxValue = getLux();

    // constrain the value before mapping
    int constrainedLux = constrain(luxValue, 0, 10000); 

    // map to 0-8 because we have 8 leds
    int ledsToTurnOn = map(constrainedLux, 0, 10000, 0, 9);

    // avoid going out of range
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

![foto\_74HC595](images/ejemplo4_74HC595_LEDS.png)

## Development of Complete Functionality

Once the examples are completed, we must determine which are useful to implement the desired functionality. Our goal is to maintain the buoy batteries at a constant temperature. This requires sensing ambient temperature (already implemented in Activity 1) and acting on the batteries based on it.

Thus, we design the following battery temperature regulation system:

* When batteries exceed 50°C, a cooling fluid circulates through a series of conduits. A servo motor is chosen because it provides more force at high speed than a stepper. Thus, example\_1 of the stepper will be added to Activity 1. Below, the servo pushes the cooling fluid through the battery system.

![gear\_pump](images/gear_pump.png)

* When batteries fall below 5°C, a set of heating resistors warms the system. This is modeled using an LED with a pull-down resistor.

For this control, a hysteresis system is necessary to prevent the actuator from being too repetitive in maintaining the target temperature.

The following hysteresis diagram has been developed:

```C
/* Hysteresis diagram --> Level 0: Heat battery, Level 1: Do nothing, Level 2: Cool Battery
 *
 *
 *   level
 *   ^
 *   |
 * 4_|
 *   |
 *   |
 *   |
 * 3_|
 *   |
 *   |
 *   | 
 * 2_|. . . . . . . . __________
 *   |                |  |      
 *   |                v  ^      
 *   |                |  |      
 * 1_|. . .___________|__|            
 *   |     |  |       .  .      
 *   |     v  ^       .  .      
 *   |     |  |       .  .      
 * 0_|_____|__|______________________________________|  temperature in centigrades
 *         |  |       |  |      
 *         5  10     45  50    
 *
 */
```

This diagram indicates 3 levels and 4 thresholds:

* **2** **-->** Batteries need cooling (in code: "COOL").
* **1** **-->** Batteries are within an acceptable temperature range (in code: "NONE").
* **0** **-->** Batteries need heating (in code: "HEAT").

Once ambient temperature is set, when actuators (cooling fluid or heating resistors) act, battery temperature does not change instantly. It decreases slowly (1°C per 500 ms, via *deltaTemperatureInterval*) until reaching 45°C. Then, when no longer acted upon, it rises back toward ambient temperature. Once it reaches 50°C again, cooling resumes. In this process, the servo acts as the fluid access valve, and similarly for heating at low temperatures.

The hysteresis behavior is inspired by code from [this repo](https://github.com/lille-boy/hysteresis/blob/master/hysteresis.c) but adapted to our case, using 2 levels instead of 4, with different thresholds:

```C
struct threshold {
    unsigned int low;
    unsigned int high;
    unsigned int level;
};

// number of thresholds
const int NB_THRESHOLDS = 2;

// definition of the thresholds
const struct threshold thresholds[NB_THRESHOLDS] = {
    {5, 10, 1},   // low, high, level
    {45, 50, 2}
};

// Hysteresis function (inspiration from https://github.com/lille-boy/hysteresis/blob/master/hysteresis.c)
unsigned int hysteresis(unsigned int input_temp) {
    static unsigned int current_level = 0;
    static unsigned int prev_temp = 0;

    if (input_temp >= prev_temp) {
        // Ascending - use high threshold
        for (int i = 0; i < NB_THRESHOLDS; i++) {
            if (input_temp >= thresholds[i].high && thresholds[i].level > current_level) {
                current_level = thresholds[i].level;
            }
        }
    } else {
        // Descending - use low threshold
        for (int i = NB_THRESHOLDS - 1; i >= 0; i--) {
            if (current_level == thresholds[i].level && input_temp <= thresholds[i].low) {
                current_level = thresholds[i].level - 1;
                break;
            }
        }
    }

    prev_temp = input_temp;
    return current_level;
}
```

Once the hysteresis logic is developed, it can be applied non-blocking in our *loop()* as follows, managing real-time battery system actuation. Battery temperature follows ambient temperature (1°C per 500 ms) until reached. Temperature changes if actuated by cooling fluid or heating resistors:

```C
  // This part of the code is in charge of managing the actions of the hysteresis
  if(currentMillis - previousMillisTemperature >= deltaTemperatureInterval){
    previousMillisTemperature = currentMillis;

    if(currentBatteryTemp != (int)lastTemperature && abs(currentBatteryTemp - (int)lastTemperature)  > 0.5){
      unsigned int currentLevel = hysteresis((int)currentBatteryTemp);

      if(currentLevel == 1){ // the battery does not need action
        batteryAction = "NONE";
        myservo.write(-90);
        digitalWrite(HEAT_RESISTOR, LOW);
        if(currentBatteryTemp - (int)lastTemperature < 0){
        currentBatteryTemp += 1;
        }else{
          currentBatteryTemp -= 1;
        }
      }

      if(currentLevel == 2){ // the battery needs cooling
        batteryAction = "COOL";
        myservo.write(90);
        currentBatteryTemp -= 1;
      }

      if(currentLevel == 0){ // the battery needs heat
        batteryAction = "HEAT";
        digitalWrite(HEAT_RESISTOR, HIGH);
        currentBatteryTemp += 1;
      }
      
      // emulating a clear log to read better
      clearSerialMonitor();

      Serial.println("----------------------------------------------------------");
      Serial.println("Ambient Temperature: " + String((int)lastTemperature));
      Serial.println("Battery Current Temperature: " + String(currentBatteryTemp));
      Serial.println("Current Action: " + batteryAction);
      Serial.println("----------------------------------------------------------");

    }

  }
```

Additionally, the final solution implements the LED driver example for light intensity. Other sensors function as in Activity 1 but without using *delay()*, which is blocking.

The complete implementation can be tested at [this link](https://wokwi.com/projects/431134122051145729):

![foto\_74HC595](images/implementacion_final.png)

## Made with ❤️ by

* [@ginestopo](https://github.com/ginestopo) (Ginés Díaz Chamorro)

## License

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
