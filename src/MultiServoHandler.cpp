#include "MultiServoHandler.h"

void MultiServoHandler::_validadeDegValue(uint8_t* deg, uint8_t index) {
    if (*deg < this->_minDeg[index])
        *deg = this->_minDeg[index];

    else if (*deg > this->_maxDeg[index])
        *deg = this->_maxDeg[index];
}

void MultiServoHandler::config(uint8_t amount, uint8_t* pins, uint8_t* minDeg, uint8_t* maxDeg) {
    this->_minDeg = new uint8_t[amount];
    this->_maxDeg = new uint8_t[amount];
    this->_tasks = new uint64_t[amount];

    this->_currPos = new uint8_t[amount];
    this->_lock = new bool[amount];
    this->_amount = amount;

    this->Motor = new Servo[amount];

    for (uint8_t i = 0; i < amount; i++) {
        this->_minDeg[i] = minDeg[i];
        this->_maxDeg[i] = maxDeg[i];

        this->Motor[i].attach(pins[i]);
        this->_currPos[i] = minDeg[i];
    }

    this->loopUpdate();
}

void MultiServoHandler::loopUpdate(void) {
    this->_cMill = millis();

    for (uint8_t i = 0; i < this->_amount; i++) {
        this->Motor[i].write(this->_currPos[i]);
    }
}

void MultiServoHandler::move(uint8_t index, uint8_t deg, uint16_t sleep = 0) {
    this->_validadeDegValue(&deg, index);

    if (not sleep or deg == this->_currPos[index]) {
        return;
    }

    if (this->_cMill - this->_tasks[index] >= sleep) {
        this->_tasks[index] = this->_cMill;

        if (deg > this->_currPos[index])
            this->_currPos[index]++;

        else if (deg < this->_currPos[index])
            this->_currPos[index]--;

    }
}
