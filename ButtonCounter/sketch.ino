#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int lastButtonState = LOW;
int currButtonState;
int Count = 0;
int len_val;
enum Pins {
  buttonPin = 8
};
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
}

void loop() { 
  currButtonState = digitalRead(buttonPin);
  if (lastButtonState == LOW && currButtonState == HIGH) {
    Count++;
  }
  lastButtonState = digitalRead(buttonPin);
  lcd.setCursor(0,0);
  lcd.print(Count);
}

