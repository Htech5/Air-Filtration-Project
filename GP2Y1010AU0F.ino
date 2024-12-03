#include <Wire.h>
#include <GP2YDustSensor.h> // Library for dust sensor
const uint8_t SHARP_LED_PIN = 12; // Dust sensor LED pin
const uint8_t SHARP_VO_PIN = 34;
GP2YDustSensor dustSensor(GP2YDustSensorType :: GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

void setup() {
  Serial.begin(9600);
    dustSensor.begin();
    Serial.println(F("TEST DUST"));
}

void loop() {
  Serial.println(dustSensor.getDustDensity());

}
