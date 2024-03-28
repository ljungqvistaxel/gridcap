/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 3;


HX711 scale;

void setup() {
  Serial.begin(115200);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(-247.07);   // this value is obtained by calibrating the scale with known weights;
  scale.tare();				        // reset the scale to 0

  Serial.print("\n\rReadings:\n\r");
}

void loop() {
  Serial.print("average:\t");
  Serial.println(scale.get_units(5), 1);
  delay(1);
}