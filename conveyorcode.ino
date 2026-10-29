
/*
  Arduino Mega 2560 + DM542 Stepper Driver

  Toggle 1 (pin 2):
    ON  = Forward
    OFF = Reverse

  Toggle 2 (pin 3):
    ON  = Enable driver and run conveyor
    OFF = Stop pulses and disable driver

  Signal wiring must match your exact DM542 model.
*/

const byte STEP_PIN = 6;
const byte DIR_PIN = 7;
const byte ENA_PIN = 8;

const byte DIRECTION_SWITCH_PIN = 2;
const byte CONVEYOR_SWITCH_PIN = 3;

// Speed: 500 us per half-pulse = about 1000 pulses/sec
const unsigned int STEP_HALF_PERIOD_US = 1000;

// Confirm enable polarity in your DM542 manual.
// Change to LOW if LOW is the documented ENABLE level.
const byte DRIVER_ENABLED_LEVEL = HIGH;

const bool FORWARD_LEVEL = HIGH;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  pinMode(DIRECTION_SWITCH_PIN, INPUT_PULLUP);
  pinMode(CONVEYOR_SWITCH_PIN, INPUT_PULLUP);

  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);

  // Start with the driver disabled.
  digitalWrite(ENA_PIN, !DRIVER_ENABLED_LEVEL);
}

void loop() {
  // Toggle switches connect their inputs to GND when ON.
  bool directionOn =
      (digitalRead(DIRECTION_SWITCH_PIN) == LOW);

  bool conveyorOn =
      (digitalRead(CONVEYOR_SWITCH_PIN) == LOW);

  // Set direction before enabling motion.
  bool forward = directionOn;

  digitalWrite(
    DIR_PIN,
    (forward == FORWARD_LEVEL) ? HIGH : LOW
  );

  if (!conveyorOn) {
    // Stop sending step pulses.
    digitalWrite(STEP_PIN, LOW);

    // Disable the driver, if supported by its ENA input.
    digitalWrite(ENA_PIN, !DRIVER_ENABLED_LEVEL);
    return;
  }

  // Enable the driver before sending step pulses.
  digitalWrite(ENA_PIN, DRIVER_ENABLED_LEVEL);

  // Generate one step pulse.
  delayMicroseconds(STEP_HALF_PERIOD_US);
  digitalWrite(STEP_PIN, HIGH);

  delayMicroseconds(STEP_HALF_PERIOD_US);
  digitalWrite(STEP_PIN, LOW);
}

