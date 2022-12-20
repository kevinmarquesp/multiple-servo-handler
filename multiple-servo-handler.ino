#define AMOUNT 6
#include "src/MSH.h"

uint8_t maxDeg[AMOUNT] = {180, 180, 180, 180, 180, 180};
uint8_t minDeg[AMOUNT] = { 20,  20,  20,  20,  20,  20};
uint8_t pins[AMOUNT] =   {  4,   5,   6,   7,   8,   9};

MSH::Handler Arm(AMOUNT, minDeg, maxDeg);

void debug_display(void);

void setup(void) {
    Serial.begin(9600);

    // -----------------
    Arm.attachAll(pins);
    Arm.moveSlots(16);
    // -----------------
    Arm.set(0, 90, 100);
    Arm.set(1, 90, 100);
    Arm.load();

    Arm.set(2, 90, 100);
    Arm.load();
    // -----------------
    Arm.start();
    // -----------------
}

void loop(void) {
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
