const int intruderLED_pin = 13;  //intruder alert LED is connected to pin 13
const int analogOne = 1;  //analog input pin one

void setup() {  
  pinMode (intruderLED_pin, OUTPUT); //tells the computer the pin corresponding to the intruder alert LED is an output
  Serial.begin(9600);
}

void loop() {
  intruderAlertLight(intruderLED_pin, analogOne);  //flash intruder alert LED when intruder alert button is triggered
}
/*
 * Function Name:       intuderAlertLight
 * Function Parameters: The pin number of the LED used to simulate the intruder alert, as a constant integer. 
 *                      The pin number of the analog sensor pin that measures the intruder alert activation button voltage, as a constant integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Flashes the input LED when the button corresponding to the input sensor pin is triggered. When the button is un-triggered is is off.
 */
void intruderAlertLight(const int ledPin, const int sensorPin) {
  static bool intruderAlertToggled = false;
  static bool intruderModeActivated = false;
  static int buttonLevel = LOW;
  buttonLevel = analogRead(analogOne);  //reads the voltage level of the button from the analog one sensor pin
  intruderModeActivated = isButtonPressed(buttonLevel);  //when the button is pressed, intruder mode is activated, and vice versa
  intruderAlertToggled = updateButtonToggle(intruderModeActivated, intruderAlertToggled);  //the intruder alert is toggled when intruder mode is activated, and vice versa
  if (intruderAlertToggled) {  //when the intruder alert IS toggled
    flashLED(ledPin);  //flash the intruder alert LED  
  }
  else {  //when the intruder alert is NOT toggled
    digitalWrite(ledPin, LOW);  //turn the intruder alert LED off
  }
}
/*
 * Function Name:       updateButtonToggle
 * Function Parameters: The press state of a button, as a boolean.
 *                      The toggle state of the same button, as a boolean. 
 * Function Return:     Whether or not the input button is toggled, as a boolean. 
 * Function Behavior:   Determines/updates the toggle state of the button. This is dependent on the previous state and whether the button is pressed.
 */
bool updateButtonToggle(bool pressed, bool toggled) {
  static unsigned long lastTimeToggled;
  const int debouncingTime = 200;
  bool toggle = toggled;  //creates seperate toggle variable as to not overwrite the input variable
  unsigned long difference = millis() - lastTimeToggled;
  if (pressed) {  //if button is pressed
    if (toggled && (difference > debouncingTime)){  //if button is toggled and a certain delay time has passed
      toggle = false;  //untoggle the button
      lastTimeToggled = millis();  //update last time toggled to current time
    }
    else if (!toggled && (difference > debouncingTime)) {
      toggle = true;  //toggle the button
      lastTimeToggled = millis();  //update last time toggled to current time
    }
  }
  return toggle;
}
/*
 * Function Name:       flashLED
 * Function Parameters: The pin number of an LED, as a constant integer. 
 * Function Return:     Nothing.
 * Function Behavior:   Rapidly flashes the input LED, without delaying the program time. 
 */
void flashLED(const int LED_pin) {
  const int waitTime = 50;
  static unsigned long previousTime = 0;  //previous time the LED state was changed
  unsigned long currentTime = millis();  //saves current program time
  static int LED_state = LOW;
  if (currentTime - previousTime > waitTime) {  //when the program waits a certain amount of time
    previousTime = currentTime; 
    if (LED_state == LOW) {
      LED_state = HIGH;
    }
    else {
      LED_state = LOW;
    }
    digitalWrite(LED_pin, LED_state);  //change the state of the LED, aka turn it on or off
  }
}
/*
 * Function Name:       isButtonPressed
 * Function Parameters: The button voltage, how much voltage from the source is passed through the button, as an integer. 
 * Function Return:     The pressed state of the input button, as a boolean.
 * Function Behavior:   Determines whether the input button is pressed or not, based on the voltage therefore current passing through the device. 
 */
bool isButtonPressed(int buttonVoltage) {
  const int threshold = 1000;
  static bool pressed = false;
  if (buttonVoltage < threshold) {  //if voltage read is 0V
    pressed = false;
  }
  else if (buttonVoltage > threshold) {  //if voltage read is 5V
    pressed = true;
  }
  return pressed;
}
