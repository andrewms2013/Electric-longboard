//last
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <time.h>
#include "FastLED.h"

#define trigPin 2
#define echoPin 3
#define LED_COUNT 74
#define LED_PIN 9
CRGB strip[LED_COUNT];

RF24 radio(7, 8); // CE, CSN
Servo motor;
const byte address[6] = "00001";
bool led_switch = false;
unsigned long c_time = millis();
unsigned long c_gay_time = millis();
unsigned long rainbow_time = millis();

typedef enum  {
  MODE_ONE,
  MODE_TWO,
  MODE_THREE,
  MODE_FOUR,
  MODE_FIVE
}ledMode;

int isNear;

bool gay_switch = true;
int gayNumber = 0;
unsigned long gayTime = millis();


int currentColour = random(0,255);
int prevColour = random(0,255);

typedef struct _ToSend ToSend;

struct _ToSend{
  int power;
  ledMode mode;
};

ToSend global {
  0,
  MODE_THREE
};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  motor.attach(5);
  motor.write(2300);
  delay(2500);
  motor.write(800);
  delay(2500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(strip, LED_COUNT);
}

unsigned short led_index = 0;
unsigned short led_hue = 0;
boolean isGrow = true;



void checkDistance(){
  int duration, cm; 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58;
  if(cm < 100){
    isNear = 1;
  }
  else{
    isNear = 0;
  }
}

void loop() {
  if (radio.available()) {
    radio.read(&global, sizeof(global));
    motor.write(global.power);
  }
  checkDistance();
  if(isNear){
    for (int i = 0; i < LED_COUNT; i++) {
        strip[i] = CRGB::Red;
    }
    FastLED.show();
    return;
  }

  switch (global.mode) {
    case (MODE_TWO): {
      if (millis() - 200 >= c_time) {
        led_switch = !led_switch;
        c_time = millis();
      }
      if (led_switch) {
        for (int i = 0; i < LED_COUNT; i++) {
          if(i < 37){
            strip[i] = CRGB::Yellow;
          }
          else strip[i] = CRGB::Blue;
        }
      } else {
        for (int i = 0; i < LED_COUNT; i++) {
          if(i < 37){
            strip[i] = CRGB::Blue;
          }
          else strip[i] = CRGB::Yellow;
        }    
      }
      break;
    }
//    case (MODE_TWO): {
//        for (int i = 0; i < LED_COUNT; i++) {
//          if (i >= led_index && i < led_index + 4) {
//            strip[i] = CRGB::Red;
//          } else {
//            strip[i] = CRGB::White;
//          }
//          if(isGrow) {
//            led_index++;
//            if (led_index >= LED_COUNT) {
//              led_index = LED_COUNT - 1;
//              isGrow = false;
//            }
//          } else {
//            led_index--;
//            if (led_index < 0) {
//              led_index = 0;
//              isGrow = true;
//            }
//          }
//          FastLED.delay(100);
//        break;
//      }
//      case (MODE_THREE): {
//        led_index++;
//        led_hue = led_hue + 5;
//        if (led_index >= LED_COUNT) {
//          led_index = 0;
//        } 
//        if (led_hue > 255) {
//          led_hue = 0;
//        }
//        strip[led_index] = CHSV(led_hue, 255, 255);
//        FastLED.delay(20);
//        break;
//      }
//      case (MODE_FOUR): {
//        led_hue++;
//        if (led_hue > 255) {
//          led_hue = 0;
//        }
//        for (int i = 0; i < LED_COUNT; i++) {
//          strip[i] = CHSV(led_hue, 255, 255);
//        }
//        break;
//      }
      case (MODE_ONE): {
      if (millis() - 3700 >= c_gay_time) {
        gayNumber = 0;
        gay_switch = !gay_switch;
        c_gay_time = millis();
      }
      if (gay_switch) {
        for (int i = 0; i < LED_COUNT; i++) {
           strip[i] = CHSV(224, 255, 255);
        }
      } else {
        if (millis() - 100 >= gayTime) {
          ++gayNumber;
          gayTime = millis();
        }
        for (int i = 0; i < LED_COUNT; i++) {
          if(i < 37 && i < gayNumber){
            strip[i] = CHSV(86, 140, 255);
          }
          else if(i < 37){
            strip[i] = CHSV(224, 255, 255);
          }
          else if(i > 36 && i - 36 < gayNumber){
            strip[i] = CRGB(0, 0, 200);
          }
          else{
            strip[i] = CHSV(224, 255, 255);
          }
        }    
      }
      break;
    }
      case (MODE_THREE): {
        if (millis() - 30 >= c_time) {
          c_time = millis();
          led_index++;
          led_hue = led_hue + 5;
          if (led_index >= LED_COUNT) {
            led_index = 0;
          } 
          if (led_hue > 255) {
            led_hue = 0;
          }
          strip[led_index] = CHSV(led_hue, 255, 255);
        }
        break;
    }
      case (MODE_FOUR): {
      if (millis() - 200 >= c_time) {
        led_switch = !led_switch;
        c_time = millis();
      }
      if (led_switch) {
        for (int i = 0; i < LED_COUNT; i++) {
          if(i < 37){
            strip[i] = CRGB::Blue;
          }
          else strip[i] = CRGB::Pink;
        }
      } else {
        for (int i = 0; i < LED_COUNT; i++) {
          if(i < 37){
            strip[i] = CRGB::Pink;
          }
          else strip[i] = CRGB::Blue;
        }    
      }
      break;
    }
      case (MODE_FIVE): {
        for (int i = 0; i < LED_COUNT; i++) {
          strip[i] = CRGB::Black;
        }
      }
    }
  
  FastLED.show();
}



