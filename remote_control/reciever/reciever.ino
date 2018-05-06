#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

typedef enum  {
  MODE_ONE,
  MODE_TWO,
  MODE_THREE
}ledMode;

typedef struct _ToSend ToSend;

struct _ToSend{
  int power;
  ledMode mode;
  boolean isRestricted;
  int brightness;
};

ToSend global {
  0,
  MODE_ONE,
  false,
  0
};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    radio.read(&global, sizeof(ToSend));
    Serial.println(global.power);
  }
}

