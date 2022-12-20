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
        for (uint8_t i = 0; i < this->amt; i++) {
            this->MoveSet[this->selectedSlot][i] = this->MemSlot[i];
            this->MemSlot[i].available = false;
        }

        this->selectedSlot++;
    }
}

void MSH::Handler::isReady(void) {
    this->lockSettings = true;
    this->amtSlots = this->selectedSlot;
    this->selectedSlot = 0;
}

void MSH::Handler::update(uint64_t currMillis) {
    if (this->lockSettings and this->selectedSlot < this->amtSlots) {
        uint8_t availableList = 0;
        uint8_t doneList = 0;
        Motion* currSet = this->MoveSet[this->selectedSlot];

        for (uint8_t i = 0; i < this->amt; i++)
            if (currSet[i].available)
                availableList++;

        for (uint8_t i = 0; i < this->amt; i++) {
            if (currSet[i].available) {
                if (this->Motor[i].read() == currSet[i].deg)
                    doneList++;
                else
                    this->Motor[i].move(currMillis, currSet[i].deg, currSet[i].sleep);
            }
        }

        if (availableList == doneList)
            this->selectedSlot++;
    }
}

// End of  <MSH::Handler>
