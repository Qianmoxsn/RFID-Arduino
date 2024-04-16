#include <Arduino.h>

unsigned long timerecord = 0;
unsigned long time_1s = 1000;
unsigned long time_2s = 2000;
bool led_state = 1;

void task1setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void task1loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}


void task2setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void task2loop() {
  if (millis() - timerecord > time_1s && led_state == 0) {
    timerecord = millis();
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }
  if (millis() - timerecord > time_2s && led_state == 1) {
    timerecord = millis();
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
  }
}


void task3setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
}

void task3loop() {
  int button_state = digitalRead(7);
  Serial.println(button_state);
  if (button_state == HIGH) {
    task2loop();
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(300);
}


void setup() {
  // task1setup();
  // task2setup();
  task3setup();
}

void loop() {
  // task1loop();
  // task2loop();
  task3loop();
}