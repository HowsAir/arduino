# BreathBetter Arduino Beacon Transmitter

## Description

BreathBetter is an organization committed to informing users about the true quality of the air they breathe. This Arduino project is part of a larger system that includes an Android app and a Web Page. The Arduino sketch reads air quality data (ozone concentration and temperature) from a ULPSM-O3 968-046 sensor and broadcasts this information via Bluetooth Low Energy (BLE) beacons. This data can then be received and interpreted by Android devices running the BreathBetter app.



## Code Examples

Reading sensor data:

```cpp
SensorData measureData() {
    float vgas = analogRead(PIN_VGAS) * (3.3 / 4096);
    float vref = analogRead(PIN_VREF) * (3.3 / 4096);
    float vtemp = analogRead(PIN_VTEMP) * (3.3 / 4096);

    SensorData data;
    data.ozonePPM = calculateOzone(vgas, vref);
    data.temperature = calculateTemperature(vtemp);

    return data;
}
```

Broadcasting data as a BLE beacon:

```cpp
void broadcastIBeaconAdvertisement(int16_t major, int16_t minor, int8_t rssi) {
    Bluefruit.Advertising.stop();
    
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addName();

    BLEBeacon theBeacon(beaconUUID, major, minor, rssi);
    theBeacon.setManufacturer(manufacturerID);
    Bluefruit.Advertising.setBeacon(theBeacon);
    
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);
    Bluefruit.Advertising.setFastTimeout(30);
    Bluefruit.Advertising.start(0);
}
```

## Repository
Link to the BreathBetter Arduino Repository

## FAQ

* Q: What type of Arduino board is required?
  
    This project is designed for boards with built-in BLE capabilities, such as Adafruit nFR52 Mini.

* Q: How often does the device update the air quality data?

    By default, the device updates and broadcasts data every second. This can be adjusted in the code.

* Q: What's the range of the BLE beacon?

    The range can vary depending on environmental factors but is typically around 10-30 meters.

## Testing
To verify that the code is working correctly:

1. Open the Serial Monitor in the Arduino IDE after uploading the sketch

2. You should see output displaying the ozone concentration and temperature readings

3. Use a BLE scanner app on your smartphone to verify that the device is broadcasting as an iBeacon

4. The major and minor values in the iBeacon should correspond to the ozone and temperature readings respectively


## Classes Diagram

On doc's folder there's a diagram that shows how the classes interact:

* **BeaconTransmitter** is the main class that uses instances of SerialPort, BLETransmitter, Sensor, and Publisher.
* **Publisher** uses both BLETransmitter and Sensor to read data and broadcast it.
* **Sensor and BLETransmitter** are independent classes that handle sensor readings and BLE transmission respectively.
* **SerialPort** is used for debugging and displaying information.

