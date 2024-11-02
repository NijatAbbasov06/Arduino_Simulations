#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int potPin = A0;

/*changes the digital voltage value
between 0 and 1023 to voltage 
between 0V and 5V*/
int digital_to_voltage(int digital){
  float voltage = (digital * 5)/1023;
  return voltage;
}

void setup() {

  lcd.begin(16,2);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

}

void loop() {

  int potValue = analogRead(potPin);
  if (potValue > 1023) {
    lcd.setCursor(0, 0);
    lcd.print("Error: > 1023"); 
    return;
}
  int potVoltage = digital_to_voltage(potValue); 
  lcd.setCursor(0, 0);                      
  lcd.print("Digital Pot: ");
  lcd.print(potValue); 
  String potValue_string = String(potValue);
  FillSpace("Digital Pot: ", potValue_string);
  lcd.setCursor(0, 1);   
  lcd.print("Real Voltage: ");
  lcd.print(potVoltage);
  String potVoltage_string = String(potVoltage);
  FillSpace("Real Voltage: ", potVoltage_string);
}


void FillSpace(const char* string1, String string2){

  int len = strlen(string1) + string2.length();
  
  for (int i =0; i< (20 -len); i++){
    lcd.print(" ");
  }
}

