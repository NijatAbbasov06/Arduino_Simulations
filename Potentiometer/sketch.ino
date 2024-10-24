int potPin = A0;  
int ledPin = 9;     
int potValue = 0;  
int ledBrightness = 0; 

void setup() {
  pinMode(ledPin, OUTPUT); 
  Serial.begin(115200);     
}

void loop() {
  potValue = analogRead(potPin); 
  

  ledBrightness = map(potValue, 0, 1023, 0, 255);
  
  analogWrite(ledPin, ledBrightness); 
  

  
  Serial.println(potValue);
  // Serial.println(ledBrightness);
  
  delay(10);  
}
