//Zumbador.h
#ifndef ZUMBA_H
#define ZUMBA_H

/**
 * @class Zumbador
 * @brief A class to control a buzzer connected to the Arduino
 * 
 * This class provides methods to initialize, activate, deactivate, 
 * and update the state of a buzzer that plays a melody. It controls 
 * timing between notes and loops through a predefined melody.
 * 
 * @author Alejandro Rosado Jiménez
 * @date 2024-11-08
 * last updated: 2024-11-09
 */
class Zumbador {
private:
    int pin;
    bool isPlaying;
    unsigned long lastNoteTime;
    int currentNote;

    const int melody[8] = {
        262, 294, 330, 349, 392, 440, 494, 523
    };
    
    const int noteDurations[8] = {
        300, 300, 300, 300, 300, 300, 300, 600
    };

public:
    /**
     * @brief Constructor for Zumbador
     * 
     * Initializes the buzzer with the specified pin number and
     * sets the initial state to inactive.
     * 
     * @param pin The pin number for the buzzer
     */
    Zumbador(int pin) : pin(pin), isPlaying(false), lastNoteTime(0), currentNote(0) {}

    /**
     * @brief Initialize the buzzer
     * 
     * Sets up the pin as an output and prepares the buzzer to play
     * without starting the melody.
     */
    void iniciar() {
        pinMode(pin, OUTPUT);
        isPlaying = false;
        currentNote = 0;
    }

    /**
     * @brief Activate the buzzer
     * 
     * Starts playing the melody by setting the initial note 
     * and marking the buzzer as active.
     */
    void activar() {
        isPlaying = true;
        currentNote = 0;
        lastNoteTime = millis();
    }

    /**
     * @brief Deactivate the buzzer
     * 
     * Stops the buzzer by turning off the tone and marking
     * the buzzer as inactive.
     */
    void desactivar() {
        isPlaying = false;
        noTone(pin);
    }

    /**
     * @brief Update the buzzer's state
     * 
     * Checks if it's time to move to the next note in the melody. 
     * If the buzzer is active, this method advances to the next 
     * note based on the timing specified in noteDurations.
     */
    void update() {
        if (!isPlaying) return;

        unsigned long currentTime = millis();
        
        
        if (currentTime - lastNoteTime >= noteDurations[currentNote]) {
            noTone(pin);  
            
            currentNote = (currentNote + 1) % 8;  
            tone(pin, melody[currentNote]);  
            lastNoteTime = currentTime;  
        }
    }

    /**
     * @brief Check if the buzzer is active
     * 
     * @return true if the buzzer is currently playing, false otherwise
     */
    bool estaActivo() {
        return isPlaying;
    }
};

#endif