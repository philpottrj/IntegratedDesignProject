const int photoLED_pin = 13;  //photosensing LED connected to pin 13
const int analogZero = 0;  //A0

void setup() {  
  pinMode (photoLED_pin, OUTPUT); //set the photosensing LED pin as on output
  Serial.begin(9600);
}

void loop() {
  nightLight(analogZero, photoLED_pin);
}
/*
 * Function Name:       nightLight
 * Function Parameters: The analog pin number that measures the voltage across the photosensor.
 *                      The pin number of an LED, as a constant integer.
 * Function Return:     Nothing.
 * Function Behavior:   Turns the LED on when the photosensor detects it is dark, and vice versa. 
 */
void nightLight(const int sensorPin, const int LED_pin) {
  const int threshold = 600;  //based on the voltage divider characteristics of the circuit
  int lightLevel= analogRead(sensorPin);
  //Serial.print("Light Level: ");
  //Serial.println(lightLevel);
  if (lightLevel < threshold){
    digitalWrite(LED_pin, HIGH); //if the light level is below the threshold level, the LED turns on
  }
  else{
    digitalWrite(LED_pin, LOW); //otherwise, if the light level is above the threshold level, the LED is off
  }
}
