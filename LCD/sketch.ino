#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int potPin = A0;

/*changes the digital voltage value
between 0 and 1023 to voltage 
between 0V and 5V*/
int digital_to_voltage(int digital){
  int voltage = (digital * 5)/1023;
  return voltage;
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  int potValue = analogRead(potPin);
  int potVoltage = digital_to_voltage(potValue); 
  lcd.setCursor(0, 0);                      // Set cursor to start of the first row
  lcd.print("Digital Pot: ");
  lcd.print(potValue); 
  lcd.setCursor(0, 1);   
  lcd.print("Real Voltage: ");
  lcd.print(potVoltage);
}
