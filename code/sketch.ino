#include <SimpleDHT.h>
#include <LiquidCrystal_I2C.h>
#define SENSORHT 7
#define LDR_PIN 2

SimpleDHT22 sensorht;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;

// Function prototypes 
void printLCD(String name, int number = -1, int delayMS = 0, String text = "none");
void updateSensorData();
int getLux();
String getAirQuality();
String getWindDirection();

// Variables to avoid blocking the system
unsigned long previousMillis = 0;
const long displayInterval = 2000; // Screen change interval (in milliseconds)
int currentDisplay = 0;
byte lastTemperature = 0;
byte lastHumidity = 0;
int lastLux = 0;
String lastAirQuality = "";
String lastWindDirection = "";

void setup() {
  //------- DHT22 setup -------
  pinMode(SENSORHT, INPUT);
  Serial.println("DHT22 started!");

  //------- LCD setup ----------
  lcd.init(); // initialize the lcd 
  lcd.backlight();

  //------- LDR setup -------
  pinMode(LDR_PIN, INPUT);

  //built-in led
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Update sensors continuously
  updateSensorData();
  
  // Update the display in a non-blocking way
  if (currentMillis - previousMillis >= displayInterval) {
    previousMillis = currentMillis;
    
    switch(currentDisplay) {
      case 0:
        printLCD("Temperature (*C):", (int)lastTemperature);
        break;
      case 1:
        printLCD("Humidity (%):", (int)lastHumidity);
        break;
      case 2:
        printLCD("Luxes:", lastLux);
        break;
      case 3:
        printLCD("Air quality: ", -1, 0, lastAirQuality);
        break;
      case 4:
        printLCD("Wind Direction: ", -1, 0, lastWindDirection);
        break;
    }
    
    currentDisplay = (currentDisplay + 1) % 5; // Show the next param in the screen
  }

  //day/night indicator (led ON when night)
  (lastLux > 50) ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);
}

void updateSensorData() {
  //------- DHT22 data reading ----------
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if(!sensorht.read(SENSORHT, &temperature, &humidity, data)) {
    lastTemperature = temperature;
    lastHumidity = humidity;
  }
  
  // Update other sensors
  lastLux = getLux();
  lastAirQuality = getAirQuality();
  lastWindDirection = getWindDirection();
}

// printLCD without delay
void printLCD(String name, int number = -1, int delayMS = 0, String text = "none"){
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print(name);   
  lcd.setCursor(2, 1);
  if(number != -1)        
    lcd.print(number);
  else
    lcd.print(text);
}

int getLux(){
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  return lux;
}

String getAirQuality(){
  int analogValue = analogRead(A1);
  if(analogValue > 515)
    return "Good!";
  else 
    return "Bad :(";
}

String getWindDirection(){
  int y = analogRead(A2);
  int x = analogRead(A3);
  const int DEADZONE = 100;
  int centerX = 512;
  int centerY = 512;

  if (abs(x - centerX) < DEADZONE && abs(y - centerY) < DEADZONE) {
    return "no wind";
  }

  if (abs(x - centerX) > abs(y - centerY)) {
    if (x > centerX + DEADZONE) return "West";
    if (x < centerX - DEADZONE) return "East";
  } else {
    if (y > centerY + DEADZONE) return "North";
    if (y < centerY - DEADZONE) return "South";
  }

  return "wind error";
}