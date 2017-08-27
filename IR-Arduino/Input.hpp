#ifndef IR_THERMO_INPUT_H
#define IR_THERMO_INPUT_H

#include "Bounce2.h"
#include "LongPressBouncer.hpp"
#include "ClickListener.hpp"
#include "my_config.h"

class Input {
private:
    Bounce laserBouncer, backgroundBouncer;
    LongPressBouncer *triggerBouncer = nullptr;
    ClickListener laserClickListener, backgroundClickListener;

public:
    Input(
            ClickListener shortTriggerListener,
            ClickListener longTriggerListener,
            ClickListener _laserClickListener,
            ClickListener _backgroundLightClickListener) :
            laserClickListener(_laserClickListener),
            backgroundClickListener(_backgroundLightClickListener)
    {

        laserBouncer.attach(LASER_INPUT_PIN, INPUT_PULLUP);
        backgroundBouncer.attach(BACKGROUND_LIGHT_INPUT_PIN, INPUT_PULLUP);

        triggerBouncer = new LongPressBouncer(PISTOL_TRIGGER_PIN, shortTriggerListener, longTriggerListener);
    }

    void update() {
        triggerBouncer->update();
        laserBouncer.update();
        backgroundBouncer.update();

        // laser button updated?
        if (laserBouncer.rose()) laserClickListener(laserBouncer.read());

        // background button updated?
        if (backgroundBouncer.rose()) backgroundClickListener(backgroundBouncer.read());
    }
};

#endif// IR_THERMO_INPUT_H