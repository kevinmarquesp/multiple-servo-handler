#define AMOUNT 6
#include "src/MultiServoHandler.h"

uint8_t i;

uint8_t atPins[AMOUNT] = {  4,   5,   6,   7,   8,   9};
uint8_t maxDeg[AMOUNT] = {180, 180, 180, 180, 180, 180};
uint8_t minDeg[AMOUNT] = {  0,   0,   0,   0,   0,   0};

MultiServoHandler Servos;

void run(void);
void display(void);

void setup(void) {
    Serial.begin(9600);
    Servos.config(AMOUNT, atPins, minDeg, maxDeg);
}

void loop(void) {
    Servos.loopUpdate();
    run();
    display();
}

void run(void) {
    Servos.move(0, 90, 100);
}

void display(void) {
    for (i = 0; i < AMOUNT; i++) {
        Serial.print("m");
        Serial.print(i);
        Serial.print(":");
        Serial.print(Servos.Motor[i].read());
        Serial.print("\t");
    }
    Serial.println();
}
