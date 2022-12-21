#ifndef MSH_H
#define MSH_H

#include <Arduino.h>
#include <Servo.h>


namespace Lib {
    typedef struct MotorSetting {
        bool available = false;
        uint8_t deg;
        uint16_t sleep;
    } MotorSetting;

    class ServoAlt: public Servo {
    public:
        void setupValues(uint8_t min, uint8_t max);
        void move(uint64_t procMillis, uint8_t deg, uint16_t sleep = 0);

    private:
        uint8_t min;
        uint8_t max;
        uint64_t process;

        void validateDegValue(uint8_t *deg);
    };
}

class ServoHandler {
public:
    Lib::ServoAlt* Motor;

    ServoHandler(uint8_t amount, uint8_t* minDeg, uint8_t* maxDeg);
    void attachAll(uint8_t* pins);
    void setRepeat(bool value);
    void setAlocatedSlots(uint8_t value);
    void detachAll(void);
    void set(uint8_t motor, uint8_t deg, uint16_t sleep);
    void load(void);
    void start(void);
    void update(uint64_t currMillis);

private:
    bool lock;
    bool repeat;
    uint8_t amt;
    uint8_t aSlots; // Alocated slots
    uint8_t sSlot;  // Current selected slot
    Lib::MotorSetting* MemSlot;
    Lib::MotorSetting** MoveSet;

    void applySetting(uint64_t currMillis);
};

#endif
