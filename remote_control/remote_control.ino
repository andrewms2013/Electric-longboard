// screen sda A4, sck A5




#include "U8glib.h"
#include <SPI.h>
#include <RF24.h>

U8GLIB_SSD1306_128X64 u8g(0x3C);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4
#define debounce 80 
int encoder0Pos = 0;
int valRotary,lastValRotary;

typedef enum  {
  MODE_ONE,
  MODE_TWO,
  MODE_THREE
}ledMode;

typedef struct _toSend toSend;

struct _toSend{
  int power;
  ledMode mode;
  boolean isRestricted;
  int brightness;
};


typedef enum  {
  RIDE_MODE,
  LED_MODE
}MenuMode;

void printTextOnPos(short x, short y, String toPrint){
  u8g.setPrintPos(x, y);
  u8g.print(toPrint);
}

void draw(int val, int checked) {
  u8g.setFont(u8g_font_8x13);
  printTextOnPos(5, 10, "normal");
  printTextOnPos(90, 10, "100%");
  printTextOnPos(5, 35, "power");
  u8g.setFont(u8g_font_9x15);
  char intStr[16];
  itoa(val, intStr, 10);
  String str = String(intStr);
  str += "%";
  printTextOnPos(10, 50, str);
  MenuMode mode = (MenuMode)checked;
  switch (mode) {
    case RIDE_MODE: {
      u8g.setFont(u8g_font_8x13);
      printTextOnPos(80, 35, "ride");
      u8g.setPrintPos(77, 50);
      break;
    }
    case LED_MODE: {
      u8g.setFont(u8g_font_6x10);
      printTextOnPos(88, 35, "LED");
      u8g.setPrintPos(77, 50);
      u8g.setFont(u8g_font_8x13);
      break;
    }
  }
  String s;
  s += "<";
  s += " o " ;
  s += ">";
  u8g.print(s);
}

void setup(void) {
  // flip screen, if required
   //u8g.setRot90();
  Serial.begin(9600);
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE);
}
boolean butt_flag;
boolean butt;
boolean led_flag = 0;
unsigned long last_press;
void loop(void) {
  // picture loop
  butt = !digitalRead(encoder0Btn);
  if (butt == 1 && butt_flag == 0 && millis() - last_press > 100) {
    butt_flag = 1;
  }
  if (butt == 0 && butt_flag == 1) {
    butt_flag = 0;
    led_flag = !led_flag;
    last_press = millis();
  }
  Serial.print(butt_flag);
  Serial.print(" ");
  Serial.print(valRotary);
  Serial.println(" ");
  u8g.firstPage();  
  do {
    if(valRotary > 100){
      valRotary = 100;
    }
    else if(valRotary < 0){
      valRotary = 0;
    }
   draw(valRotary, led_flag);
  } while( u8g.nextPage() );
  
}
void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
  encoder0Pos++;
  }
  else
  {
  encoder0Pos--;
  }
  valRotary = encoder0Pos/2.5;
}
