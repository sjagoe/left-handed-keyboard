#include <Bounce2.h>

#define PEDAL_1 2
#define PEDAL_2 3
#define PEDAL_3 4
#define PEDAL_4 5

#define PEDAL_1_KEY KEY_LEFT_CTRL
#define PEDAL_2_KEY KEY_LEFT_SHIFT
#define PEDAL_3_KEY KEY_LEFT_ALT
#define PEDAL_4_KEY KEY_RIGHT_GUI

static uint8_t current_mode = 0;

static const uint8_t[][] pedal_keys = {
  {KEY_LEFT_CTRL},
  {KEY_LEFT_SHIFT},
  {KEY_LEFT_ALT},
  {KEY_RIGHT_GUI},
};


const uint8_t key_for_mode (const uint8_t input_pin, const uint8_t mode) {
  const uint8_t key;
  switch (input_pin) {
    case PEDAL_1:
      key = pedal_keys[0][mode];
      break;
    case PEDAL_2:
      key = pedal_keys[1][mode];
      break;
    case PEDAL_3:
      key = pedal_keys[2][mode];
      break;
    case PEDAL_4:
      key = pedal_keys[3][mode];
      break;
  };
  return key;
}


class Pedal {
  private:
    Bounce debouncer;
    uint8_t pin;
    const uint8_t key;
    uint8_t pressed;
  public:
    Pedal(uint8_t input_pin, uint8_t output_key, uint8_t pressed_val);
    void initialize();
    void update_state();
    void set_mode(uint8_t mode);
};


Pedal::Pedal(uint8_t input_pin, uint8_t output_key, uint8_t pressed_val) : debouncer() {
  this->pin = input_pin;
  this->key = output_key;
  this->pressed = pressed_val;
};


void Pedal::initialize() {
  // make pins input and turn on the
  // pullup resistors so they go high unless
  // connected to ground:
  pinMode(this->pin, INPUT_PULLUP);
  this->debouncer.attach(this->pin);
  this->debouncer.interval(5);
}


void Pedal::update_state() {
  boolean changed = this->debouncer.update();
  if (changed) {
    int value = this->debouncer.read();
    if (value == this->pressed) {
      // Pedal is depressed
      Keyboard.press(this->key);
    } else {
      Keyboard.release(this->key);
    }
  }
}


Pedal pedal_1 = Pedal(PEDAL_1, PEDAL_1_KEY, LOW);
Pedal pedal_2 = Pedal(PEDAL_2, PEDAL_2_KEY, LOW);
Pedal pedal_3 = Pedal(PEDAL_3, PEDAL_3_KEY, LOW);
Pedal pedal_4 = Pedal(PEDAL_4, PEDAL_4_KEY, LOW);


void setup() {
  pedal_1.initialize();
  pedal_2.initialize();
  pedal_3.initialize();
  pedal_4.initialize();
  Keyboard.begin();
}


void loop() {
  pedal_1.update_state();
  pedal_2.update_state();
  pedal_3.update_state();
  pedal_4.update_state();
}
