#include "HX710B.h"

const int DOUT_Pin = 2;   //sensor data pin
const int SCLK_Pin  = 3;   //sensor clock pin

HX710B pressure_sensor; 

void setup() {
  Serial.begin(57600);
  pressure_sensor.begin(DOUT_Pin, SCLK_Pin);
}

void loop() {

  if (pressure_sensor.is_ready()) {
    Serial.print("Pascal: ");
    Serial.println(pressure_sensor.pascal());
    Serial.print("ATM: ");
    Serial.println(pressure_sensor.atm());
    Serial.print("mmHg: ");
    Serial.println(pressure_sensor.mmHg());
    Serial.print("PSI: ");
    Serial.println(pressure_sensor.psi());
  } else {
    Serial.println("Pressure sensor not found.");
  }

  delay(1000);
  
}
