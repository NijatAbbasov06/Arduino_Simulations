#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <assert.h>
#include <DHT.h>



// OLED screen configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins and constants
int idtVal = 29;
int button = 0;
int lastButtonState = HIGH;
int lastButtonState2 = HIGH;
unsigned long startTime;
unsigned long lastDebounceTime = 0;  
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50;
unsigned long elapsedTime;
int time;
float temVal;
int TEM;
int len;
int len_val;
int currButtonState;
int currButtonState2;

enum Pins {
  lightPin = 4,
  DHT11_PIN = 9,
  incrPin = 12,
  decrPin = 13,
  relPin = 2
};

struct KalmanResult {
 float tem_kalman;
 float P;
 float tem_kalman_next;
 float P_next;
};


DHT dht(DHT11_PIN, DHT11);

void setup() {
  Serial. begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  dht.begin();
  
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  add_space(6);
  display.print("MADE BY");
  display.setCursor(0, 10);
  add_space(7);
  display.print("NIJAT");
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setCursor(0, 0);
  // display.print("TEM ");
  // display.print("IDT ");
  // display.print("SV ");
  // display.print("TM  ");
  // display.print("I");
  display.display();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(incrPin, INPUT);
  pinMode(decrPin, INPUT);
  pinMode(relPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(3, OUTPUT);

  startTime = millis();
}

unsigned long time_ms = millis();


float tem_kalman_prev = 4.25;
float P_prev = 1;
float R = 0.01;
float Q = 0;
float temKalman;


void loop() {
  time_ms = millis();
  // display.clearDisplay();
  display.setCursor(0, 10);
  
  // Read temperature from DHT sensor
  temVal = dht.readTemperature();
  if (isnan(temVal)) {
      Serial.println("Failed to read from DHT sensor!");
      display.print("TEM: ERR");
      display.display();
      delay(2000); // Wait 2 seconds before retrying
      return;
  }
  display.print("TEM: ");
  display.fillRect(30, 10, SCREEN_WIDTH - 30, 10, SSD1306_BLACK); 
  display.print(temVal);
  display.display();

  KalmanResult result = KalmanFilter(tem_kalman_prev, P_prev, temVal);
  float tem_kalman = result.tem_kalman;
  float P = result.P;
  //we use prev here for the next value
  float tem_kalman_prev = result.tem_kalman_next;
  float P_prev = result.P_next;

  display.setCursor(0, 20);
  display.print("Kalman val: ");
  display.fillRect(72, 20, SCREEN_WIDTH - 72, 20, SSD1306_BLACK); 
  display.print(tem_kalman);

  display.display();

  check_button_one_sec();

  if (idtVal > temVal) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }

  len = num_len(idtVal);
  add_space(3);
  // check_button_one_sec();

  display.setCursor(0, 30);
  display.print("IDT: ");
  display.fillRect(30, 30, SCREEN_WIDTH - 30, 10, SSD1306_BLACK);
  display.print(idtVal);
  display.display();

  // check_button_one_sec();

  elapsedTime = time_ms - startTime;
  time = elapsedTime / 1000;
  check_button_one_sec();

  display.setCursor(0, 40);
  display.fillRect(30, 40, SCREEN_WIDTH - 30, 10, SSD1306_BLACK);
  display.print("Time: ");
  display.print(time);
  display.display();



  display.setCursor(0, 50);
  display.fillRect(30, 50, SCREEN_WIDTH - 30, 10, SSD1306_BLACK);
  display.print("Rel: ");
  if (digitalRead(relPin)) {
    display.print("AK");
  } else {
    display.print("PS");
  }
  display.display();


  display.setCursor(60, 50);
  display.fillRect(102, 50, SCREEN_WIDTH - 102, 30, SSD1306_BLACK);
  display.print("Light: ");
  if (digitalRead(lightPin) == LOW) {
    display.print("G");
  } else {
    display.print("A");
  }
  display.display();

 
}

void add_space(int n) {
  for (; n > 0; n--) {
    display.print(" ");
  }
}

int num_len(int val) {
  len_val = 0;
  while (val) {
    val /= 10;
    len_val++;
  }
  return len_val;
}

void check_button_one_sec() {
  unsigned long start = millis();
  while (millis() < start + 200) {
    currButtonState = digitalRead(incrPin);

    if ((millis() - lastDebounceTime) > debounceDelay && lastButtonState == HIGH
        && currButtonState == LOW) {
      idtVal++;
      lastDebounceTime = millis();
    }

    lastButtonState = currButtonState;
    currButtonState2 = digitalRead(decrPin);

    if ((millis() - lastDebounceTime2) > debounceDelay && lastButtonState2 == HIGH 
        && currButtonState2 == LOW) {
      idtVal--;
      lastDebounceTime2 = millis();
    }
    lastButtonState2 = currButtonState2;

    if (idtVal < 0 || idtVal > 99) {
      return;
    }
  }
}

KalmanResult KalmanFilter(float x_prev, float P_prev, float z){
 KalmanResult result;
 float K = P_prev / (P_prev + R);
 result.tem_kalman = x_prev + K * (z - x_prev);
 result.P = (1- K) * P_prev;
 result.tem_kalman_next = result.tem_kalman;
 result.P_next = result.P + Q;
 return result;
}