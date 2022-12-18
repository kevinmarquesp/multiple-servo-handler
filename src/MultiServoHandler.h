#ifndef MULTISERVOHANDLER_H
#define MULTISERVOHANDLER_H

#include <Arduino.h>
#include <Servo.h>

class MultiServoHandler {
private:
    uint8_t* _minDeg;
    uint8_t* _maxDeg;
    uint64_t* _tasks;
    uint64_t _cMill;

    void _validadeDegValue(uint8_t*, uint8_t);
    void _waitTask(uint64_t*, uint16_t, void (*)(void));

public:
    Servo* Motor;

    void config(uint8_t, uint8_t*, uint8_t*, uint8_t*);
    void loopUpdate(void);
    void move(uint8_t, uint8_t, uint16_t);
};

#endif
