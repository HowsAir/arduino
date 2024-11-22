/**
 * @file BeaconTransmitter.ino
 * @brief Main Arduino sketch for beacon transmission and reception
 * 
 * This program initializes a Bluetooth Low Energy (BLE) module to act as both transmitter
 * and receiver. It transmits sensor data as beacons while simultaneously listening for
 * control beacons that can activate the buzzer.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Alejandro Rosado Jiménez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 * last edited 2024-11-09
 */

#include "SerialPort.h"
#include "BLETransmitter.h"
#include "Sensor.h"
#include "Publisher.h"
#include "Zumbador.h"

#define PIN_VGAS 5
#define PIN_VREF 28
#define PIN_VTEMP 29
#define SENSITIVITY_CODE -35.35
#define X_CALIBRATION 0.64
#define Y_CALIBRATION -2.56

SerialPort port(115200);
BLETransmitter transmitter("ManusBeacon", 0x0000, 4);
Sensor sensor(PIN_VGAS, PIN_VREF, PIN_VTEMP, SENSITIVITY_CODE, X_CALIBRATION, Y_CALIBRATION);
Publisher publisher(transmitter, sensor);
Zumbador zumbador(3);

// Add a flag to track if the buzzer is already active
bool buzzerActive = false;
unsigned long lastBuzzerTime = 0;
const unsigned long BUZZER_COOLDOWN = 2000;

unsigned long lastPublishTime = 0;
unsigned long lastBuzzerUpdateTime = 0;
const unsigned long PUBLISH_INTERVAL = 1000;  // 1 second
const unsigned long BUZZER_UPDATE_INTERVAL = 50;  // 10 times per second

// Helper function to print hex values
void printHex(SerialPort& port, uint8_t num) {
    if (num < 16) {
        port.write("0");
    }
    char hex[3];
    sprintf(hex, "%X", num);
    port.write(hex);
}


/**
 * @brief Callback function for handling BLE scan reports
 * 
 * This function processes BLE advertisement reports received during scanning. 
 * It parses the report to check for manufacturer-specific data, and if the 
 * data matches a specific pattern, it identifies it as a valid buzzer command. 
 * Depending on the command byte, it activates or deactivates the buzzer.
 * 
 * @param report Pointer to the BLE advertisement report
 */
void scan_callback(ble_gap_evt_adv_report_t* report) {
    uint8_t buffer[32];
    uint8_t len = Bluefruit.Scanner.parseReportByType(
        report, 
        BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 
        buffer, 
        sizeof(buffer)
    );

    // Process only advertisements that meet specific criteria
    if (len == 5 && buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0xBE && buffer[3] == 0xEE) {
        port.writeLine("Valid buzzer command identifier received!");

        uint8_t command = buffer[4];
        if (command == 0x01) { 
            zumbador.activar();
        } else if (command == 0x00) {
            zumbador.desactivar();
        }
    }

    // Resume scanning to continue receiving advertisements
    Bluefruit.Scanner.resume();
}

void setup() {
    port.waitForAvailable();
    
    port.writeLine("-----------------------");
    port.writeLine(" iBeacon TESTS  ");
    port.writeLine("-----------------------");
    
    // Initialize the buzzer first
    zumbador.iniciar();
    digitalWrite(3, LOW);
    
    port.writeLine("Initializing the Bluefruit nRF52 module");
    transmitter.turnOnTransmitter();
    
    port.writeLine("Setting Device Name to ManusBeacon");
    
    sensor.initializeSensor();
    
    Bluefruit.Scanner.setRxCallback(scan_callback);
    // Scan more frequently: interval = 100ms, window = 50ms
    Bluefruit.Scanner.setInterval(80, 40);
    Bluefruit.Scanner.useActiveScan(true);
    Bluefruit.Scanner.start(0);
    
    port.writeLine("Scanner initialized for buzzer commands");
    publisher.publishData();
}


void loop() {
   unsigned long currentMillis = millis();
    
    // Update buzzer more frequently
    if (currentMillis - lastBuzzerUpdateTime >= BUZZER_UPDATE_INTERVAL) {
        zumbador.update();
        lastBuzzerUpdateTime = currentMillis;
    }

    // Publish data less frequently
    if (currentMillis - lastPublishTime >= PUBLISH_INTERVAL) {
        // Measure sensor data
        SensorData data = sensor.measureData();
        port.write("Ozone concentration: ");
        port.write(data.ozonePPM);
        port.writeLine(" ppm");
        port.write("Temperature: ");
        port.write(data.temperature);
        port.writeLine(" °C");

        publisher.publishData();
        lastPublishTime = currentMillis;
    }

    // A small delay to prevent overwhelming the CPU
    delay(10);
}
