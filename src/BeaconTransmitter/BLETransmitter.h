// BLETransmitter.h
#ifndef BLE_TRANSMITTER_H
#define BLE_TRANSMITTER_H

#include <bluefruit.h>

/**
 * @class BLETransmitter
 * @brief A class to manage BLE transmission
 * 
 * This class provides methods to initialize and use BLE for transmitting data.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 */
class BLETransmitter {
private:
    const char* transmitterName;
    const uint16_t manufacturerID;
    const int8_t txPower;
    uint8_t beaconUUID[16];

public:
    /**
     * @brief Constructor for BLETransmitter
     *
     *    Texto:name
     *    Natural:manuFacturer ---> Constructor()
     *    Natural:power
     *
     * @param name The name of the BLE transmitter
     * @param manufacturer The manufacturer ID
     * @param power The transmission power
     */
    BLETransmitter(const char* name, uint16_t manufacturer, int8_t power) 
        : transmitterName(name), manufacturerID(manufacturer), txPower(power) {
        memcpy(beaconUUID, "MANU-EPSG-GTI-3A", 16);
    }

    /**
     * @brief Turn on the BLE transmitter
     */
     void turnOnTransmitter() {
        Bluefruit.begin();
        Bluefruit.setName(transmitterName);
        Bluefruit.setTxPower(txPower);
        
        // Configurar advertising
        Bluefruit.Advertising.clearData();
        Bluefruit.ScanResponse.clearData();
        Bluefruit.ScanResponse.addName();

        // Configurar scanner para operar simultáneamente
        Bluefruit.Scanner.setInterval(1000, 100);   // Intervalo de escaneo
        Bluefruit.Scanner.useActiveScan(true);      // Solicitar más información
    }

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
};

#endif