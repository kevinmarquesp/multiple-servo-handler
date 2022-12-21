#include "MSH.h"

// <MSH::ServoAlt>

// public methods

void Lib::ServoAlt::setupValues(uint8_t min, uint8_t max) {
    this->min = min;
    this->max = max;
    this->write(min);
}

void Lib::ServoAlt::move(uint64_t procMillis, uint8_t deg, uint16_t sleep) {
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

void Lib::ServoAlt::validateDegValue(uint8_t* deg) {
    if (*deg < this->min)
        *deg = this->min;
    else if (*deg > this->max)
        *deg = this->max;
}

// End of <MSH::ServoAlt>

// <ServoHandler> -> constructor

ServoHandler::ServoHandler(uint8_t amt, uint8_t* minDeg, uint8_t* maxDeg) {
    this->amt = amt;

    this->Motor = new Lib::ServoAlt[amt];
    this->MemSlot = new Lib::MotorSetting[amt];

    for (uint8_t i = 0; i < amt; i++) 
        this->Motor[i].setupValues(minDeg[i], maxDeg[i]);
}

// public methods

void ServoHandler::attachAll(uint8_t* pins) {
    for (uint8_t i = 0; i < this->amt; i++)
        this->Motor[i].attach(pins[i]);
}

void ServoHandler::detachAll(void) {
    uint8_t i;
    for (uint8_t i = 0; i < this->amt; i++)
        this->Motor[i].detach();
}

void ServoHandler::setAlocatedSlots(uint8_t value) {
    this->aSlots = value;
    this->MoveSet = new Lib::MotorSetting*[value];

    for (uint8_t i = 0; i < value; i++)
        this->MoveSet[i] = new Lib::MotorSetting[this->amt];
}

void ServoHandler::set(uint8_t motor, uint8_t deg, uint16_t sleep) {
    if (this->lock)
        return;

    Lib::MotorSetting setting;

    setting.available = true;
    setting.deg = deg;
    setting.sleep = sleep;

    this->MemSlot[motor] = setting;
}

void ServoHandler::load(void) {
    if (this->lock or this->sSlot >= this->aSlots)
        return;

    for (uint8_t i = 0; i < this->amt; i++) {
        this->MoveSet[this->sSlot][i] = this->MemSlot[i];
        this->MemSlot[i].available = false;
    }

    this->sSlot++;
}

void ServoHandler::start(void) {
    this->lock = true;
    this->aSlots = this->sSlot;
    this->sSlot = 0;
}

void ServoHandler::update(uint64_t currMillis) {
    if (not this->lock)
        return;

    if (this->sSlot == this->aSlots) {
        if (this->repeat)
            this->sSlot = 0;
        else
            return;
    }

    this->applySetting(currMillis);
}

void ServoHandler::setRepeat(bool value) {
    this->repeat = value;
}

// private methods

void ServoHandler::applySetting(uint64_t currMillis) {
    Lib::MotorSetting* currSet = this->MoveSet[this->sSlot];

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
        if (this->sSlot < this->aSlots)
            this->sSlot++;
}

// End of  <ServoHandler>
