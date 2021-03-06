#ifndef IR_THERMO_THERMOMETERWRAPPER_H
#define IR_THERMO_THERMOMETERWRAPPER_H

#include <Wire.h>
#include "Adafruit_MLX90614.h"

class Thermometer {
    Adafruit_MLX90614* tempSensor;
    bool initSuccess;
public:
    Thermometer() {
        // use tempSensor as a pointer to cease clions whinings
        tempSensor = new Adafruit_MLX90614();
        initSuccess = tempSensor->begin();
    }

    bool initializationSuccessful(){ return initSuccess; }

    float getTemperature() {
        // readObjectAmbient? see adafruit...
        return (float) tempSensor->readObjectTempC();
    }
};



#endif //IR_THERMO_THERMOMETERWRAPPER_H
