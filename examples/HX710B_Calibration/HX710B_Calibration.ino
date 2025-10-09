/*
1. Open Serial Monitor @ 57600.
2. Press z with sensor open to air → records Raw₀.
3. Apply known pressure (e.g., 10 kPa), press r → records Raw₁.
4. Press c → computes m and b, saves to EEPROM, calls setCalibration().
5. Press p → shows calibrated kPa/Pa/psi/mmHg.
*/


#include "HX710B.h"
#include <EEPROM.h>

const int PIN_DOUT = 2;
const int PIN_SCLK = 3;

// Known reference pressure for second point (kPa)
const double P1_kPa = 10.0;

struct Calib { double m, b; };
HX710B hx;

// Simple EEPROM helpers (store at address 0)
void saveCal(const Calib& c){ EEPROM.put(0, c); }
bool loadCal(Calib& c){
  EEPROM.get(0, c);
  return !isnan(c.m) && !isnan(c.b) && c.m != 0.0;
}

unsigned long avgRaw(uint16_t n=50){
  return hx.readRawAverage(n);
}

void setup(){
  Serial.begin(57600);
  hx.begin(PIN_DOUT, PIN_SCLK);

  Calib c;
  if (loadCal(c)) {
    hx.setCalibration(c.m, c.b);
    Serial.print(F("Loaded calib: m=")); Serial.print(c.m, 10);
    Serial.print(F(", b=")); Serial.println(c.b, 10);
  } else {
    Serial.println(F("No calib found. Press z (zero), then r (ref), then c (compute)."));
  }

  Serial.println(F("Commands: z=zero, r=ref, c=compute-save, p=live print"));
}

unsigned long Raw0=0, Raw1=0;

void loop(){
  if (Serial.available()){
    char cmd = tolower(Serial.read());
    if (cmd=='z'){
      Serial.println(F("[Zero] Keep open to air..."));
      delay(800);
      Raw0 = avgRaw();
      Serial.print(F("Raw0 = ")); Serial.println(Raw0);
    }
    else if (cmd=='r'){
      Serial.print(F("[Ref] Apply known pressure P1_kPa = "));
      Serial.println(P1_kPa, 3);
      delay(800);
      Raw1 = avgRaw();
      Serial.print(F("Raw1 = ")); Serial.println(Raw1);
    }
    else if (cmd=='c'){
      if (Raw1 > Raw0){
        Calib c;
        c.m = (P1_kPa - 0.0) / (double)(Raw1 - Raw0);
        c.b = -c.m * (double)Raw0;
        hx.setCalibration(c.m, c.b);
        saveCal(c);
        Serial.println(F("[Cal] Saved calibration:"));
        Serial.print(F("m=")); Serial.print(c.m, 10);
        Serial.print(F(", b=")); Serial.println(c.b, 10);
      } else {
        Serial.println(F("[Error] Do z then r first."));
      }
    }
    else if (cmd=='p'){
      Serial.println(F("[Live] Calibrated readings (kPa, Pa, psi, mmHg)..."));
      while (!Serial.available()){
        float PkPa = hx.kPaCal();
        float Pa   = PkPa * 1000.0f;
        float psi  = Pa / 6894.757f;
        float mmHg = Pa / 133.322f;
        Serial.print(F("kPa=")); Serial.print(PkPa, 3);
        Serial.print(F("  Pa=")); Serial.print(Pa, 0);
        Serial.print(F("  psi=")); Serial.print(psi, 3);
        Serial.print(F("  mmHg=")); Serial.println(mmHg, 1);
        delay(200);
      }
      while (Serial.available()) Serial.read();
    }
  }
}
