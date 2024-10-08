#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "BLETransmitter.h"
#include "Sensor.h"

/**
 * @class Publisher
 * @brief A class to manage data publishing
 * 
 * This class provides methods to publish sensor data via BLE.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 */
class Publisher {
private:
    BLETransmitter& transmitter; 
    Sensor& sensor;

public:
    /**
     * @brief Constructor for Publisher
     *
     *    BLETransmitter:ble
     *        Sensor:sen      ---> Constructor()
     *
     * @param ble Reference to BLETransmitter object
     * @param sen Reference to Sensor object
     */
    Publisher(BLETransmitter& ble, Sensor& sen) : transmitter(ble), sensor(sen) {} // this.transmitter = ble, this.sensor = sen

    /**
     * @brief Publish sensor data
     */
    void publishData() {
        SensorData data = sensor.measureData();
        transmitter.broadcastIBeaconAdvertisement(data.ozonePPM, data.temperature, 73);
    }
};

#endif