#ifndef IR_THERMO_LONGPRESSBOUNCER_H
#define IR_THERMO_LONGPRESSBOUNCER_H

#include <Arduino.h>
#include "../Bounce2/Bounce2.h"
#include "ClickListener.hpp"

#define LONG_CLICK_INTERVAL 500

class LongPressBouncer {
public:
    explicit LongPressBouncer(
            int pin,
            ClickListener shortClickListener,
            ClickListener longClickListener) :
            lastActivation(0),
            longClickListener(longClickListener),
            shortClickListener(shortClickListener) {
        bouncer.attach(pin, INPUT_PULLUP);
        pressed = bouncer.read();
    }

    void update() {
        bouncer.update();

        // pressed down
        if (bouncer.fell()) {
            lastActivation = millis();
            pressed = true;
        }

        // released before a long click was triggered
        if (pressed && bouncer.rose()) {
            pressed = false;
            shortClickListener(bouncer.read());
        }

        // held down longer than the LONG_CLICK_INTERVAL
        if (pressed && (millis() - lastActivation) > LONG_CLICK_INTERVAL) {
            longClickListener(bouncer.read());
            pressed = false;
        }
    }

private:
    bool pressed;
    double lastActivation;
    ClickListener shortClickListener, longClickListener;
    Bounce bouncer = Bounce();
};

#endif