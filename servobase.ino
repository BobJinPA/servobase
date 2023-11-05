#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define MULTIPLEX 0x70
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display3(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display4(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
#define SERVO1PIN 27
#define SERVO2PIN 14
#define SERVO3PIN 12
#define SERVO4PIN 13
int cursor_x = 20;
#define CLK1 39
#define DT1 36
#define CLK2 35
#define DT2 34
#define CLK3 33
#define DT3 32
#define CLK4 26
#define DT4 25
int counter1 = 90;
int counter2 = 90;
int counter3 = 90;
int counter4 = 90;
int clk1StateCurrent = LOW;
int clk1StateLast = clk1StateCurrent;
int clk2StateCurrent = LOW;
int clk2StateLast = clk2StateCurrent;
int clk3StateCurrent = LOW;
int clk3StateLast = clk1StateCurrent;
int clk4StateCurrent = LOW;
int clk4StateLast = clk2StateCurrent;
#define SPEEDPIN 6
char speedLable[] = {'S', 'M', 'F'};
int speed[] = {1,3,6};
int currentSpeed[] = {1,1,1,1};

void setup() {
  Serial.begin(9600);
  pinMode(CLK1, INPUT);
  pinMode(DT1, INPUT);
  pinMode(CLK2, INPUT);
  pinMode(DT2, INPUT);
  pinMode(CLK3, INPUT);
  pinMode(DT3, INPUT);
  pinMode(CLK4, INPUT);
  pinMode(DT4, INPUT);

  clk1StateLast = digitalRead(CLK1);
  clk2StateLast = digitalRead(CLK2);
  clk3StateLast = digitalRead(CLK3);
  clk4StateLast = digitalRead(CLK4);

  attachInterrupt(digitalPinToInterrupt(DT1), update1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT2), update2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT3), update3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT4), update4, CHANGE);

  Wire.begin();

  multiplexSelect(0);
  if (!display1.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display1.clearDisplay();
  display1.setTextColor(WHITE);

  multiplexSelect(1);
  if (!display2.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display2.clearDisplay();
  display2.setTextColor(WHITE);

  multiplexSelect(2);
  if (!display3.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display3.clearDisplay();
  display3.setTextColor(WHITE);

  multiplexSelect(3);
  if (!display4.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display4.clearDisplay();
  display4.setTextColor(WHITE);

  servo1.attach(SERVO1PIN), 500, 2400;
  servo1.setPeriodHertz(50);
  servo2.attach(SERVO2PIN), 500, 2400;
  servo2.setPeriodHertz(50);
  servo3.attach(SERVO3PIN), 500, 2400;
  servo3.setPeriodHertz(50);
  servo4.attach(SERVO4PIN), 500, 2400;
  servo4.setPeriodHertz(50);
}

void loop() {
  writeOled(0, counter1);
  writeOled(1, counter2);
  writeOled(2, counter3);
  writeOled(3, counter4);
  servo1.write(counter1);
  servo2.write(counter2);
  servo3.write(counter3);
  servo4.write(counter4);

}

void writeOled(int position, int counter) {
  if (counter < 10) {
    cursor_x = 50;
  } else if (counter < 100) {
    cursor_x = 35;
  } else {
    cursor_x = 15;
  }
  multiplexSelect(position);
  display1.setCursor(cursor_x, 14);
  display1.setTextSize(5);
  display1.println(counter);

  display1.setCursor(3, 10);
  display1.setTextSize(2);
  display1.println(speedLable[currentSpeed[position]]);

  display1.display();
  display1.clearDisplay();
}

void update1() {
  clk1StateCurrent = digitalRead(CLK1);
  if ((clk1StateLast == LOW) && (clk1StateCurrent == HIGH)) {
    if (digitalRead(DT1) == HIGH) {
      if (counter1 < 180) {
        counter1 = counter1 + speed[currentSpeed[0]];
      }
    } else {
      if (counter1 > 0) {
        counter1 = counter1 - speed[currentSpeed[0]];
      }
    }
  }
  clk1StateLast = clk1StateCurrent;
}
void update2() {
  clk2StateCurrent = digitalRead(CLK2);
  if ((clk2StateLast == LOW) && (clk2StateCurrent == HIGH)) {
    if (digitalRead(DT2) == HIGH) {
      if (counter2 < 180) {
        counter2 = counter2 + speed[currentSpeed[1]];
      }
    } else {
      if (counter2 > 0) {
        counter2 = counter2 - speed[currentSpeed[1]];
      }
    }
  }
  clk2StateLast = clk2StateCurrent;
}

void update3() {
  clk3StateCurrent = digitalRead(CLK3);
  if ((clk3StateLast == LOW) && (clk3StateCurrent == HIGH)) {
    if (digitalRead(DT3) == HIGH) {
      if (counter3 < 180) {
        counter3 = counter3 + speed[currentSpeed[2]];
      }
    } else {
      if (counter3 > 0) {
        counter3 = counter3 - speed[currentSpeed[2]];
      }
    }
  }
  clk3StateLast = clk3StateCurrent;
}

void update4() {
  clk4StateCurrent = digitalRead(CLK4);
  if ((clk4StateLast == LOW) && (clk4StateCurrent == HIGH)) {
    if (digitalRead(DT4) == HIGH) {
      if (counter4 < 180) {
        counter4 = counter4 + speed[currentSpeed[3]];
      }
    } else {
      if (counter4 > 0) {
        counter4 = counter4 - speed[currentSpeed[3]];
      }
    }
  }
  clk4StateLast = clk4StateCurrent;
}

void updateSpeed(){
  // delay(1);
  // if (digitalRead(SPEEDPIN)==HIGH){
  // if (currentSpeed == 2) {
  //   currentSpeed = 0;
  // } else {
  //   currentSpeed ++;
  // }}
}

void multiplexSelect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEX);
  Wire.write(1 << i);
  Wire.endTransmission();
}
