// Подключаем библиотеку FastLED.
#include "FastLED.h"

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 74

// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 6

// Создаем переменную strip для управления нашей лентой.
CRGB strip[LED_COUNT];

void setup()
{
  // Добавляем ленту
  FastLED.addLeds<WS2812B, PB8, RGB>(strip, LED_COUNT);
}

void loop()
{
  // Включаем все светодиоды
  for (int i = 0; i < LED_COUNT; i++)
  {
    if(i < 37){
      strip[i] = CRGB::White;
    }
    else strip[i] = CRGB::Green;
  }
  // Передаем цвета ленте.
  FastLED.show();
//  // Ждем 500 мс.
  delay(450);
  for (int i = 0; i < LED_COUNT; i++)
  {
    if(i < 37){
      strip[i] = CRGB::Green;
    }
    else strip[i] = CRGB::White;
  }
//  // Выключаем все светодиоды.
//  for (int i = 0; i < LED_COUNT; i++)
//  {
//    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
//  }
//  // Передаем цвета ленте.
  FastLED.show();
//  // Ждем 500 мс.
  delay(650);
}
