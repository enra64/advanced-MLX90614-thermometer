#include <Wire.h>
#include "Adafruit_MLX90614.h"

Adafruit_MLX90614 tempSensor = Adafruit_MLX90614();

void setup() {
  tempSensor.begin();
}

double getTemperature() {
  // readObjectAmbient? see adafruit...
  return tempSensor.readObjectTempC();
}

void loop() {
  
}
