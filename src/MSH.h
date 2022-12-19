#ifndef MSH_H
#define MSH_H

#include <Arduino.h>
#include <Servo.h>


namespace MSH {
    class ServoAlt: public Servo {
    public:
        bool isDone;

        void setupValues(uint8_t min, uint8_t max);
        // void move(uint8_t deg, uint16_t sleep = 0);
    private:
        uint8_t min;
        uint8_t max;
    };

    class Handler {
    public:
        ServoAlt* Motor;

        Handler(uint8_t amount, uint8_t* minDeg, uint8_t* maxDeg);
        void attachAll(uint8_t* pins);
        void detachAll();
        void update(void);
        // void set(uint8_t mi, uint16_t sleep);
        // void start(void);

    private:
        uint8_t amount;
        // bool moveLock;
    };
} // namespace MSH

#endif
