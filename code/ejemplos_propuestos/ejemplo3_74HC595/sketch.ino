// The 74HC595 uses a type of serial connection called SPI (Serial Peripheral Interface) that requires three pins:
int datapin = 2; 
int clockpin = 3;
int latchpin = 4;

// We'll also declare a global variable for the data we're
// sending to the shift register:

byte data = 0;


void setup()
{
  // Set the three SPI pins to be outputs:

  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
}


void loop()
{
  oneAfterAnother();      // All on, all off

}


void shiftWrite(int desiredPin, boolean desiredState){

// This function lets you make the shift register outputs
// HIGH or LOW in exactly the same way that you use digitalWrite().

  bitWrite(data,desiredPin,desiredState); //Change desired bit to 0 or 1 in "data"

  // Now we'll actually send that data to the shift register.
  // The shiftOut() function does all the hard work of
  // manipulating the data and clock pins to move the data
  // into the shift register:

  shiftOut(datapin, clockpin, MSBFIRST, data); //Send "data" to the shift register

  //Toggle the latchPin to make "data" appear at the outputs
  digitalWrite(latchpin, HIGH); 
  digitalWrite(latchpin, LOW); 
}


void oneAfterAnother()
{
// This function will turn on all the LEDs, one-by-one,
// and then turn them off all off, one-by-one. 

  int index;
  int delayTime = 100; // Time (milliseconds) to pause between LEDs
                       // Make this smaller for faster switching

  // Turn all the LEDs on
  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);
    delay(delayTime);                
  }

  // Turn all the LEDs off
  for(index = 7; index >= 0; index--)
  {
    shiftWrite(index, LOW);
    delay(delayTime);
  }
}