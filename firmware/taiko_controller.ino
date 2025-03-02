#include "AnalogReadNow.h"

//#define DEBUG_OUTPUT
//#define DEBUG_OUTPUT_LIVE
//#define DEBUG_TIME
//#define DEBUG_DATA

#define ENABLE_KEYBOARD
//#define ENABLE_NS_JOYSTICK

//#define HAS_BUTTONS

#ifdef ENABLE_KEYBOARD
#include <Keyboard.h>
#endif

const int min_threshold = 45;
const long cd_length = 10000;
const float k_threshold = 1.5;
const float k_decay = 0.97;

const int pin[4] = {A3, A2, A1, A0};
const int key[4] = {'d', 'f', 'j', 'k'};
const float sens[4] = {.75, .55, .9, .6};

float threshold = 20;
int raw[4] = {0, 0, 0, 0};
float level[4] = {0, 0, 0, 0};
long cd[4] = {0, 0, 0, 0};
bool down[4] = {false, false, false, false};

typedef unsigned long time_t;
time_t t0 = 0;
time_t dt = 0, sdt = 0;

void sampleAll() {
  int prev[4];
  for (int i = 0; i < 4; ++i) {
    prev[i] = raw[i];
    raw[i] = analogRead(pin[i]);
    level[i] = abs(raw[i] - prev[i]) * sens[i];
  }
}

void setup() {
  analogReference(DEFAULT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#ifdef ENABLE_KEYBOARD
  Keyboard.begin();
#endif
  t0 = micros();
  Serial.begin(9600);
}

void loop() {
#ifdef ENABLE_KEYBOARD
  // Parse serial commands if needed
#endif
  
  time_t t1 = micros();
  dt = t1 - t0;
  sdt += dt;
  t0 = t1;
  
  // Decrease cooldown timers
  for (int i = 0; i < 4; ++i) {
    if (cd[i] > 0) {
      cd[i] -= dt;
      if (cd[i] <= 0) {
        cd[i] = 0;
        if (down[i]) {
#ifdef ENABLE_KEYBOARD
          Keyboard.release(key[i]);
#endif
          down[i] = false;
        }
      }
    }
  }
  
  sampleAll();  // Sample all sensors

  threshold *= k_decay;  // Adjust threshold
  
  // Find the sensor with the highest level
  int i_max = -1;
  float level_max = 0;
  for (int i = 0; i < 4; ++i) {
    if (level[i] > level_max) {
      level_max = level[i];
      i_max = i;
    }
  }

  // Check if the highest level exceeds the threshold
  if (level_max >= max(threshold, min_threshold)) {
    if (cd[i_max] == 0) {
      if (!down[i_max]) {
#ifdef DEBUG_DATA
        Serial.print(level[0], 1); Serial.print("\t");
        Serial.print(level[1], 1); Serial.print("\t");
        Serial.print(level[2], 1); Serial.print("\t");
        Serial.print(level[3], 1); Serial.print("\n");
#endif
#ifdef ENABLE_KEYBOARD
        Keyboard.press(key[i_max]);
#endif
        down[i_max] = true;
      }
      // Set cooldown for all sensors
      for (int i = 0; i < 4; ++i)
        cd[i] = cd_length;
    }
    sdt = 0;
    threshold = max(threshold, level_max * k_threshold);  // Increase threshold to prevent false triggers
  }

#ifdef DEBUG_OUTPUT
  // Debug output if needed
#endif

  long ddt = 300 - (micros() - t0);
  if (ddt > 3) delayMicroseconds(ddt);
}
