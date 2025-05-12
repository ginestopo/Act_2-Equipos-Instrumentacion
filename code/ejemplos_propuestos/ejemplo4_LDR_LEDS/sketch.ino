// The 74HC595 uses a type of serial connection called SPI (Serial Peripheral Interface) that requires three pins:
int datapin = 2; 
int clockpin = 3;
int latchpin = 4;

// We'll also declare a global variable for the data we're sending to the shift register:
byte data = 0;

// LDR function & pins
int getLux();
int LDR_PIN = A0;

// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;

void setup()
{
  // Set the three SPI pins to be outputs:
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);

  // LDR setup
  pinMode(LDR_PIN, INPUT);

  Serial.begin(9600);
} 

void loop()
{
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

  delay(200); // avoid reading too fast
}

int getLux() {
  int analogValue = analogRead(LDR_PIN); 
  float voltage = analogValue / 1024.0 * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  return (int)lux;
}

void shiftWrite(int desiredPin, boolean desiredState){
  bitWrite(data, desiredPin, desiredState);
  shiftOut(datapin, clockpin, MSBFIRST, data);
  digitalWrite(latchpin, HIGH); 
  digitalWrite(latchpin, LOW); 
}
