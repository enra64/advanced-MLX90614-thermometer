#include <Bounce2.h>

#define LONG_CLICK_INTERVAL 500

class LongPressBouncer {
  public:
    LongPressBouncer(int pin) {
      bouncer.attach(pin, INPUT_PULLUP);
    }

    void attachListeners(std::function<(bool)> shortClickListener, std::function<(bool)> longClickListener){
      this.longClickListener = longClickListener;
      this.shortClickListener = shortClickListener;
    }

    void update(){
      bouncer.update();
      
      // pressed down
      if(bouncer.fell()){
        lastActivation = millis();
        pressed = true;
      }

      // released before a long click was triggered
      if(pressed = bouncer.rose()) {
        pressed = false;
        shortClickListener(bouncer.read());
      }

      // held down longer than the LONG_CLICK_INTERVAL
      if(pressed && (millis() - lastActivation) > LONG_CLICK_INTERVAL){
        longClickListener(bouncer.read());
        pressed = false;
      }
    }
  
  private:
    int pin;
    bool pressed;
    double lastActivation;
    std::function<(bool)> shortClickListener = nullptr;
    std::function<(bool)> longClickListener = nullptr;
    Bounce bouncer = Bounce();
}

