/*
	Milestone 2
	Created by Risa Philpott & Bernie Cieplak
	Parts taken from the following website for LCD Code:
	http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay <- code in the public domain
	
*/

// include the library code:
#include <LiquidCrystal.h>

// definitions
#define INTRUDER_SOUND 6 // intruder speaker output
#define INTRUDER_LED 13 //intruder alert LED is connected to pin 13
#define INTRUDER_INPUT 1 //analog input pin one
#define NIGHT_OUTPUT 10 // night light output LED pin
#define NIGHT_INPUT 0 // night light input pin - light sensor
#define WEATHER_INPUT 2 // weather station temperature sensor input
#define WEATHER_OUTPUT 9 // weather station appliance output
#define SWITCH_INPUT 3 // smart switch input
#define SWITCH_OUTPUT 8 // smart switch output


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
bool LCD_ON = false;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool nightOn, switchOn, intruderOn = false;
int weatherTemp = 0;
String topText = "Hello";
String bottomText = "World";

void setup() {
	lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  pinMode (INTRUDER_LED, OUTPUT); //tells the computer the pin corresponding to the intruder alert LED is an output
  pinMode (INTRUDER_SOUND, OUTPUT); //set the photosensing LED pin as an output
  pinMode (NIGHT_OUTPUT, OUTPUT); //set the photosensing LED pin as an output
  pinMode (WEATHER_OUTPUT, OUTPUT); //set the temperature controlled appliance pin as an output
  pinMode (SWITCH_OUTPUT, OUTPUT); //set the smart switch appliance pin as an output
  Serial.begin(9600);
}

void loop() {
  intruderOn = intruderAlertLight(INTRUDER_LED, INTRUDER_INPUT);  //flash intruder alert LED when intruder alert button is triggered
  nightOn = nightLight(NIGHT_INPUT, NIGHT_OUTPUT); // create instance of night light function
	weatherTemp = weatherStation(WEATHER_OUTPUT,WEATHER_INPUT);  // create instance of weather station
	switchOn = smartSwitch(SWITCH_OUTPUT,SWITCH_INPUT); // create instance of smart switch
	if(!intruderOn) {
		printStatus(topText,bottomText);
	}
}

void printAlert() { 
	const int waitTime = 500;
  static unsigned long previousTime = 0;  //previous time the LED state was changed
	unsigned long currentTime = millis();  //saves current program time
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
	const int waitTime = 1000;
  static unsigned long previousTime = 0;  //previous time the LED state was changed
	unsigned long currentTime = millis();  //saves current program time
	if (currentTime - previousTime > waitTime) {  //when the program waits a certain amount of time
		previousTime = currentTime; 
		lcd.clear();
		printLCD(top,bottom);
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
  buttonLevel = analogRead(SWITCH_INPUT);  //reads the voltage level of the button from the analog one sensor pin
  //Serial.println(buttonLevel);
  ledActivated = isButtonPressed(buttonLevel);  //when the button is pressed, LED is activated, and vice versa
  buttonToggled = updateButtonToggle(ledActivated, buttonToggled);  //the LED is toggled when intruder mode is activated, and vice versa
  if (buttonToggled) {  //when the intruder alert IS toggled
		digitalWrite(ledPin, HIGH); // turn on the smart switch
  }
  else {  //when the intruder alert is NOT toggled
    digitalWrite(ledPin, LOW); // turn off the smart switch
  }
	return buttonToggled;
}

/*
 * Function Name:       intuderAlertLight
 * Function Parameters: The pin number of the LED used to simulate the intruder alert, as a constant integer. 
 *                      The pin number of the analog sensor pin that measures the intruder alert activation button voltage, as a constant integer. 
 * Function Return:     Nothing. 
 * Function Behavior:   Flashes the input LED when the button corresponding to the input sensor pin is triggered. When the button is un-triggered is is off.
 */
bool intruderAlertLight(const int ledPin, const int sensorPin) {
  static bool intruderAlertToggled = false;
  static bool intruderModeActivated = false;
  static int buttonLevel = LOW;
  buttonLevel = analogRead(INTRUDER_INPUT);  //reads the voltage level of the button from the analog one sensor pin
  //Serial.println(buttonLevel);
  intruderModeActivated = isButtonPressed(buttonLevel);  //when the button is pressed, intruder mode is activated, and vice versa
  intruderAlertToggled = updateButtonToggle(intruderModeActivated, intruderAlertToggled);  //the intruder alert is toggled when intruder mode is activated, and vice versa
  if (intruderAlertToggled) {  //when the intruder alert IS toggled
    flashLED(ledPin);  //flash the intruder alert LED  
		printAlert();
		playSpeakerTone(INTRUDER_SOUND, 1523.25, 200);
  }
  else {  //when the intruder alert is NOT toggled
    digitalWrite(ledPin, LOW);  //turn the intruder alert LED off
    lcd.clear();
    LCD_ON = false; // Is the LCD on in the Intruder Blinking function?
		noTone(INTRUDER_SOUND);
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
  unsigned long currentTime = millis();  //saves current program time
  static unsigned long previousTime = 0;  //previous time the LED state was changed
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

bool nightLight(const int sensorPin, const int LED_pin) {
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
  static unsigned long previousTime = 0;  //previous time the LED state was changed
  unsigned long currentTime = millis();  //saves current program time
  static bool toneOn = false;
	if (currentTime - previousTime > waitTime) {  //when the program waits a certain amount of time
		previousTime = currentTime; 
		if (!toneOn) {  //when tone is OFF aka not playing
			toneOn = true;  //turn tone ON
		}
		else {
			toneOn = false;  //turn tone OFF
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
int weatherStation(const int outputPin, const int inputPin) {
  int tempLevel = analogRead(inputPin);
  float temperature = (tempLevel * 0.46560509554);  //convert serial temperature input to fahrenheit 
  const int threshold = 85;
   
  if (temperature > threshold){  //when the temperature is too high
    digitalWrite(outputPin, HIGH); //turn LED on
  }
  else{  //when the temperature is not too high
    digitalWrite(outputPin, LOW); //turn LED off
  }
  Serial.print("Temp Level: ");
  Serial.print(tempLevel);
  Serial.print("Temperature: ");
  Serial.println(temperature);
	return temperature;
}
