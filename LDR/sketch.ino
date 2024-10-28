#include<LiquidCrystal_I2C.h>

#define LDR_PIN 2

/*log(R)/log(lux) = GAMMA and 
*/
const float GAMMA = 0.7;
const float RL10 = 50;

/*changes the digital voltage value
between 0 and 1023 to voltage 
between 0V and 5V*/
int digital_to_voltage(int digital){
  int voltage = (digital * 5)/1023;
  return voltage;
}

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  delay(100);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  int analog = analogRead(A0);
  int vertVal = analogRead(A1);
  int potVal = analogRead(A2);

  float voltage = (analog *5)/1024;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float aux_var = RL10 * 1e3 * pow(10, GAMMA)/ resistance;
  float lux = pow(aux_var, (1/GAMMA));

  float potVoltage = digital_to_voltage(potVal); 

  lcd.setCursor(0,0);
  lcd.print("Lux val: ");
  lcd.print(lux);

  lcd.setCursor(0,1);
  lcd.print("Joystick val: ");
  lcd.print(vertVal);


  lcd.setCursor(0,2);
  lcd.print("Pot. val: ");
  lcd.print(potVoltage);
  

  


}
