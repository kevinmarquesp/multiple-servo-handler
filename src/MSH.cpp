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
    this->MemSlot = new Motion[amt];

    this->amt = amt;

    this->amtSlots = 0;
    this->selectedSlot = 0;
    this->alocatedSlots = 0;

    for (uint8_t i = 0; i < amt; i++) 
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

void MSH::Handler::moveSlots(uint8_t mvAmt) {
    this->alocatedSlots = mvAmt;
    this->MoveSet = new Motion*[mvAmt];

    for (uint8_t i = 0; i < mvAmt; i++) {
        this->MoveSet[i] = new Motion[this->amt];
    }
}

void MSH::Handler::set(uint8_t motor, uint8_t deg, uint16_t sleep) {
    if (not this->lockSettings) {
        Motion setting;

        setting.available = true;
        setting.deg = deg;
        setting.sleep = sleep;

        this->MemSlot[motor] = setting;
    }
}

void MSH::Handler::load(void) {
    if (not this->lockSettings) {
        for (uint8_t i = 0; i < this->amt; i++)
            this->MoveSet[this->selectedSlot][i] = this->MemSlot[i];

        if (this->selectedSlot < this->alocatedSlots)
            this->selectedSlot++;
    }
}

void MSH::Handler::start(void) {
    this->lockSettings = true;
    this->amtSlots = this->selectedSlot + 1;
    this->selectedSlot = 0;
}

// End of  <MSH::Handler>
