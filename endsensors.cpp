#include <avr/io.h>
#include <util/delay.h>

class __FlashStringHelper;
#include <debugSerial.h>
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

template<uint8_t Pin>
struct Sensor {
    int8_t last_state;

    pin_changes last_change;
    Sensor() {
      DDRB = DDRB & ~(1 << Pin);    //set Pin as input
      PORTB = PORTB | (1 << Pin);  //activate internal pull-up resistor for Pin
      this->last_change = UNCHANGED;
      this->last_state = this->digitalRead();
    };

    uint8_t digitalRead() {
      return (PINB & (1 << Pin)) == (1 << Pin);
    }

    pin_changes debounced_read() {
      uint16_t statemask;
      uint16_t statecompare;
      int8_t state = this->digitalRead();
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
        state = this->digitalRead();
        statemask = (statemask << 1) + state;
        _delay_ms(20);
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

Sensor<UP_SENSOR> up_sensor;
Sensor<DOWN_SENSOR> down_sensor;

debugSerial dSerial;

int main(void) {
  if (up_sensor.digitalRead()) {
    dSerial.write('D');
  } else if (down_sensor.digitalRead()) {
    dSerial.write('U');
  }
  while (true) {
    if (up_sensor.debounced_read() == GOING_LOW) {
      moving = CLOSING;
      dSerial.write('u');
    }
    if (up_sensor.debounced_read() == GOING_HIGH) {
      moving = NO;
      dSerial.write('U');
    }

    if (down_sensor.debounced_read() == GOING_LOW) {
      moving = OPENING;
      dSerial.write('d');
    }
    if (down_sensor.debounced_read() == GOING_HIGH) {
      moving = NO;
      dSerial.write('D');
    }

  };

}
