#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct KalmanResult {
 float x;
 float P;
 float x_next;
 float P_next;
};

int potPin = A3;

float x_prev = 2.5;
float P_prev = 1;

float R = 0.001;
float Q = 0;

float digital_to_voltage(float digital){
 float voltage = (digital * 5)/1023;
 return voltage;
}

void setup() {
 if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("SSD1306 allocation failed"));
   for(;;);
 }
 display.display();
 delay(2000);
 display.clearDisplay();
}

void loop() {

 float potValue = analogRead(potPin);
 float potVoltage = digital_to_voltage(potValue);

 display.clearDisplay();
 display.setCursor(0, 0);
 display.setTextSize(1);
 display.setTextColor(SSD1306_WHITE);


 display.print("Pot Value: ");
 display.print(potVoltage, 3);

 display.display();

 //printing kalman result
 KalmanResult result = KalmanFilter(x_prev, P_prev, potVoltage);

 float x = result.x;
 float P = result.P;
 //we use prev here for the next value
 float x_prev = result.x_next;
 float P_prev = result.P_next;

 display.setCursor(0, 10);
 display.print("Kalman val: ");
 display.print(x, 3);

 display.display();

 

 

}


/* K = Kalman gain,
  x = Estimation of potVoltage
  P = Covariance
  z = Meausurement (potVoltage value itself,
                   this is the measured value of potentiometer
                   voltage, and has some fluctuation.)
  Subscript prev means the projected estimate from the previous loop.
  Subscript next means the projected estimate in this loop for the next loop.
*/
KalmanResult KalmanFilter(float x_prev, float P_prev, float z){
 KalmanResult result;
 float K = P_prev / (P_prev + R);
 result.x = x_prev + K * (z - x_prev);
 result.P = (1- K) * P_prev;
 result.x_next = result.x;
 result.P_next = result.P + Q;
 return result;
}