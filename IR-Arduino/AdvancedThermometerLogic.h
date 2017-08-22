//
// Created by arne on 22.08.17.
//

#ifndef IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H
#define IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H


#include "Input.hpp"
#include "Display.hpp"
#include "ThermometerWrapper.h"
#include "Logger.hpp"
#include "ContinuousLoggingManager.h"

class AdvancedThermometerLogic {
private:
    Input *inputHandler = nullptr;
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
        inputHandler = new Input(
                this->onTriggerShortClick,
                this->onTriggerLongClick,
                this->onLaserToggled,
                this->onBackgroundLightToggled
        );

        // initialize main classes
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

    void update() {
        inputHandler->update();
        Logger::getLog(logBuffer, logLength);
        displayIsDirty |= scanManager->update();

        if(displayIsDirty){
            clean();
            display->update(logBuffer, logLength, scanManager->isEnabled(), backgroundLightEnabled, laserEnabled);
        }
    }

private:
    void dirty() {
        displayIsDirty = true;
    }

    void clean() {
        displayIsDirty = false;
    }

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
};


#endif //IR_THERMO_ADVANCEDTHERMOMETERLOGIC_H
