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
 * last edited 2024-11-08
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

// Helper function to print hex values
void printHex(SerialPort& port, uint8_t num) {
    if (num < 16) {
        port.write("0");
    }
    char hex[3];
    sprintf(hex, "%X", num);
    port.write(hex);
}

void scan_callback(ble_gap_evt_adv_report_t* report) {
    uint8_t buffer[32];
    uint8_t len = Bluefruit.Scanner.parseReportByType(
        report, 
        BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 
        buffer, 
        sizeof(buffer)
    );

    // Procesar solo los anuncios que cumplen con los criterios
    if (len == 5 && buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0xBE && buffer[3] == 0xEE) {
        port.writeLine("Valid buzzer command identifier received!");

        uint8_t command = buffer[4];
        if (command == 0x01) { 
            zumbador.activar();
        } else if (command == 0x00) {
            zumbador.desactivar();
        }
    }

    // Continuar escaneando
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
    // Medir los datos del sensor
    SensorData data = sensor.measureData();
    port.write("Ozone concentration: ");
    port.write(data.ozonePPM);
    port.writeLine(" ppm");
    port.write("Temperature: ");
    port.write(data.temperature);
    port.writeLine(" °C");

    publisher.publishData();
    
    zumbador.update();

    delay(1000);
}
