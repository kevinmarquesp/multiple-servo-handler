#include "MSH.h"

void MSH::ServoAlt::setupValues(uint8_t min, uint8_t max) {
    this->min = min;
    this->max = max;
    this->isDone = true;
    this->process = 0;

    this->write(min);
}

void MSH::ServoAlt::validateDegValue(uint8_t* deg) {
    if (*deg < this->min)
        *deg = this->min;

    else if (*deg > this->max)
        *deg = this->max;
}

void MSH::ServoAlt::move(uint64_t procMillis, uint8_t deg, uint16_t sleep = 0) {
    this->isDone = false;

    this->validateDegValue(&deg);
    uint8_t currPos = this->read();

    if (not sleep) {
        this->write(deg);
        return;

    } else if (deg == currPos) {
        this->isDone = true;
        return;
    }

    if (procMillis - this->process >= sleep) {
        this->process = procMillis;

        if (deg > currPos)
            currPos++;

        else if (deg < currPos)
            currPos--;

        this->write(currPos);
    }
}

MSH::Handler::Handler(uint8_t amount, uint8_t* minDeg, uint8_t* maxDeg) {
    this->amount = amount;
    // this->moveLock = false;

    this->moveSet = new uint8_t[amount];
    this->Motor = new ServoAlt[amount];

    for (uint8_t i = 0; i < amount; i++) {
        this->Motor[i].setupValues(minDeg[i], maxDeg[i]);
        this->moveSet[i] = minDeg[i];
    }
}

void MSH::Handler::attachAll(uint8_t* pins) {
    for (uint8_t i = 0; i < this->amount; i++) {
        this->Motor[i].attach(pins[i]);
    }
}

void MSH::Handler::detachAll(void) {
    for (uint8_t i = 0; i < this->amount; i++) {
        this->Motor[i].detach();
    }
}

void MSH::Handler::update(uint64_t procMillis) {
    if (not this->moveLock) {
        for (uint8_t i = 0; i < this->amount; i++) {
            this->Motor[i].move(procMillis, this->moveSet[i], 100);
        }
    }
}
