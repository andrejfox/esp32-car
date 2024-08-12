#include <Arduino.h>

void setup() {
    Serial.begin(921600);
}

void loop() { 
    Serial.println("test");
    delay(500);
}
