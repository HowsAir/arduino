// SerialPort.h
#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

/**
 * @class SerialPort
 * @brief A class to manage serial communication
 * 
 * This class provides methods to initialize and use the serial port for communication.
 * 
 * @author Manuel Antonio Borregales Perez
 * @author Jordi Bataller Mascarell
 * @date 2024-09-30
 */
class SerialPort {
public:
    /**
     * @brief Constructor for SerialPort
     * @param baudRate The baud rate for serial communication
     */
    SerialPort(long baudRate) {
        Serial.begin(baudRate);
    }

    /**
     * @brief Wait for the serial port to become available
     */
    void waitForAvailable() {
        while (!Serial) delay(10);
    }

    /**
     * @brief Write a message to the serial port
     * @param message The message to be written
     */
    template<typename T>
    void write(T message) {
        Serial.print(message);
    }

    /**
     * @brief Write a message to the serial port followed by a newline
     * @param message The message to be written
     */
    template<typename T>
    void writeLine(T message) {
        Serial.println(message);
    }
};

#endif