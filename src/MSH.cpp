#include "MSH.h"

void MSH::ServoAlt::setupValues(uint8_t min, uint8_t max) {
    this->min = min;
    this->max = max;
    this->isDone = true;

    this->write(min);
}

MSH::Handler::Handler(uint8_t amount, uint8_t* minDeg, uint8_t* maxDeg) {
    this->amount = amount;
    this->Motor = new ServoAlt[amount];

    for (uint8_t i = 0; i < amount; i++)
        this->Motor[i].setupValues(minDeg[i], maxDeg[i]);
}

void MSH::Handler::attachAll(uint8_t* pins) {
    for (uint8_t i = 0; i < this->amount; i++) {
        this->Motor[i].attach(pins[i]);
    }
}

void MSH::Handler::detachAll() {
    for (uint8_t i = 0; i < this->amount; i++) {
        this->Motor[i].detach();
    }
}
