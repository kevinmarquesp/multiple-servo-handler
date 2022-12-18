#ifndef MULTISERVOHANDLER_H
#define MULTISERVOHANDLER_H

#include <Arduino.h>
#include <Servo.h>

class MultiServoHandler {
private:
    uint8_t* _minDeg;
    uint8_t* _maxDeg;
    uint64_t* _tasks;

    uint8_t _amount;
    uint64_t _time;

public:
    Servo* Motor;

    config(uint8_t, uint8_t*, uint8_t*, uint8_t*);
    loopUpdate(void);
    move(uint8_t, uint8_t, uint16_t);
};

#endif
