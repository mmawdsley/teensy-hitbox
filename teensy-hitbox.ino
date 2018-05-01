#include <Bounce.h>

#define LEFT_PIN 0
#define LEFT_KEY KEY_A
#define RIGHT_PIN 2
#define RIGHT_KEY KEY_D
#define UP_PIN 3
#define UP_KEY KEY_W
#define DOWN_PIN 1
#define DOWN_KEY KEY_S
#define LIGHT_PUNCH_PIN 13
#define LIGHT_PUNCH_KEY KEY_G   /* X button */
#define MEDIUM_PUNCH_PIN 14
#define MEDIUM_PUNCH_KEY KEY_H  /* Y button */
#define HEAVY_PUNCH_PIN 15
#define HEAVY_PUNCH_KEY KEY_J   /* R1 */
#define EX_PUNCH_PIN 16
#define EX_PUNCH_KEY KEY_K      /* L1 */
#define LIGHT_KICK_PIN 13
#define LIGHT_KICK_KEY KEY_B    /* A button */
#define MEDIUM_KICK_PIN 14
#define MEDIUM_KICK_KEY KEY_N   /* B button */
#define HEAVY_KICK_PIN 15
#define HEAVY_KICK_KEY KEY_M    /* R2 */
#define EX_KICK_PIN 13
#define EX_KICK_KEY KEY_COMMA   /* L2 */
#define LEFT_CLICK_PIN
#define LEFT_CLICK_KEY          /* L3 */
#define RIGHT_CLICK_PIN
#define RIGHT_CLICK_KEY         /* R3 */
#define START_PIN
#define START_KEY KEY_ENTER     /* Start */
#define BACK_PIN
#define BACK_KEY                /* Back */

#define BUTTON_COUNT 8
#define BOUNCE_TIME 10

const int buttonPins[BUTTON_COUNT] = {
  LEFT_PIN,
  RIGHT_PIN,
  UP_PIN,
  DOWN_PIN,
  LIGHT_PUNCH_PIN,
  MEDIUM_PUNCH_PIN,
  HEAVY_PUNCH_PIN,
  EX_PUNCH_PIN,
  /* LIGHT_KICK_PIN, */
  /* MEDIUM_KICK_PIN, */
  /* HEAVY_KICK_PIN, */
  /* EX_KICK_PIN, */
  /* LEFT_CLICK_PIN, */
  /* RIGHT_CLICK_PIN, */
  /* START_PIN, */
  /* BACK_PIN, */
};

const int buttonKeys[BUTTON_COUNT] = {
  LEFT_KEY,
  RIGHT_KEY,
  UP_KEY,
  DOWN_KEY,
  LIGHT_PUNCH_KEY,
  MEDIUM_PUNCH_KEY,
  HEAVY_PUNCH_KEY,
  EX_PUNCH_KEY,
  /* LIGHT_KICK_KEY, */
  /* MEDIUM_KICK_KEY, */
  /* HEAVY_KICK_KEY, */
  /* EX_KICK_KEY, */
  /* LEFT_CLICK_KEY, */
  /* RIGHT_CLICK_KEY, */
  /* START_KEY, */
  /* BACK_KEY, */
};

const int readDelay = 10;

Bounce *buttons[BUTTON_COUNT];

void setup() {
  int i;

  /* Add each of the buttons to the array and initialise each pin as pull-up */
  for (i = 0; i < BUTTON_COUNT; i += 1) {
    buttons[i] = new Bounce(buttonPins[i], BOUNCE_TIME);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  int i;
  
  /* Update each of the buttons */
  for (i = 0; i < BUTTON_COUNT; i += 1) {
    buttons[i]->update();
  }

  /* Check to see whether each button is falling or rising */
  for (i = 0; i < BUTTON_COUNT; i += 1) {  
    if (buttons[i]->fallingEdge()) {
      Keyboard.press(buttonKeys[i]);
    } else if (buttons[i]->risingEdge()) {
      Keyboard.release(buttonKeys[i]);
    }
  }

  /* Delay next read */
  delay(readDelay);
}
