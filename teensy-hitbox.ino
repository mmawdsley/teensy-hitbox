#include <Bounce.h>

#define DEBUG
#define LEFT_PIN 0
#define RIGHT_PIN 2
#define UP_PIN 3
#define DOWN_PIN 1
#define LIGHT_PUNCH_PIN 13
#define MEDIUM_PUNCH_PIN 14
#define HEAVY_PUNCH_PIN 15
#define EX_PUNCH_PIN 16
#define LIGHT_KICK_PIN
#define MEDIUM_KICK_PIN
#define HEAVY_KICK_PIN
#define EX_KICK_PIN
#define LEFT_CLICK_PIN
#define RIGHT_CLICK_PIN
#define START_PIN
#define BACK_PIN
#define DIRECTION_COUNT 4
#define BUTTON_COUNT 4
#define BOUNCE_TIME 10

const int BUTTON_PINS[BUTTON_COUNT] = {
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

const int BUTTON_OUTPUTS[BUTTON_COUNT] = {
  1,
  2,
  3,
  4,
  /* 5, */
  /* 6, */
  /* 7, */
  /* 8, */
  /* 9, */
  /* 10, */
  /* 11, */
  /* 12, */
};

const int READ_DELAY = 10;
const int UP_BIT = 1;
const int DOWN_BIT = 2;
const int LEFT_BIT = 4;
const int RIGHT_BIT = 8;

Bounce leftButton = Bounce(LEFT_PIN, BOUNCE_TIME);
Bounce rightButton = Bounce(RIGHT_PIN, BOUNCE_TIME);
Bounce upButton = Bounce(UP_PIN, BOUNCE_TIME);
Bounce downButton = Bounce(DOWN_PIN, BOUNCE_TIME);
Bounce *buttons[BUTTON_COUNT];

struct Controller {
  bool up;
  bool down;
  bool left;
  bool right;
};

Controller controller;
Controller lastController;

void setup() {
  int i;

  // Initialise the directions
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  controller.up = false;
  controller.down = false;
  controller.left = false;
  controller.right = false;

  lastController.up = false;
  lastController.down = false;
  lastController.left = false;
  lastController.right = false;

  // Add each of the buttons to the array and initialise each pin as pull-up
  for (i = 0; i < BUTTON_COUNT; i += 1) {
    buttons[i] = new Bounce(BUTTON_PINS[i], BOUNCE_TIME);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  // Only send once we've read all the inputs
  Joystick.useManualSend(true);

  #ifdef DEBUG
  Serial.begin(9600); // USB is always 12 Mbit/sec
  #endif
}

void loop() {
  int i;

  // Update each of the directions
  leftButton.update();
  rightButton.update();
  upButton.update();
  downButton.update();

  // Update each of the buttons
  for (i = 0; i < BUTTON_COUNT; i += 1) {
    buttons[i]->update();
  }

  int direction = 0;

  // Check to see whether each of the directions is falling or rising
  if (upButton.fallingEdge() && downButton.fallingEdge()) {
    // Both pressed at the same time, return the stick to neutral
    #ifdef DEBUG
    Serial.println("Up and down pressed at the same time, ignoring");
    #endif

    controller.up = true;
    controller.down = true;

    direction = direction & ~ UP_BIT;
    direction = direction & ~ DOWN_BIT;
  } else {
    if (upButton.fallingEdge()) {
      // Up button pressed
      // Add the up bit and remove the down bit
      #ifdef DEBUG
      Serial.println("Up pressed");
      #endif

      direction = direction | UP_BIT;
      direction = direction & ~ DOWN_BIT;
      controller.up = true;
    } else if (upButton.risingEdge()) {
      // Up button released
      // Remove the up bit
      #ifdef DEBUG
      Serial.println("Up released");
      #endif

      direction = direction & ~ UP_BIT;
      controller.up = false;

      // Add the down bit if it's still being held
      if (controller.down) {
        #ifdef DEBUG
        Serial.println("Reverting to down");
        #endif
        direction = direction | DOWN_BIT;
      }
    }

    if (downButton.fallingEdge()) {
      // Down button pressed
      // Add the down but and remove the up bit
      #ifdef DEBUG
      Serial.println("Down pressed");
      #endif

      direction = direction | DOWN_BIT;
      direction = direction & ~ UP_BIT;

      controller.down = true;
    } else if (downButton.risingEdge()) {
      // Down button released
      // Remove the down bit
      #ifdef DEBUG
      Serial.println("Down released");
      #endif

      direction = direction & ~ DOWN_BIT;
      controller.down = false;

      if (controller.up) {
        #ifdef DEBUG
        Serial.println("Reverting to up");
        #endif

        direction = direction | UP_BIT;
      }
    }
  }

  if (leftButton.fallingEdge() && rightButton.fallingEdge()) {
    // Both pressed at the same time, return the stick to neutral
    #ifdef DEBUG
    Serial.println("Left and right pressed at the same time, ignoring");
    #endif

    controller.left = true;
    controller.right = true;

    direction = direction & ~ LEFT_BIT;
    direction = direction & ~ RIGHT_BIT;
  } else {
    if (leftButton.fallingEdge()) {
      // Left button pressed
      // Add the left bit and remove the right bit

      #ifdef DEBUG
      Serial.println("Left pressed");
      #endif

      direction = direction | LEFT_BIT;
      direction = direction & ~ RIGHT_BIT;

      controller.left = true;
    } else if (leftButton.risingEdge()) {
      // Left button released
      // Remove the left bit

      #ifdef DEBUG
      Serial.println("Left released");
      #endif

      direction = direction & ~ LEFT_BIT;
      controller.left = false;

      // Add the right if it's still being held
      if (controller.right) {
        #ifdef DEBUG
        Serial.println("Reverting to right");
        #endif

        direction = direction | RIGHT_BIT;
      }
    }

    if (rightButton.fallingEdge()) {
      // Right button pressed
      // Add the right bit and remove the left bit
      #ifdef DEBUG
      Serial.print("Right pressed, direction is ");
      Serial.println(direction);
      #endif

      direction = direction | RIGHT_BIT;
      direction = direction & ~LEFT_BIT;

      controller.right = true;

      #ifdef DEBUG
      Serial.print("Direction now ");
      Serial.println(direction);
      #endif
    } else if (rightButton.risingEdge()) {
      // Right button released
      // Remove the right bit
      #ifdef DEBUG
      Serial.println("Right released");
      #endif

      direction = direction & ~ RIGHT_BIT;
      controller.right = false;

      // Add the left bit if it's still being held
      if (controller.left) {
        #ifdef DEBUG
        Serial.println("Reverting to left");
        #endif

        direction = direction | LEFT_BIT;
      }
    }
  }

  // Translate the direction into an angle
  int angle;

  if (direction == UP_BIT) {
    angle = 0;
  } else if (direction & UP_BIT && direction & RIGHT_BIT) {
    angle = 45;
  } else if (direction == RIGHT_BIT) {
    angle = 90;
  } else if (direction & RIGHT_BIT && direction & DOWN_BIT) {
    angle = 135;
  } else if (direction == DOWN_BIT) {
    angle = 180;
  } else if (direction & DOWN_BIT && direction & LEFT_BIT) {
    angle = 225;
  } else if (direction == LEFT_BIT) {
    angle = 270;
  } else if (direction & LEFT_BIT && direction & UP_BIT) {
    angle = 315;
  } else {
    angle = -1;
  }

  // Set the default values for the inputs we don't use
  Joystick.X(512);
  Joystick.Y(512);
  Joystick.Z(512);
  Joystick.Zrotate(512);
  Joystick.sliderLeft(512);
  Joystick.sliderRight(512);

  // Set the direction
  Joystick.hat(angle);

  // Check to see whether each button is falling or rising
  for (i = 0; i < BUTTON_COUNT; i += 1) {
    if (buttons[i]->fallingEdge()) {
      Joystick.button(BUTTON_OUTPUTS[i], 1);
    } else if (buttons[i]->risingEdge()) {
      Joystick.button(BUTTON_OUTPUTS[i], 0);
    }
  }

  // Send the updated state
  Joystick.send_now();

  // Copy the state for next time
  lastController.up = controller.up;
  lastController.down = controller.down;
  lastController.left = controller.left;
  lastController.right = controller.right;

  // Delay next read
  delay(READ_DELAY);
}
