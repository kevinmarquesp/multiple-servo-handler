#include "MultiServoHandler.h"

MultiServoHandler::config(uint8_t amount, uint8_t* pins, uint8_t* minDeg, uint8_t* maxDeg) {
    this->_amount = amount;
    this->_minDeg = new uint8_t[amount];
    this->_maxDeg = new uint8_t[amount];
    this->_tasks = new uint64_t[amount];
    this->Motor = new Servo[amount];

    for (uint8_t i = 0; i < amount; i++) {
        this->_minDeg[i] = minDeg[i];
        this->_maxDeg[i] = maxDeg[i];

        this->Motor[i].attach(pins[i]);
        this->Motor[i].write(minDeg[i]);
    }
}

MultiServoHandler::loopUpdate(void) {
    this->_time = millis();
}

MultiServoHandler::move(uint8_t index, uint8_t degValue, uint16_t waitDelay = 0) {
    degValue = degValue < this->_minDeg[index] ? this->_minDeg[index] : degValue;
    degValue = degValue > this->_maxDeg[index] ? this->_maxDeg[index] : degValue;

    uint8_t currPos = this->Motor[index].read();

    if (not waitDelay or degValue == currPos)
        return;

    if (degValue > currPos) {
        for (currPos; currPos <= degValue; currPos++) {
            this->Motor[index].write(currPos);
            delay(waitDelay);
        }

    } else if (degValue < currPos) {
        for (currPos; currPos >= degValue; currPos--) {
            this->Motor[index].write(currPos);
            delay(waitDelay);
        }
    }
}
