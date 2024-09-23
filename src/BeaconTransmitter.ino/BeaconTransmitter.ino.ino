/**
 * @file BeaconTransmitter.ino
 * @brief This Arduino sketch reads air quality data from a sensor and broadcasts its measurements via Bluetooth Beacons.
 *
 * This program initializes a Bluetooth Low Energy (BLE) module to act as a beacon,
 * while simultaneously reading data from a ULPSM-O3 968-046 ozone sensor. The sensor
 * data (ozone concentration and temperature) is read at regular intervals and 
 * transmitted as part of the beacon's payload. This allows for remote monitoring
 * of air quality parameters using BLE-enabled devices.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-23
 */

#include <bluefruit.h>

// Pin definition for the ULPSM-O3 968-046 sensor's gas voltage output
#define PIN_VGAS 5
// Pin definition for the ULPSM-O3 968-046 sensor's reference voltage output
#define PIN_VREF 28
// Pin definition for the ULPSM-O3 968-046 sensor's temperature voltage output
#define PIN_VTEMP 29

// Sensitivity code from the sensor label, this changes between sensors
const float SENSITIVITY_CODE = -35.35;
// Gain for ozone sensor, this is always the same no matter what for this sensor
const float TIA_GAIN = 499;
// Calculated m value for ozone concentration calculation from the sensor's datasheet
const float m = SENSITIVITY_CODE * TIA_GAIN * 1e-9 * 1e3;

// UUID for the Bluetooth beacon
const uint8_t beaconUUID[16] = {
    'M', 'A', 'N', 'U', '-', 'E', 'P', 'S', 'G', '-', 'G', 'T', 'I', '-', '3', 'A'
};

/**
 * @struct SensorData
 * @brief Struct to hold sensor measurements
 */
struct SensorData {
    float ozonePPM;    // Ozone concentration in parts per million
    float temperature; // Temperature in degrees Celsius
};

/**
 * @brief Setup function to initialize the device
 * 
 * This function initializes the serial communication, sets up the Bluefruit module,
 * starts advertising, and configures the sensor pins.
 */
void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("-----------------------\n");
    Serial.println(" PRUEBAS iBeacon  ");
    Serial.println("-----------------------\n");
    
    Serial.println("Initialise the Bluefruit nRF52 module");
    Bluefruit.begin();
    
    Serial.println("Setting Device Name to ManusBeacon");
    Bluefruit.setName("ManusBeacon");
    Bluefruit.ScanResponse.addName();
  
    // Initialize analog pins for sensor reading
    pinMode(PIN_VGAS, INPUT);
    pinMode(PIN_VREF, INPUT);
    pinMode(PIN_VTEMP, INPUT);
}

/**
 * @brief Start advertising the Bluetooth beacon
 * 
 * This function configures and starts the Bluetooth advertising process,
 * setting up the device as a beacon with the specified UUID.
 */
void startAdvertising(SensorData sensorData) {
    Bluefruit.Advertising.stop();
    
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    
    Serial.println("Setting Device UUID to MANU-EPSG-GTI-3A");
    Bluefruit.Advertising.addName();

    BLEBeacon elBeacon(beaconUUID, sensorData.ozonePPM, sensorData.temperature, 73);
    elBeacon.setManufacturer(0x004c); // Apple ID
    Bluefruit.Advertising.setBeacon(elBeacon);
    
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
    
    Serial.println("Bluefruit.Advertising.start(0);");
}

/**
 * @brief Read sensor data
 * @return SensorData struct containing ozone concentration and temperature
 * 
 * This function reads the analog values from the sensor pins,
 * calculates the ozone concentration and temperature, and returns the data.
 */
SensorData readSensor() {
    float vgas = analogRead(PIN_VGAS) * (3.3 / 4096);
    float vref = analogRead(PIN_VREF) * (3.3 / 4096);
    float vtemp = analogRead(PIN_VTEMP) * (3.3 / 4096);

    SensorData data;
    data.ozonePPM = calculateOzone(vgas, vref);
    data.temperature = calculateTemperature(vtemp);

    return data;
}

/**
 * @brief Calculate ozone concentration
 * @param vgas Gas voltage from the sensor
 * @param vref Reference voltage from the sensor
 * @return float Ozone concentration in parts per million (ppm)
 */
float calculateOzone(float vgas, float vref) {
    return (vgas - vref) / m;
}

/**
 * @brief Calculate temperature
 * @param vtemp Temperature voltage from the sensor
 * @return float Temperature in degrees Celsius
 * 
 * @note This is a placeholder implementation and may need to be adjusted
 * based on the specific characteristics of the temperature sensor.
 */
float calculateTemperature(float vtemp) {
    return vtemp; // I have to still calibrate the temperature measurements
}

/**
 * @brief Main loop function
 * 
 * This function runs repeatedly, reading sensor data and printing it to the serial monitor.
 * It also handles the continuous advertising of the Bluetooth beacon.
 */
void loop() {
    SensorData sensorData = readSensor();
    
    Serial.print("Ozone concentration: ");
    Serial.print(sensorData.ozonePPM);
    Serial.println(" ppm");
    Serial.print("Temperature: ");
    Serial.print(sensorData.temperature);
    Serial.println(" °C");
    
    startAdvertising(sensorData);
    
    delay(1000);
}