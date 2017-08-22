#include <Wire.h>
#include "Adafruit_MLX90614.h"

Adafruit_MLX90614 tempSensor = Adafruit_MLX90614();

void setup() {
  tempSensor.begin();
}

float getTemperature() {
  // readObjectAmbient? see adafruit...
  return (float) tempSensor.readObjectTempC();
}

void loop() {
  
}
