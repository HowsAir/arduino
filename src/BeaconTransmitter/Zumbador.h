//Zumbador.h
#ifndef ZUMBA_H
#define ZUMBA_H

/**
 * @class Zumbador
 * @brief A class to control a buzzer connected to the Arduino
 * 
 * This class provides methods to initialize and activate the buzzer
 * when triggered by a BLE command from the Android device.
 * 
 * @author Alejandro Rosado Jiméez
 * @date 2024-11-08
 */
class Zumbador {
private:
    int pin; /**< The pin where the buzzer is connected */

public:
    /**
     * @brief Constructor for Zumbador
     * 
     * This constructor initializes the buzzer object with the pin where it is connected.
     * 
     * @param pin The pin number for the buzzer
     */
    Zumbador(int pin) : pin(pin) {}

    /**
     * @brief Initialize the buzzer
     * 
     * This method configures the pin connected to the buzzer as an output pin.
     */
    void iniciar() {
        pinMode(pin, OUTPUT);  // Set the pin as output
    }

    /**
     * @brief Activate the buzzer
     * 
     * This method activates the buzzer for 1 second and then deactivates it.
     * 
     * It writes HIGH to the buzzer pin for 1 second, then sets it to LOW.
     */
    void activar() {
    // Definimos las frecuencias de las notas
    int melody[] = {
        262, 294, 330, 349, 392, 440, 494, 523
    };
    
    // Definimos la duración de cada nota en milisegundos
    int noteDurations[] = {
        300, 300, 300, 300, 300, 300, 300, 600
    };

    // Reproducir cada nota en el zumbador
    for (int i = 0; i < 8; i++) {
        tone(pin, melody[i]);                 // Toca la nota actual en el zumbador
        delay(noteDurations[i]);              // Espera el tiempo de duración de la nota
        noTone(pin);                          // Apaga el sonido
    }
}

};

#endif