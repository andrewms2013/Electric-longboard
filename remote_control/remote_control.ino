// screen sda A4, sck A5

#include <SPI.h>
#include <nRF24L01.h>
#include "U8glib.h"
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

#define pot A0
#define buttonOne 2
#define buttonTwo 3


U8GLIB_SSD1306_128X64 u8g(0x3C);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

boolean buttonOneStatus = false;
boolean buttonTwoStatus = false;
boolean led_flag = 0;

typedef enum  {
  MODE_ONE,
  MODE_TWO,
  MODE_THREE
}ledMode;

typedef enum  {
  RIDE_MODE,
  LED_MODE
}MenuMode;

typedef struct _ToSend ToSend;

struct _ToSend{
  int power;
  ledMode mode;
  boolean isRestricted;
  int brightness;
};

ToSend global { 0, MODE_ONE, 0, 0 };

void printTextOnPos(short x, short y, String toPrint);
void draw(int val, int checked);

void setup(void) {
  global.power = analogRead(0);
  global.power = map(global.power, 0, 1020, 0, 100);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(buttonOne, INPUT);
  pinMode(buttonTwo, INPUT);
}

void loop(void) {
  if(digitalRead(buttonOne)){
    buttonOneStatus = !buttonOneStatus;
  }
  if(digitalRead(buttonTwo)){
    buttonTwoStatus = !buttonTwoStatus;
  }
  
  // picture loop
//  butt = !digitalRead(encoder0Btn);
//  if (butt == 1 && butt_flag == 0 && millis() - last_press > 100) {
//    butt_flag = 1;
//  }
//  if (butt == 0 && butt_flag == 1) {
//    butt_flag = 0;
//    led_flag = !led_flag;
//    last_press = millis();
//  }
//  Serial.print(butt_flag);
//  Serial.print(" ");
//  Serial.print(valRotary);
//  Serial.println(" ");
  u8g.firstPage();  
  do {
   int val = analogRead(0);
   val = map(val, 0, 1020, 0, 100);
   if(val != global.power){
    global.power = val;
    radio.write(&global, sizeof(ToSend));
   } 
   draw(val, buttonOneStatus);
  } while( u8g.nextPage() );
  
}

void printTextOnPos(short x, short y, String toPrint){
  u8g.setPrintPos(x, y);
  u8g.print(toPrint);
}

void draw(int val, int checked) {
  u8g.drawFrame(0,0,128,64);
  u8g.setFont(u8g_font_8x13);
  printTextOnPos(5, 12, "normal");
  printTextOnPos(90, 12, "100%");
  printTextOnPos(5, 43, "power");
  u8g.setFont(u8g_font_9x15);
  char intStr[16];
  itoa(val, intStr, 10);
  String str = String(intStr);
  str += "%";
  printTextOnPos(10, 58, str);
  MenuMode mode = (MenuMode)checked;
  switch (mode) {
    case RIDE_MODE: {
      u8g.setFont(u8g_font_8x13);
      printTextOnPos(80, 43, "ride");
      u8g.setPrintPos(77, 58);
      break;
    }
    case LED_MODE: {
      u8g.setFont(u8g_font_6x10);
      printTextOnPos(88, 43, "LED");
      u8g.setPrintPos(77, 58);
      u8g.setFont(u8g_font_8x13);
      break;
    }
  }
  String s = "< o >";
  u8g.print(s);
}
//void doEncoder()
//{
//  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
//  {
//  encoder0Pos++;
//  }
//  else
//  {
//  encoder0Pos--;
//  }
//  valRotary = encoder0Pos/2.5;
//}
