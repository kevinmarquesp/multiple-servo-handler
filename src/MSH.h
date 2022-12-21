#ifndef MSH_H
#define MSH_H

#include <Arduino.h>
#include <Servo.h>


namespace MSH {
    typedef struct Motion {
        bool available = false;
        uint8_t deg;
        uint16_t sleep;
    } Motion;

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

    class Handler {
    public:
        ServoAlt* Motor;

        Handler(uint8_t amount, uint8_t* minDeg, uint8_t* maxDeg);
        void attachAll(uint8_t* pins);
        void detachAll();
        void moveSlots(uint8_t mvAmt);
        void set(uint8_t motor, uint8_t deg, uint16_t sleep);
        void load(void);
        void isReady(void);
        void update(uint64_t currMillis);
        void setRepeat(bool value);

    private:
        bool lockSettings;
        bool repeat;
        uint8_t amt;
        uint8_t alocatedSlots;
        uint8_t selectedSlot;
        Motion* MemSlot;
        Motion** MoveSet;

        void applySetting(void);
    };
} // namespace MSH

#endif
