//Zumbador.h
#ifndef ZUMBA_H
#define ZUMBA_H

/**
 * @class Zumbador
 * @brief A class to control a buzzer connected to the Arduino
 * 
 * 
 * 
 * @author Alejandro Rosado Jiméez
 * @date 2024-11-08
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
     * 
     * 
     * @param pin The pin number for the buzzer
     */
    Zumbador(int pin) : pin(pin), isPlaying(false), lastNoteTime(0), currentNote(0) {}

    /**
     * @brief Initialize the buzzer
     * 
     * 
     */
    void iniciar() {
        pinMode(pin, OUTPUT);
        isPlaying = false;
        currentNote = 0;
    }

    /**
     * @brief Activate the buzzer
     * 
     * 
     *
     */
    void activar() {
        isPlaying = true;
        currentNote = 0;
        lastNoteTime = millis();
    }
    void desactivar() {
        isPlaying = false;
        noTone(pin);
    }

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

    bool estaActivo() {
        return isPlaying;
    }
};

#endif