#include "MultiServoHandler.h"

void MultiServoHandler::_validadeDegValue(uint8_t* deg, uint8_t index) {
    if (*deg < this->_minDeg[index])
        *deg = this->_minDeg[index];

    else if (*deg > this->_maxDeg[index])
        *deg = this->_maxDeg[index];
}

void MultiServoHandler::_waitTask(uint64_t* task, uint16_t sleep, void (*exec)(void)) {
    if (this->_cMill - *task >= sleep) {
        *task = this->_cMill;
        exec();
    }
}

void MultiServoHandler::config(uint8_t amount, uint8_t* pins, uint8_t* minDeg, uint8_t* maxDeg) {
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

void MultiServoHandler::loopUpdate(void) {
    this->_cMill = millis();
}

void MultiServoHandler::move(uint8_t index, uint8_t deg, uint16_t sleep = 0) {
    this->_validadeDegValue(&deg, index);
    uint8_t cPos = this->Motor[index].read();

    if (not sleep or deg == cPos)
        return;

    if (this->_cMill - this->_tasks[index] >= sleep) {

        this->_tasks[index] = this->_cMill;
        this->Motor[index].write(cPos);

        if (deg > cPos) {
            this->Motor[index].write(cPos);
            cPos++;

        } else if (deg < cPos) {
            cPos--;
        }

    }

    /* if (deg > cPos) { */
    /*     for (cPos; cPos <= deg; cPos++) { */
    /*         this->Motor[index].write(cPos); */
    /*         delay(waitDelay); */
    /*     } */

    /* } else if (deg < cPos) { */
    /*     for (cPos; cPos >= deg; cPos--) { */
    /*         this->Motor[index].write(cPos); */
    /*         delay(waitDelay); */
    /*     } */
    /* } */
}
