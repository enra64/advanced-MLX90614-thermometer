#include <functional>

#include <Bounce2.h>
#include "LongPressBouncer.hpp"

#define PISTOL_TRIGGER_PIN 5
#define LASER_INPUT_PIN 4

Bounce laserBouncer = Bounce(); 
LongPressBouncer triggerBouncer = LongPressBouncer(PISTOL_TRIGGER_PIN);
std::function<(bool)> laserClickListener;

void init(std::function<(bool)> shortTriggerListener, std::function<(bool)> longTriggerListener, std::function<(bool)> _laserClickListener) {
  laserBouncer.attach(LASER_INPUT_PIN, INPUT_PULLUP);
  triggerBouncer.attachListeners(shortTriggerListener, longTriggerListener);
}

void update() {
  debouncer.update();

  // released
  if(laserBouncer.fell || laserBouncer.rose())
    laserClickListener(laserBouncer.read());
}

