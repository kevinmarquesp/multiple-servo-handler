#include "MSH.h"

// <MSH::ServoAlt>

// public methods

void MSH::ServoAlt::setupValues(uint8_t min, uint8_t max) {
    this->min = min;
    this->max = max;
    this->write(min);
}

void MSH::ServoAlt::move(uint64_t procMillis, uint8_t deg, uint16_t sleep) {
    this->validateDegValue(&deg);

    if (not sleep)
        this->write(deg);

    uint8_t currPos = this->read();

    if (deg == currPos)
        return;

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
    this->amt = amt;

    this->Motor = new ServoAlt[amt];
    this->MemSlot = new Motion[amt];

    for (uint8_t i = 0; i < amt; i++) 
        this->Motor[i].setupValues(minDeg[i], maxDeg[i]);
}

// public methods

void MSH::Handler::attachAll(uint8_t* pins) {
    for (uint8_t i = 0; i < this->amt; i++)
        this->Motor[i].attach(pins[i]);
}

void MSH::Handler::detachAll(void) {
    uint8_t i;
    for (uint8_t i = 0; i < this->amt; i++)
        this->Motor[i].detach();
}

void MSH::Handler::moveSlots(uint8_t mvAmt) {
    this->alocatedSlots = mvAmt;
    this->MoveSet = new Motion*[mvAmt];

    for (uint8_t i = 0; i < mvAmt; i++)
        this->MoveSet[i] = new Motion[this->amt];
}

void MSH::Handler::set(uint8_t motor, uint8_t deg, uint16_t sleep) {
    if (this->lockSettings)
        return;

    Motion setting;

    setting.available = true;
    setting.deg = deg;
    setting.sleep = sleep;

    this->MemSlot[motor] = setting;
}

void MSH::Handler::load(void) {
    if (this->lockSettings or this->selectedSlot >= this->alocatedSlots)
        return;

    for (uint8_t i = 0; i < this->amt; i++) {
        this->MoveSet[this->selectedSlot][i] = this->MemSlot[i];
        this->MemSlot[i].available = false;
    }

    this->selectedSlot++;
}

void MSH::Handler::isReady(void) {
    this->lockSettings = true;
    this->alocatedSlots = this->selectedSlot;
    this->selectedSlot = 0;
}

void MSH::Handler::update(uint64_t currMillis) {
    if (not this->lockSettings)
        return;

    if (this->selectedSlot == this->alocatedSlots) {
        if (this->repeat)
            this->selectedSlot = 0;
        else
            return;
    }

    this->applySetting();
}

void MSH::Handler::setRepeat(bool value) {
    this->repeat = value;
}

// private methods

void MSH::Handler::applySetting(void) {
    Motion* currSet = this->MoveSet[this->selectedSlot];

    uint8_t availableList = 0;
    uint8_t doneList = 0;
    uint8_t i;

    // Count how many servos are available to move in the setting array
    for (i = 0; i < this->amt; i++)
        if (currSet[i].available)
            availableList++;

    for (i = 0; i < this->amt; i++) {
        if (currSet[i].available) {
            if (this->Motor[i].read() == currSet[i].deg) // If this servo is done
                doneList++;
            else
                this->Motor[i].move(currMillis, currSet[i].deg, currSet[i].sleep);
        }
    }

    // If every servo available was done
    if (availableList == doneList)
        if (this->selectedSlot < this->alocatedSlots)
            this->selectedSlot++;
}

// End of  <MSH::Handler>
