/*

  Title: Smart Home - Milestone 2
  Authors: Risa Philpott & Bernie Cieplak
  Created: February 27, 2020
  Last Modified: March 7, 2020 by Bernie Cieplak
  
*/

// LCD library
#include <LiquidCrystal.h>

// definitions
// analog pin definitions
#define INTRUDER_INPUT 0 // intruder alert analog button input
#define NIGHT_INPUT 1 // night light analog light sensor input
#define SWITCH_INPUT 2 // smart switch analog input
#define WEATHER_INPUT 3 // weather station temperature analog input

// digital pin definitions
#define INTRUDER_LED 13 // intruder alert LED
#define LCD_PIN6 12 // LCD pin 6
#define LCD_PIN5 11 // LCD pin 5
#define LCD_PIN4 10 // LCD pin 4
#define LCD_PIN3 9 // LCD pin 3
#define SWITCH_OUTPUT 8 // smart switch output
#define WEATHER_OUTPUT 7 // weather station appliance output
// PIN 6 NOT USED
#define LCD_PIN2 5 // LCD pin 2
#define LCD_PIN1 4 // LCD pin 1
#define INTRUDER_SOUND 3 // intruder speaker output
#define NIGHT_OUTPUT 2 // night light output LED pin

// global variables
bool LCD_ON = false;
int TEMP_THRESHOLD = 69; // temperature to turn on AC unit
int LIGHT_THRESHOLD = 600; // light level to turn on Night Light
LiquidCrystal lcd(LCD_PIN1,LCD_PIN2,LCD_PIN3,LCD_PIN4,LCD_PIN5,LCD_PIN6);
bool nightOn, switchOn, intruderOn, acOn = false;
int weatherTemp = 0;
String topText; // text that is put on the top row the LCD
String bottomText; // text that is put on the bottom row of the LCD
String serialText; // string that is output to the COM port

// setup function
void setup() {
  lcd.begin(16, 2); // set up the LCD's columns and rows
  pinMode (INTRUDER_LED, OUTPUT); // intruder LED pin is output
  pinMode (INTRUDER_SOUND, OUTPUT); // speaker pin is output
  pinMode (NIGHT_OUTPUT, OUTPUT); // night light LED pin is output
  pinMode (WEATHER_OUTPUT, OUTPUT); // ac unit pin is output
  pinMode (SWITCH_OUTPUT, OUTPUT); // smart switch pin is output
  Serial.begin(9600); // open serial port

  // startup LCD message
  printLCD("ECE 2804 Project","By Bernie & Risa");
  delay(5000);
  lcd.clear();
}

// main loop
void loop() {
  nightOn = nightLight(NIGHT_INPUT, NIGHT_OUTPUT); // create instance of night light function
  acOn = weatherStation(WEATHER_OUTPUT,WEATHER_INPUT);  // create instance of weather station
  switchOn = smartSwitch(SWITCH_OUTPUT,SWITCH_INPUT); // create instance of smart switch
  // Prints in the following order: NL,AC,SS,Temp,Intruder as array
  serialText = String(nightOn) + ',' + String(acOn) + ',' + String(switchOn) + ','
    + String(weatherTemp) + ',' + String(intruderOn);
  printSerial(serialText);
  weatherTemp = getTemp(WEATHER_INPUT); // get temperature from weather station
  intruderOn = intruderAlertLight(INTRUDER_LED, INTRUDER_INPUT);  // flash intruder alert LED when intruder alert button is triggered
  if(!intruderOn) {
    bottomText = "Temp: " + String(weatherTemp) + "\xDF" + "F";
    topText = "NL:" + String(nightOn) + " SS:" + String(switchOn) + " AC:" + String(acOn);
    printStatus(topText,bottomText);
  }
}

void printAlert() { 
  const int waitTime = 500;
  static unsigned long previousTime = 0;  // previous time the LCD state was changed
  unsigned long currentTime = millis();  // saves current program time
  printLCD("INTRUDER","ALERT");
  if (currentTime - previousTime > waitTime) {  //when the program waits a certain amount of time
    previousTime = currentTime; 
    if (LCD_ON) {
      LCD_ON = false;
      lcd.noDisplay();
    }
    else {
      LCD_ON = true;
      lcd.display();
    }
  }
}

void printStatus(String top, String bottom) {
  const int waitTime = 100;
  static unsigned long previousTime = 0;  // previous time the LCD state was changed
  unsigned long currentTime = millis();  // saves current program time
  if (currentTime - previousTime > waitTime) {  // when the program waits a certain amount of time
    previousTime = currentTime; 
    printLCD(top,bottom);
  }
}

void printSerial(String serial) {
  const int waitTime = 100;
  static unsigned long previousTime = 0;  // previous time the LCD state was changed
  unsigned long currentTime = millis();  // saves current program time
  if (currentTime - previousTime > waitTime) {  // when the program waits a certain amount of time
    previousTime = currentTime; 
    Serial.println(serial);
  }
}

void printLCD(String top, String bottom) {
    if (top.length() > 16) {
      lcd.setCursor(5,0);
      lcd.print("ERROR");
    }
    else {
      int startTop = (16 - top.length())/2;
      lcd.setCursor(startTop,0);
      lcd.print(top);
    }
    if (bottom.length() > 16) {
      lcd.setCursor(5,1);
      lcd.print("ERROR");
    }
    else {
      int startBottom = (16 - bottom.length())/2;
      lcd.setCursor(startBottom,1);
      lcd.print(bottom);
    }
}

/*
 * Function Name:       smartSwitch
 * Function Parameters: The pin number of the LED used to simulate the smart switch, as a constant integer. 
 *                      The pin number of the analog sensor pin that measures the button voltage, as a constant integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Turns on input LED when the button corresponding to the input sensor pin is triggered. When the button is un-triggered it is off.
 */
bool smartSwitch(const int ledPin, const int sensorPin) {
  static bool buttonToggled = false;
  static bool ledActivated = false;
  static int buttonLevel = LOW;
  buttonLevel = analogRead(SWITCH_INPUT);  // reads the voltage level of the button from the analog one sensor pin
  ledActivated = isButtonPressed(buttonLevel);  // when the button is pressed, LED is activated, and vice versa
  buttonToggled = updateButtonToggle(ledActivated, buttonToggled);  // the LED is toggled when intruder mode is activated, and vice versa

  // Raspberry Pi Trigger
  bool serialData = Serial.available();
  if(serialData){ // only trigger if data has been sent
    char inByte = Serial.read(); // read the incoming data
    if(inByte == 's') {
      ledActivated = true;  // when the button is pressed, LED is activated, and vice versa
      buttonToggled = updateButtonToggle(ledActivated, buttonToggled);  // the LED is toggled when intruder mode is activated, and vice versa
    }
  }

  if (buttonToggled) {  // when the intruder alert IS toggled
    digitalWrite(ledPin, HIGH); // turn on the smart switch
  }
  else {  // when the intruder alert is NOT toggled
    digitalWrite(ledPin, LOW); // turn off the smart switch
  }
  return buttonToggled;
}

/*
 * Function Name:       intruderAlertLight
 * Function Parameters: The pin number of the LED used to simulate the intruder alert, as a constant integer. 
 *                      The pin number of the analog sensor pin that measures the intruder alert activation button voltage, as a constant integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Flashes the input LED when the button corresponding to the input sensor pin is triggered. When the button is un-triggered is is off.
 */
bool intruderAlertLight(const int ledPin, const int sensorPin) {
  static bool intruderAlertToggled = false;
  static bool intruderModeActivated = false;
  static int buttonLevel = LOW;
  buttonLevel = analogRead(INTRUDER_INPUT);  // reads the voltage level of the button from the analog one sensor pin
  intruderModeActivated = isButtonPressed(buttonLevel);  // when the button is pressed, intruder mode is activated, and vice versa
  intruderAlertToggled = updateButtonToggle(intruderModeActivated, intruderAlertToggled);  // the intruder alert is toggled when intruder mode is activated, and vice versa
  char inByte = ' ';

  // Raspberry Pi Trigger
  bool isIntruder = false;
  bool serialData = Serial.available();
  if(serialData){ // only trigger if data has been sent
    char inByte = Serial.read(); // read the incoming data
    isIntruder = inByte == 'i';
    if(isIntruder) {
      intruderModeActivated = true;
      intruderAlertToggled = updateButtonToggle(intruderModeActivated, intruderAlertToggled);  // the intruder alert is toggled when intruder mode is activated, and vice versa
    }
  }

  if (intruderAlertToggled) {  // when the intruder alert IS toggled
    flashLED(ledPin);  // flash the intruder alert LED  
    printAlert();
    playSpeakerTone(INTRUDER_SOUND, 1523.25, 200);
    if (isButtonPressed(buttonLevel) || isIntruder) {
      lcd.clear();
      lcd.display();
    }
  }
  else {  // when the intruder alert is NOT toggled
    digitalWrite(ledPin, LOW);  // turn the intruder alert LED off
    LCD_ON = false; // Is the LCD on in the Intruder Blinking function?
    noTone(INTRUDER_SOUND);
    if (isButtonPressed(buttonLevel) || isIntruder) {
      lcd.clear();
      lcd.display();
    }
  }
  return intruderAlertToggled;
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
  bool toggle = toggled;  // creates separate toggle variable as to not overwrite the input variable
  unsigned long difference = millis() - lastTimeToggled;
  if (pressed) {  // if button is pressed
    if (toggled && (difference > debouncingTime)){  // if button is toggled and a certain delay time has passed
      toggle = false;  // untoggle the button
      lastTimeToggled = millis();  // update last time toggled to current time
    }
    else if (!toggled && (difference > debouncingTime)) {
      toggle = true;  // toggle the button
      lastTimeToggled = millis();  // update last time toggled to current time
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
  unsigned long currentTime = millis();  // saves current program time
  static unsigned long previousTime = 0;  // previous time the LED state was changed
  static int LED_state = LOW;
  if (currentTime - previousTime > waitTime) {  // when the program waits a certain amount of time
    previousTime = currentTime; 
    if (LED_state == LOW) {
      LED_state = HIGH;
    }
    else {
      LED_state = LOW;
    }
    digitalWrite(LED_pin, LED_state);  // change the state of the LED, aka turn it on or off
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
  if (buttonVoltage < threshold) {  // if voltage read is 0V
    pressed = false;
  }
  else if (buttonVoltage > threshold) {  // if voltage read is 5V
    pressed = true;
  }
  return pressed;
}

bool nightLight(const int sensorPin, const int LED_pin) {
  const int threshold = LIGHT_THRESHOLD;  // based on the voltage divider characteristics of the circuit
  int lightLevel= analogRead(sensorPin);
  if (lightLevel < threshold){
    digitalWrite(LED_pin, HIGH); // if the light level is below the threshold level, the LED turns on
  }
  else{
    digitalWrite(LED_pin, LOW); // otherwise, if the light level is above the threshold level, the LED is off
  }
  return (lightLevel < threshold);
}

/*
 * Function Name:       playSpeakerTone()
 * Function Parameters: The output pin number of that drives the speaker, as a constant integer. 
 *                      The desired frequency of the tone, in Hertz, as a float. 
 *                      The desired duration of the tone, in ms, as an integer. 
 *                      The desired number of times the tone will be played, as an integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Plays a desired tone to the desired output pin. 
 */
void playSpeakerTone(const int pinOut, float freq, int duration) {
  const int waitTime = duration;
  static unsigned long previousTime = 0;  // previous time the LED state was changed
  unsigned long currentTime = millis();  // saves current program time
  static bool toneOn = false;
  if (currentTime - previousTime > waitTime) {  // when the program waits a certain amount of time
    previousTime = currentTime; 
    if (!toneOn) {  // when tone is OFF aka not playing
      toneOn = true;  // turn tone ON
    }
    else {
      toneOn = false;  // turn tone OFF
    }
  }
  if (toneOn) {
    tone(pinOut, freq);
  }
  else {
    noTone(pinOut);
  }
}

/*
 * Function Name:       weatherStation()
 * Function Parameters: The output pin number that drives the simulation LED, as a constant integer. 
 *                      The input (analog sensor) pin number that drives the automatic air conditioner, as a constant integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Reads the input temperature data and turns on the automatic air conditioner (LED) when temperature is too high.
 */
bool weatherStation(const int outputPin, const int inputPin) {
  int temperature = getTemp(inputPin);
  const int threshold = TEMP_THRESHOLD;
  if (temperature > threshold){  // when the temperature is too high
    digitalWrite(outputPin, HIGH); // turn LED on
  }
  else{  // when the temperature is not too high
    digitalWrite(outputPin, LOW); // turn LED off
  }
  return temperature > threshold;
}

int getTemp(const int inputPin) {
  const int waitTime = 5000;
  static unsigned long previousTime = 0;  // previous time the LCD state was changed
  unsigned long currentTime = millis();  // saves current program time
  int tempLevel = analogRead(inputPin);
  int temperature = (tempLevel * 0.46560509554);  // convert analog temperature input to fahrenheit 
  static unsigned int prevTemp = temperature;
  if (currentTime - previousTime > waitTime) {  // when the program waits a certain amount of time
    prevTemp = temperature;
    previousTime = currentTime;
  }
  return prevTemp;
}