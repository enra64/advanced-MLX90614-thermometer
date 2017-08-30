//
// Created by arne on 22.08.17.
//

#ifndef IR_THERMO_CONTINUOUSLOGGINGMANAGER_H
#define IR_THERMO_CONTINUOUSLOGGINGMANAGER_H

#include "ThermometerWrapper.h"
#include "my_config.h"
#include "Logger.hpp"

class ContinuousLoggingManager {
private:
    Thermometer *thermometer;
    long lastScan = 0;
    bool enabled;

public:
    explicit ContinuousLoggingManager(Thermometer *thermometer) : thermometer(thermometer), enabled(false) {
    }

    /// Scan if necessary
    /// \return true if a scan was executed
    bool update() {
        if (enabled && (millis() - lastScan) > CONTINUOUS_MEASUREMENT_INTERVAL_MILLI_SECS) {
            Logger::append(thermometer->getTemperature());
            lastScan = millis();
            return true;
        }
        return false;
    }

    bool isEnabled(){
        return enabled;
    }

    void enable() {
        enabled = true;
    }

    void disable() {
        enabled = false;
    }
};


#endif //IR_THERMO_CONTINUOUSLOGGINGMANAGER_H
