//
// Created by arne on 22.08.17.
//

#ifndef IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H
#define IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H


#include "Display.hpp"
#include "ThermometerWrapper.h"
#include "Logger.hpp"
#include "Input.hpp"
#include "ContinuousLoggingManager.hpp"

class AdvancedThermometerLogic {
private:
    Display *display = nullptr;
    Thermometer *thermometer = nullptr;
    ContinuousLoggingManager* scanManager = nullptr;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ClangTidyInspection" // wants to use default member initializers, don't trust that
    bool displayIsDirty, laserEnabled, backgroundLightEnabled;
#pragma clang diagnostic pop

    float* logBuffer;
    size_t logLength;
public:
    AdvancedThermometerLogic() : displayIsDirty(false), laserEnabled(false), backgroundLightEnabled(false) {
        // initialize main classes
        digitalWrite(SPI_CS_ATMEGA328, HIGH);
        Logger::init();
        display = new Display();
        thermometer = new Thermometer();
        scanManager = new ContinuousLoggingManager(thermometer);

        // create log buffer on heap
        logLength = display->getGraphHorizontalResolution();
        logBuffer = new float[logLength];

        // initialize the output pins for laser and background light control
        pinMode(BACKGROUND_LIGHT_OUTPUT_PIN, OUTPUT);
        pinMode(LASER_OUTPUT_PIN, OUTPUT);

        digitalWrite(BACKGROUND_LIGHT_OUTPUT_PIN, HIGH);
        digitalWrite(LASER_OUTPUT_PIN, HIGH);
    }

    /// Update the input system and the continuous scanning manager. If necessary, update the display.
    void update() {
        displayIsDirty |= scanManager->update();

        if(displayIsDirty){
            displayIsDirty = false;
            bool success = false;
            Serial.println("test1");
            size_t retrievedLogItems = Logger::getLog(logBuffer, logLength, success);
            Serial.println("test2");

            for (size_t i = 0; i < retrievedLogItems; i++)
                Serial.println(logBuffer[i]);
            Serial.println("finished buffer printing");

            display->update(logBuffer, retrievedLogItems, scanManager->isEnabled(), backgroundLightEnabled, laserEnabled);
        }
    }

    /// Called when the trigger was pressed for a short time.
    /// \param activated always false, here
    void onTriggerShortClick(bool activated) {
        if(!scanManager->isEnabled())
            Logger::append(thermometer->getTemperature());
        else
            scanManager->disable();
        dirty();
    }

    void onTriggerLongClick(bool activated) {
        scanManager->enable();
        dirty();
    }

    void onLaserToggled(bool activated) {
        laserEnabled = activated;
        // enable the laser (transistor lets through on low, so activation should digitalWrite a LOW
        digitalWrite(LASER_OUTPUT_PIN, static_cast<uint8_t>(!activated));
        dirty();
    }

    void onBackgroundLightToggled(bool activated) {
        backgroundLightEnabled = activated;
        digitalWrite(BACKGROUND_LIGHT_OUTPUT_PIN, static_cast<uint8_t>(!activated));
        dirty();
    }
private:
    /// Set the displayIsDirty flag to true, so that the next update call will update it
    void dirty() {
        displayIsDirty = true;
    }
};


#endif //IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H
