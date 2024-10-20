/**
 * @file BeaconTransmitter.ino
 * @brief Main Arduino sketch for beacon transmission of sensor data
 *
 * This program initializes a Bluetooth Low Energy (BLE) module to act as a beacon,
 * while simultaneously reading data from a ULPSM-O3 968-046 ozone sensor. The sensor
 * data (ozone concentration and temperature) is read at regular intervals and 
 * transmitted as part of the beacon's payload. This allows for remote monitoring
 * of air quality parameters using BLE-enabled devices.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 */

#include "SerialPort.h"
#include "BLETransmitter.h"
#include "Sensor.h"
#include "Publisher.h"

#define PIN_VGAS = 5
#define PIN_VREF = 28
#define PIN_VTEMP = 29
#define SENSITIVITY_CODE = -35.35
#define X_CALIBRATION = 0.64
#define Y_CALIBRATION = -2.56

SerialPort port(115200);
BLETransmitter transmitter("ManusBeacon", 0x004c, 4);
Sensor sensor(PIN_VGAS, PIN_VREF, PIN_VTEMP, SENSITIVITY_CODE, X_CALIBRATION, Y_CALIBRATION);
Publisher publisher(transmitter, sensor);

void setup() {
    port.waitForAvailable();
    
    port.writeLine("-----------------------");
    port.writeLine(" iBeacon TESTS  ");
    port.writeLine("-----------------------");
    
    port.writeLine("Initializing the Bluefruit nRF52 module");
    transmitter.turnOnTransmitter();
    
    port.writeLine("Setting Device Name to ManusBeacon");
    
    sensor.initializeSensor();
    
    publisher.publishData();
}

void loop() {
    SensorData data = sensor.measureData();
    
    port.write("Ozone concentration: ");
    port.write(data.ozonePPM);
    port.writeLine(" ppm");
    
    port.write("Temperature: ");
    port.write(data.temperature);
    port.writeLine(" °C");
    
    publisher.publishData();
    
    delay(1000);
}