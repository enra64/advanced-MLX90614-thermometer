/// This is the actual implementation that should run on the thermometer

#include "AdvancedThermometerLogic.hpp"

#define RUN 0
#define TEST_INPUT 1
#define TEST_DISPLAY 2
#define TEST_LOGGER 3
#define TEST_THERMOMETER_WRAPPER 4
#define MODE TEST_LOGGER


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

    #include <Logger.hpp>

void printBuffer(float* buffer, size_t length) {
    for(int i = 0; i < length; i++)
        Serial.println(buffer[i]);
    Serial.println("finished buffer printing");
}

    void setup() {
        Logger::init();

        Serial.println("appending 1.1, 2.2, 3.3, 4.5 to log");
        Logger::append(1.1);
        Logger::append(2.2);
        Logger::append(3.3);
        Logger::append(4.5);

        Serial.print("retrieving last value: ");
        float lastValue;
        Logger::getLastEntry(lastValue);
        Serial.println(lastValue);

        float buffer[10];

        Serial.println("retrieving less than full log:");
        for(int i = 0; i < 10; i++) buffer[i] = 0;
        Logger::getLog(buffer, 2);
        printBuffer(buffer, 10);

        Serial.println("retrieving exactly the full log:");
        for(int i = 0; i < 10; i++) buffer[i] = 0;
        Logger::getLog(buffer, 4);
        printBuffer(buffer, 10);

        Serial.println("retrieving more than full log:");
        for(int i = 0; i < 10; i++) buffer[i] = 0;
        Logger::getLog(buffer, 12);
        printBuffer(buffer, 10);
    }

    void loop() {

    }

#elif MODE == TEST_THERMOMETER_WRAPPER
#else
#error "INVALID OPERATION MODE"
#endif

