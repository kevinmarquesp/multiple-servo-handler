#include "MSH.h"

// <MSH::ServoAlt>

// public methods

void MSH::ServoAlt::setupValues(uint8_t min, uint8_t max) {
    this->min = min;
    this->max = max;

    this->isDone = true;
    this->process = 0;

    this->write(min);
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

// private methods

void MSH::ServoAlt::validateDegValue(uint8_t* deg) {
    if (*deg < this->min)
        *deg = this->min;

    else if (*deg > this->max)
        *deg = this->max;
}

// End of <MSH::ServoAlt>

// <MSH::Handler> -> constructor

MSH::Handler::Handler(uint8_t amt, uint8_t* minDeg, uint8_t* maxDeg) {
    this->Motor = new ServoAlt[amt];
    this->amt = amt;

    for (uint8_t i = 0; i < amt; ++i)
        this->Motor[i].setupValues(minDeg[i], maxDeg[i]);
}

// public methods

void MSH::Handler::attachAll(uint8_t* pins) {
    for (uint8_t i = 0; i < this->amt; i++) {
        this->Motor[i].attach(pins[i]);
    }
}

void MSH::Handler::detachAll(void) {
    for (uint8_t i = 0; i < this->amt; i++) {
        this->Motor[i].detach();
    }
}

// End of  <MSH::Handler>
