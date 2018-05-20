// screen sda A4, sck A5

//#include <SPI.h>
#include <nRF24L01.h>
//#include "U8glib.h"
#include <RF24.h>
//#include "OneButton.h"

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

#define pot A0

long c_time = millis();


typedef enum  {
  MODE_ONE,
  MODE_TWO,
  MODE_THREE,
  MODE_FOUR,
  MODE_FIVE
}ledMode;

typedef struct _ToSend ToSend;

struct _ToSend{
  int power;
  ledMode mode;
};

boolean butt_flag = 0;
boolean butt;
unsigned long last_press;

ToSend global { 0, MODE_ONE };

void setup(void) {
  //buttonOne.setClickTicks(0);
  global.power = analogRead(0);
  global.power = map(global.power, 0, 1020, 0, 100);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  Serial.begin(9600);
  pinMode(pot, INPUT);
  pinMode(4, INPUT);
}

void loop(void) {
  butt = digitalRead(4);
  if (butt == 1 && butt_flag == 0 && millis() - last_press > 100) {
    butt_flag = 1;
    if(global.mode == MODE_ONE){
      global.mode = MODE_TWO;
    }
    else if(global.mode == MODE_TWO){
      global.mode = MODE_THREE;
    }
    else if(global.mode == MODE_THREE){
      global.mode = MODE_FOUR;
    }
    else if(global.mode == MODE_FOUR){
      global.mode = MODE_FIVE;
    }
    else if(global.mode == MODE_FIVE){
      global.mode = MODE_ONE;
    }
    radio.write(&global, sizeof(ToSend));
    last_press = millis();
  }
  if (butt == 0 && butt_flag == 1) {
    butt_flag = 0;
  }
  int val = analogRead(0);
  int power_q = map(val, 0, 1023, 800, 2300);
  int percent = map(val, 0, 1023, 0, 100);
  if(power_q != global.power){
    global.power = power_q;
    radio.write(&global, sizeof(ToSend));
  }  
}


void buttonOneClick() {
  if(global.mode == MODE_ONE){
    global.mode = MODE_TWO;
  }
  else if(global.mode == MODE_TWO){
    global.mode = MODE_THREE;
  }
  else if(global.mode == MODE_THREE){
    global.mode = MODE_FOUR;
  }
  else if(global.mode == MODE_FOUR){
    global.mode = MODE_FIVE;
  }
  else if(global.mode == MODE_FIVE){
    global.mode = MODE_ONE;
  }
  radio.write(&global, sizeof(ToSend));
}

