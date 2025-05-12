int PIN_STEPPER = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_STEPPER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_STEPPER, HIGH); // sets the digital pin 13 on

  delay(1000);            // waits for a second

  digitalWrite(PIN_STEPPER, LOW);  // sets the digital pin 13 off

  delay(1000);            // waits for a second
}
