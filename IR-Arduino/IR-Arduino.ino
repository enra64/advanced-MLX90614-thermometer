/// This is the actual implementation that should run on the thermometer

#include "AdvancedThermometerLogic.hpp"

#define RUN 0
#define TEST_INPUT 1
#define TEST_DISPLAY 2
#define TEST_LOGGER 3
#define TEST_THERMOMETER_WRAPPER 4
#define MODE TEST_DISPLAY


#if MODE == RUN
AdvancedThermometerLogic *thermometerLogic;
Input *inputHandler;

void setup() {
    inputHandler = new Input(
            shortTriggerListener,
            longTriggerListener,
            laserClickListener,
            backgroundLightClickListener
    );

    thermometerLogic = new AdvancedThermometerLogic();
}

// listeners because std::bind is too fat for AVRs
void backgroundLightClickListener(bool activated) { thermometerLogic->onBackgroundLightToggled(activated); }

void laserClickListener(bool activated) { thermometerLogic->onLaserToggled(activated); }

void shortTriggerListener(bool activated) { thermometerLogic->onTriggerShortClick(activated); }

void longTriggerListener(bool activated) { thermometerLogic->onTriggerLongClick(activated); }

void loop() {
    inputHandler->update();
    thermometerLogic->update();
}

#elif MODE == TEST_INPUT
Input *inputHandler;

void setup() {
    inputHandler = new Input(
            shortTriggerListener,
            longTriggerListener,
            laserClickListener,
            backgroundLightClickListener
    );
}

// listeners because std::bind is too fat for AVRs
void backgroundLightClickListener(bool activated) {
    Serial.print("background light clicked, activated: ");
    Serial.println(activated);
}

void laserClickListener(bool activated) {
    Serial.print("laser clicked, activated: ");
    Serial.println(activated);
}

void shortTriggerListener(bool activated) {
    Serial.print("trigger shortclicked, activated: ");
    Serial.println(activated);
}

void longTriggerListener(bool activated) {
    Serial.print("trigger longclicked, activated: ");
    Serial.println(activated);
}

void loop() {
    inputHandler->update();
}

#elif MODE == TEST_DISPLAY
#elif MODE == TEST_LOGGER
#elif MODE == TEST_THERMOMETER_WRAPPER
#else
#error "INVALID OPERATION MODE"
#endif

