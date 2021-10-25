#include <picoUART.h>
#include <pu_print.h>
#define DOWN_SENSOR 2
#define UP_SENSOR 3

enum moving_states {
  NO,
  CLOSING,
  OPENING
};

enum pin_changes {
  UNCHANGED,
  GOING_HIGH,
  GOING_LOW
};

// TODO
void pinMode(int pin);
unsigned char digitalRead(int pin);
void delay(long long ms);

class Sensor {
  private:
    int pin;
    int last_state;

  public:
    pin_changes last_change;
    Sensor() { };

    Sensor(int pin) {
      this->pin = pin;
      this->last_change = UNCHANGED;
      pinMode(this->pin);
      this->last_state = digitalRead(this->pin);
    };

    pin_changes debounced_read() {
      unsigned int statemask;
      unsigned int statecompare;
      int state = digitalRead(this->pin);
      if (state == this->last_state) {
        return UNCHANGED;
      }

      statemask = 1 - state;

      if (state == 1) {
        statecompare = 0xFFFF;
      } else {
        statecompare = 0;
      }

      while (statemask != statecompare) {
        state = digitalRead(this->pin);
        statemask = (statemask << 1) + state;
        delay(20);
      }

      if (statemask == 0xFFFF) {
        this->last_change = GOING_HIGH;
      } else {
        this->last_change = GOING_LOW;
      }
      this->last_state = state;
      return this->last_change;
    }

};

moving_states moving = NO;

Sensor up_sensor;
Sensor down_sensor;

int main() {
  if (digitalRead(DOWN_SENSOR)) {
    prints_P(PSTR("D"));
  } else if (digitalRead(UP_SENSOR)) {
    prints_P(PSTR("U"));
  }
  up_sensor = Sensor(UP_SENSOR);
  down_sensor = Sensor(DOWN_SENSOR);
  while (true) {
    if (up_sensor.debounced_read() == GOING_LOW) {
      moving = CLOSING;
      prints_P(PSTR("u"));
    }
    if (up_sensor.debounced_read() == GOING_HIGH) {
      moving = NO;
      prints_P(PSTR("U"));
    }

    if (down_sensor.debounced_read() == GOING_LOW) {
      moving = OPENING;
      prints_P(PSTR("d"));
    }
    if (down_sensor.debounced_read() == GOING_HIGH) {
      moving = NO;
      prints_P(PSTR("D"));
    }

  };

}
