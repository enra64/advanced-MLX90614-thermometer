#include "../Bounce2/Bounce2.h"
#include "LongPressBouncer.hpp"
#include "ClickListener.hpp"
#include "my_config.h"

Bounce laserBouncer = Bounce();
Bounce backgroundBouncer = Bounce();
LongPressBouncer triggerBouncer = LongPressBouncer(PISTOL_TRIGGER_PIN);
ClickListener laserClickListener, backgroundClickListener;

void init(
        ClickListener shortTriggerListener,
        ClickListener longTriggerListener,
        ClickListener _laserClickListener,
        ClickListener _backgroundLightClickListener) {
    laserBouncer.attach(LASER_INPUT_PIN, INPUT_PULLUP);
    laserClickListener = _laserClickListener;

    backgroundBouncer.attach(BACKGROUND_LIGHT_INPUT_PIN, INPUT_PULLUP);
    backgroundClickListener = _backgroundLightClickListener;

    triggerBouncer.attachListeners(shortTriggerListener, longTriggerListener);
}

void update() {
    triggerBouncer.update();
    laserBouncer.update();
    backgroundBouncer.update();

    // laser button updated?
    if (laserBouncer.fell || laserBouncer.rose())
        laserClickListener(laserBouncer.read());

    // background button updated?
    if (backgroundBouncer.fell || backgroundBouncer.rose())
        backgroundClickListener(backgroundBouncer.read());
}

