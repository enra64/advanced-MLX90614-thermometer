/// This is the actual implementation that should run on the thermometer

#include "AdvancedThermometerLogic.h"

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
    thermometerLogic->update();
}
