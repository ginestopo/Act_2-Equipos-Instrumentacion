#include <SimpleDHT.h>
#include <LiquidCrystal_I2C.h>
#define SENSORHT 7
#define LDR_PIN 2

SimpleDHT22 sensorht;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;

//Custom function declarations
void printLCD(String name, int number = -1, int delayMS = 1000, String text = "none");

void setup() {
  //------- DHT22 setup -------
  pinMode(SENSORHT, INPUT);
  Serial.println("DHT22 started!");

  //------- LCD setup ----------
  lcd.init(); // initialize the lcd 

  //------- LDR setup -------
  pinMode(LDR_PIN, INPUT);

  //built-in led
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  //------- DHT22 data reading ----------
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if(sensorht.read(SENSORHT, &temperature, &humidity, data)){
    Serial.println("There was an error reading DHT22!");
  }
  //------- DHT22 data reading END -------

  //------- LCD printing ----------
  printLCD("Temperature (*C):",(int)temperature,2000);
  printLCD("Humidity (%):",(int)humidity,2000);
  printLCD("Luxes:", getLux(),2000);
  printLCD("Air quality: ", -1, 2000,  getAirQuality());
  printLCD("Wind Direction: ", -1, 2000,  getWindDirection());
  //------- LCD printing END -------

  //day/night indicator (led ON when night)
  (getLux() > 50) ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);

}


/*
###############################
      My Custom Functions
###############################
*/

//this function will print in the LCD display the data with its name
void printLCD(String name, int number = -1, int delayMS = 1000, String text = "none"){
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print(name);   
  lcd.setCursor(2, 1);
  //Maybe some values are string, so only print number if it is not the default value 
  if(number != -1)        
    lcd.print(number);
  else
    lcd.print(text);
  delay(delayMS);
}

//this function makes the conversion from the analog value to luxes
int getLux(){
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  return lux;
}

//this function emulates the result of using an air quality sensor
String getAirQuality(){
  int analogValue = analogRead(A1);
  if(analogValue > 515)
    return "Good!";
  else 
    return "Bad :(";
}

//emulate wind direction by using a joystick
String getWindDirection(){
  int y = analogRead(A2); //vertical
  int x = analogRead(A3); //horizontal
  const int DEADZONE = 100;

  int centerX = 512;
  int centerY = 512;

  if (abs(x - centerX) < DEADZONE && abs(y - centerY) < DEADZONE) {
    return "no wind";
  }

  if (abs(x - centerX) > abs(y - centerY)) {
    if (x > centerX + DEADZONE) return "Left";
    if (x < centerX - DEADZONE) return "Right";
  } else {
    if (y > centerY + DEADZONE) return "Up";
    if (y < centerY - DEADZONE) return "Down";
  }

  return "wind error";
}