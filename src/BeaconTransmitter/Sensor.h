// Sensor.h
#ifndef SENSOR_H
#define SENSOR_H

/**
 * @class Sensor
 * @brief A class to manage sensor reading and calibration
 * 
 * This class provides methods to read and calibrate measures from the gas sensor
 * via analogReadings with 10 bytes of precision at PIN VGAS, PIN VREF and PIN VTEMP
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
    const float X_CALIBRATION;
    const float Y_CALIBRATION;
    const float TIA_GAIN;
    const float m;

public:
    /**
     * @brief Constructor for Sensor
     *
     *    Natural:pinVgas
     *    Natural:pinVref   ---> Constructor()   
     *    Natural:pinVtemp
     *    Real:sensitivityCode
     *    Real:X 
     *    Real:Y
     *
     * @param pinVgas Pin for gas voltage
     * @param pinVref Pin for reference voltage
     * @param pinVtemp Pin for temperature voltage
     * @param sensitivityCode Sensitivity code of the sensor
     * @param X Calibration value for the ozone reading
     * @param Y Calibration value for the ozone reading
     */
    Sensor(int pinVgas, int pinVref, int pinVtemp, float sensitivityCode, float X, float Y)
        : PIN_VGAS(pinVgas), PIN_VREF(pinVref), PIN_VTEMP(pinVtemp),
          SENSITIVITY_CODE(sensitivityCode), X_CALIBRATION(X), Y_CALIBRATION(Y), 
          TIA_GAIN(499), m(SENSITIVITY_CODE * TIA_GAIN * 1e-9 * 1e3) {
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
     *
     *    measureData() ---> SensorData:[Natural:ozonePPM, Natural:temperature]
     *
     * @return SensorData struct containing ozone and temperature measurements
     */
    SensorData measureData() {
        float vgas = analogRead(PIN_VGAS) * (3.3 / 1023);
        float vref = analogRead(PIN_VREF) * (3.3 / 1023);
        float vtemp = analogRead(PIN_VTEMP) * (3.3 / 1023);

        SensorData data;
        data.ozonePPM = calculateOzone(vgas, vref);
        data.temperature = calculateTemperature(vtemp);

        return data;
    }

private:
    /**
     * @brief Calculate ozone concentration
     *
     *    Real:vgas ---> calculateOzone() ---> Natural
     *    Real:vref
     *
     * @param vgas Gas voltage
     * @param vref Reference voltage
     * @return Ozone concentration in PPM
     */
    int calculateOzone(float vgas, float vref) {
        int ozone = (vgas - vref) / m;
        if(ozone < 0) {
          ozone = ozone*(-1); // pending calibration
        }
        return int((ozone*X_CALIBRATION) + Y_CALIBRATION);
    }

    /**
     * @brief Calculate temperature
     *
     *    Real:vtemp ---> calculateTemperature() ---> Natural
     *
     * @param vtemp Temperature voltage
     * @return Temperature in degrees Celsius
     */
    int calculateTemperature(float vtemp) {
        return int(vtemp*10); // Pending calibration
    }
};

#endif