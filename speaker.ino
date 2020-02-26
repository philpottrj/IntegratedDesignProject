const int PIN = 6;  //photosensing LED connected to pin 13
const int analog = 0;  //A0

void setup() {  
  pinMode (PIN, OUTPUT); //set the photosensing LED pin as on output
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  tone(PIN, 523.25, 1000);
  delay(5000);

}
