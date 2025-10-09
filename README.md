# HX710B Pressure Sensor Library

An Arduino library for interfacing with the **MPS20N0040D pressure sensor** using the **HX710B ADC module**. This library provides pressure measurements in multiple units with calibration features.

## Features

- **Multi-unit Support**: Pressure readings in Pascal, kPa, PSI, mmHg, and ATM
- **Advanced Calibration**: Two-point linear calibration for improved accuracy
- **Robust Communication**: Interrupt-safe SPI communication with timeout handling
- **Cross-platform**: Compatible with Arduino, ESP32, ESP8266, and other platforms
- **Power Management**: Built-in power down/wake up functionality
- **Ready Detection**: Multiple methods for checking sensor readiness

## Hardware Compatibility

- **Pressure Sensor**: MPS20N0040D (0-40 kPa range)
- **ADC Module**: HX710B
- **Microcontrollers**: Arduino Uno/Nano, ESP32, ESP8266, Teensy, and more

## Installation

1. Download this library as a ZIP file
2. In Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the downloaded ZIP file

## Quick Start

### Basic Usage

```cpp
#include "HX710B.h"

const int DOUT_Pin = 2;   // Sensor data pin
const int SCLK_Pin = 3;   // Sensor clock pin

HX710B pressure_sensor;

void setup() {
  Serial.begin(57600);
  pressure_sensor.begin(DOUT_Pin, SCLK_Pin);
}

void loop() {
  if (pressure_sensor.is_ready()) {
    Serial.print("Pressure: ");
    Serial.print(pressure_sensor.kPaCal());  // Calibrated reading
    Serial.println(" kPa");
  }
  delay(1000);
}
```

### Available Pressure Units

```cpp
float pascals = pressure_sensor.pascal();    // Pascals (Pa)
float kilopascals = pressure_sensor.kPaCal(); // Kilopascals (kPa) - calibrated
float psi = pressure_sensor.psi();           // Pounds per square inch
float mmHg = pressure_sensor.mmHg();         // Millimeters of mercury
float atm = pressure_sensor.atm();           // Atmospheres
```

## Calibration

For accurate measurements, calibrate your sensor using the two-point method:

1. **Zero Point**: Record reading with sensor open to air
2. **Reference Point**: Apply known pressure (e.g., 10 kPa)
3. **Calculate**: Library computes linear calibration coefficients

See `examples/HX710B_Calibration/` for a complete calibration example.

## API Reference

### Initialization
- `begin(dout, pd_sck, gain)` - Initialize sensor with pin assignments
- `setGain(gain)` - Set ADC gain (32, 64, or 128)

### Reading Functions
- `is_ready()` - Check if sensor is ready for reading
- `read()` - Get single raw reading
- `read_average(times)` - Get averaged raw reading
- `pascal()`, `kPaCal()`, `psi()`, `mmHg()`, `atm()` - Pressure in various units

### Calibration
- `setCalibration(m, b)` - Set linear calibration coefficients
- `hasCalibration()` - Check if calibration is set
- `clearCalibration()` - Remove calibration

### Utility
- `tare(times)` - Set zero offset
- `power_down()` / `power_up()` - Power management
- `wait_ready_timeout(ms)` - Wait for sensor with timeout

## Examples

- **HX710B_Basic**: Simple pressure reading example
- **HX710B_Calibration**: Complete calibration procedure with EEPROM storage

## Wiring

| HX710B Pin | Arduino Pin | Description |
|------------|-------------|-------------|
| VCC        | 5V/3.3V     | Power supply |
| GND        | GND         | Ground |
| DOUT       | Digital Pin | Data output |
| SCLK       | Digital Pin | Clock input |

## Troubleshooting

- **"Sensor not found"**: Check wiring and power supply
- **Inconsistent readings**: Perform calibration
- **Communication errors**: Verify pin connections and try different pins

## License

MIT License - see source files for details.

## Author

**R. Pelayo** - [roland@teachmemicro.com](mailto:roland@teachmemicro.com)

## Full Tutorial

For detailed setup instructions, wiring diagrams, and advanced usage examples, visit:
**https://www.teachmemicro.com/arduino-pressure-sensor-tutorial**

## Contributing

Issues and pull requests are welcome! Please check the GitHub repository for the latest updates and to report any problems.
