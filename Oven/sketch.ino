#include <LiquidCrystal_I2C.h>
#include <assert.h>
#include <dht.h>


dht DHT;

LiquidCrystal_I2C lcd(0x27, 20, 4);


int idtVal = 29;
int button = 0;
int lastButtonState = HIGH;
int lastButtonState2 = HIGH;
unsigned long startTime;
unsigned long lastDebounceTime = 0;  
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50;



enum Pins{
  lightPin = 4,
  DHT22_PIN = 9,
  incrPin = 12,
  decrPin = 13,
  relPin = 2
  };


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  add_space(6);
  lcd.print("MADE BY");
  lcd.setCursor(0,1);
  add_space(7);
  lcd.print("NIJAT");
  delay(2000);
  lcd.clear();
  lcd.print("TEM ");
  lcd.print("IDT ");
  lcd.print("SV ");
  lcd.print("TM  ");
  lcd.print("I");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(incrPin, INPUT);
  pinMode(decrPin, INPUT);
  pinMode(relPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(3, OUTPUT);
  startTime = millis();
}


void loop() {
  lcd.setCursor(0,1);
  int TEM = DHT.read22(DHT22_PIN);
  int temVal = DHT.temperature;
  lcd.print(temVal);


  check_button_one_sec();
  if (idtVal>temVal){
    digitalWrite(3, HIGH);
  }
  else{
    digitalWrite(3, LOW);
  }
  check_button_one_sec();

  int len = num_len(idtVal);
  add_space(3-len);
  check_button_one_sec();
  lcd.setCursor(8,1);
  if(digitalRead(relPin) ){
    lcd.print("AK");
  }
  else{
    lcd.print("PS");
  }
  
  check_button_one_sec();
  unsigned long elapsedTime = millis() - startTime;
  int time = elapsedTime / 1000;
  check_button_one_sec();
  lcd.setCursor(11, 1);
  lcd.print(time);
  check_button_one_sec();
  lcd.setCursor(15,1);
  if (digitalRead(lightPin) == LOW) {
    lcd.print("G");
  } else {
    lcd.print("A");
  }
  check_button_one_sec();
}

void add_space(int n){
  for (; n>0; n--){
    lcd.print(" ");
  }
}

int num_len(int val){
  int len = 0;
  while(val){
    val /= 10;
    len++;
  }
  return len;

}


void check_button_one_sec(){
  unsigned long start = millis();
  // check muddeti 1 san erzinde gedir
  while(millis()< start + 300){
    int currButtonState = digitalRead(incrPin);

    if ((millis() - lastDebounceTime) > debounceDelay && lastButtonState == HIGH
                                             && currButtonState == LOW) {
      idtVal++;
      lastDebounceTime = millis();
    }

    lastButtonState = currButtonState;
    int currButtonState2 = digitalRead(decrPin);

    if ((millis() - lastDebounceTime2) > debounceDelay && lastButtonState2 == HIGH 
                                        && currButtonState2 == LOW) {
      idtVal--;
      lastDebounceTime2 = millis();
      }
    lastButtonState2 = currButtonState2;



    if (idtVal<0 || idtVal>99){
      return 0;
    }
    lcd.setCursor(4,1);
    lcd.print(idtVal);
  }
  

}