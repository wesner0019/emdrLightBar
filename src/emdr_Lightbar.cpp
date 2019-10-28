#include "application.h"
#include "dotstar.h"

#define NUMPIXELS 30 // Number of LEDs in strip, turn off all extra on strip, can change back to final count

//-------------------------------------------------------------------
// NOTE: If you find that the colors you choose are not correct,
// there is an optional 2nd argument (for HW SPI) and
// 4th arg. (for SW SPI) that you may specify to correct the colors.
//-------------------------------------------------------------------
// e.g. Adafruit_DotStar(NUMPIXELS, DOTSTAR_RGB);
// e.g. Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_RGB);
//
// DOTSTAR_RGB
// DOTSTAR_RBG
// DOTSTAR_GRB
// DOTSTAR_GBR (default)
// DOTSTAR_BRG
// DOTSTAR_BGR

// #define DATAPIN   D4
// #define CLOCKPIN  D5
// Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN);

// Hardware SPI is a little faster, but must be wired to specific pins
// (Core/Photon/P1/Electron = pin A5 for data, A3 for clock)
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS);
int8_t trailingLEDs = 2;
int8_t head = 1, tail = 1;
void getSpeedSetting();
uint8_t mapSpeed = 0;
uint8_t maxSpeed = 200/24; //smallest delay 
uint8_t minSpeed = 4000/24; //largest delay

void setup() {
  strip.begin(); 
  strip.show();  
  #define NUMPIXELS 24 // Number of LEDs in strip
  pinMode(A1, INPUT);
  Serial.begin(9600);
  // for (int8_t i = NUMPIXELS; i <= 30; i++) { //turn off all extra on strip, can take out when strip is cut
  //   strip.setPixelColor(i, 0,0,0);
  //   strip.show();
  // }

  for (int8_t i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 255,0,0);
    strip.show();
  }
  delay(1000);

  for (int8_t i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0,255,0);
    strip.show();
  }
  delay(1000);

  for (int8_t i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0,0,255);
    strip.show();
  }
  delay(1000);
}

void loop() {
  head = 0;
  tail = trailingLEDs * -1;

  //strip.setBrightness(254);
  // Serial.println("start up");
  // Serial.print("head "); Serial.print(head); Serial.print(" tail "); Serial.println(tail);
  for (int8_t i = 1; i <= NUMPIXELS; i++) {
    strip.setPixelColor(head, 0,255,0);
    strip.setPixelColor(tail, 0);
    strip.show();
    //if (i < NUMPIXELS) strip.setBrightness(1);
    head++;
    tail++;
    // Serial.print("head "); Serial.print(head); Serial.print(" tail "); Serial.println(tail);
    getSpeedSetting();
    if (trailingLEDs > 1 && i == NUMPIXELS) {
      strip.setPixelColor(22, 0);
      strip.show();
    }
    delay(mapSpeed);  
  }

  head = NUMPIXELS-1;
  tail = head + trailingLEDs;

  //strip.setBrightness(254);
  // Serial.println("start down");
  // Serial.print("head "); Serial.print(head); Serial.print(" tail "); Serial.println(tail);
  for (int8_t i = NUMPIXELS; i >= 1; i--) {
    strip.setPixelColor(head, 0,255,0);
    strip.setPixelColor(tail, 0);
    strip.show();
    //if (i > 1) strip.setBrightness(1);
    head--;
    tail--;
    // Serial.print("head "); Serial.print(head); Serial.print(" tail "); Serial.println(tail);
    getSpeedSetting();
    if (trailingLEDs > 1 && i == 1) {
      strip.setPixelColor(1, 0);
      strip.show();
    }
    delay(mapSpeed);  
  }
  
}

void getSpeedSetting() {
  static uint32_t prevGetSpeedMillis = 0;

  if (millis() - prevGetSpeedMillis > 1000) {
    uint16_t voltageValue = analogRead(A1); //Voltage divider reading
    mapSpeed = map(voltageValue, 0 ,4095, maxSpeed, minSpeed);
    prevGetSpeedMillis = millis();
  }
}
