// Sensor.h
#ifndef SENSOR_H
#define SENSOR_H

/**
 * @class Sensor
 * @brief A class to manage sensor reading and calibration
 * 
 * This class provides methods to read and calibrate measures from the gas sensor
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 */

/**
 * @struct SensorData
 * @brief Structure to hold sensor measurements
 */
struct SensorData {
    int ozonePPM;    /**< Ozone concentration in parts per million */
    int temperature; /**< Temperature in degrees Celsius */
};

/**
 * @class Sensor
 * @brief A class to manage sensor operations
 * 
 * This class provides methods to initialize and read data from the sensor.
 * 
 * @author Manuel Antonio Borregales Perez
 * @date 2024-09-30
 */
class Sensor {
private:
    const int PIN_VGAS;
    const int PIN_VREF;
    const int PIN_VTEMP;
    const float SENSITIVITY_CODE;
    const float TIA_GAIN;
    const float m;

public:
    /**
     * @brief Constructor for Sensor
     * @param pinVgas Pin for gas voltage
     * @param pinVref Pin for reference voltage
     * @param pinVtemp Pin for temperature voltage
     * @param sensitivityCode Sensitivity code of the sensor
     */
    Sensor(int pinVgas, int pinVref, int pinVtemp, float sensitivityCode)
        : PIN_VGAS(pinVgas), PIN_VREF(pinVref), PIN_VTEMP(pinVtemp),
          SENSITIVITY_CODE(sensitivityCode), TIA_GAIN(499),
          m(SENSITIVITY_CODE * TIA_GAIN * 1e-9 * 1e3) {
    }

    /**
     * @brief Initialize the sensor
     */
    void initializeSensor() {
        pinMode(PIN_VGAS, INPUT);
        pinMode(PIN_VREF, INPUT);
        pinMode(PIN_VTEMP, INPUT);
    }

    /**
     * @brief Measure sensor data
     * @return SensorData struct containing ozone and temperature measurements
     */
    SensorData measureData() {
        float vgas = analogRead(PIN_VGAS) * (3.3 / 4096);
        float vref = analogRead(PIN_VREF) * (3.3 / 4096);
        float vtemp = analogRead(PIN_VTEMP) * (3.3 / 4096);

        SensorData data;
        data.ozonePPM = calculateOzone(vgas, vref);
        data.temperature = calculateTemperature(vtemp);

        return data;
    }

private:
    /**
     * @brief Calculate ozone concentration
     * @param vgas Gas voltage
     * @param vref Reference voltage
     * @return Ozone concentration in PPM
     */
    int calculateOzone(float vgas, float vref) {
        return int(50 - ((vgas - vref) / m));
    }

    /**
     * @brief Calculate temperature
     * @param vtemp Temperature voltage
     * @return Temperature in degrees Celsius
     */
    int calculateTemperature(float vtemp) {
        return int(vtemp + 20); // Pending calibration
    }
};

#endif