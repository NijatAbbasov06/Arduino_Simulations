#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 20
#define LCD_LINES   4

#define VERT_PIN A0
#define HORZ_PIN A1
#define SEL_PIN  2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

/*changes the digital voltage value
between 0 and 1023 to voltage 
between 0V and 5V*/
int digital_to_voltage(int digital){
  int voltage = (digital * 5)/1023;
  return voltage;
}

void setup() {

  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT_PULLUP);

  

  lcd.begin(16,2);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

}

void loop() {

  lcd.backlight();
  int vertVal = analogRead(VERT_PIN);
  int horzVal = analogRead(HORZ_PIN);
  bool selPressed = digitalRead(SEL_PIN) == LOW;

  String vertVal_string = String(vertVal);
  String horzVal_string = String(horzVal);
  String selPressed_string = String(selPressed);
  

  lcd.setCursor(0, 0);
  lcd.print("Vertical Val: ");
  lcd.print(vertVal);
  FillSpace("Vertical Val: ", vertVal_string);
  lcd.setCursor(0,1);
  lcd.print("Horizontal Val: ");
  lcd.print(horzVal);
  FillSpace("Horizontal Val: ", horzVal_string);
  lcd.setCursor(0,2);
  lcd.print("Button is pressed: ");
  lcd.print(selPressed);
  FillSpace("Button is pressed: ", selPressed_string);
}


void FillSpace(const char* string1, String string2){

  int len = strlen(string1) + string2.length();
  
  for (int i =0; i< (20 -len); i++){
    lcd.print(" ");
  }
}