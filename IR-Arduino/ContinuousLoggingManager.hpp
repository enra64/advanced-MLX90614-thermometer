//
// Created by arne on 22.08.17.
//

#ifndef IR_THERMO_CONTINUOUSLOGGINGMANAGER_H
#define IR_THERMO_CONTINUOUSLOGGINGMANAGER_H

#include "ThermometerWrapper.h"
#include "my_config.h"

class ContinuousLoggingManager {
private:
    Thermometer *thermometer;
    long lastScan = 0;
    bool enabled;

    void executeScan() {
        thermometer->getTemperature();
        lastScan = millis();
    }

public:
    explicit ContinuousLoggingManager(Thermometer *thermometer) : thermometer(thermometer), enabled(false) {
    }

    /// Scan if necessary
    /// \return true if a scan was executed
    bool update() {
        if (enabled && (millis() - lastScan) > 1000 * CONTINUOUS_MEASUREMENT_INTERVAL_SECS) {
            executeScan();
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
