#define AMOUNT 3
#define MOVES 2

#include "src/MSH.h"

uint8_t maxDeg[AMOUNT] = {180, 180, 180};
uint8_t minDeg[AMOUNT] = {  0,   0,   0};
uint8_t pins[AMOUNT] =   {  4,   5,   6};

MSH::Handler Arm(AMOUNT, minDeg, maxDeg);

void debug_display(void);

void setup(void) {
    Serial.begin(9600);

    // -----------------
    Arm.attachAll(pins);
    Arm.moveSlots(MOVES);
    // -----------------
    Arm.set(0, 180, 5);
    Arm.set(1, 180, 10);
    Arm.set(2, 180, 5);
    Arm.load();
    Arm.set(0, 0, 5);
    Arm.set(1, 0, 10);
    Arm.set(2, 0, 5);
    Arm.load();
    // -----------------
    Arm.isReady();
    Arm.setRepeat(true);
    // -----------------
}

void loop(void) {
    Arm.update(millis());
    debug_display();
}

void debug_display(void) {
    for (uint8_t i = 0; i < AMOUNT; i++) {
        Serial.print("[");
        Serial.print(i);
        Serial.print("] ");
        Serial.print(Arm.Motor[i].read());
        Serial.print("\t");
    }
    Serial.println();
}
