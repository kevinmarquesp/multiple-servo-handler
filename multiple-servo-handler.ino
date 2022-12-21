#define AMOUNT 6
#define MOVES 4

#include "src/MSH.h"

uint8_t maxDeg[AMOUNT] = {180, 180, 180, 180, 180, 180};
uint8_t minDeg[AMOUNT] = {  0,   0,   0,   0,   0,   0};
uint8_t pins[AMOUNT] =   {  4,   5,   6,   7,   8,   9};

ServoHandler Bumblebee(AMOUNT, minDeg, maxDeg);

void debug_display(void);
void MSH_BumblebeeSettings(void);

void setup(void) {
    Serial.begin(9600);

    Bumblebee.attachAll(pins);
    Bumblebee.setAlocatedSlots(MOVES);
    Bumblebee.setRepeat(false);

    MSH_BumblebeeSettings();
    Bumblebee.start();
}

void loop(void) {
    Bumblebee.update(millis());
    debug_display();
}

void MSH_BumblebeeSettings(void) {
    Bumblebee.set(0, 90, 10);
    Bumblebee.set(1, 90, 50);
    Bumblebee.set(2, 90, 100);
    Bumblebee.load();
    
    Bumblebee.set(5, 90, 10);
    Bumblebee.set(4, 90, 50);
    Bumblebee.set(3, 90, 100);
    Bumblebee.load();
    
    Bumblebee.set(0, 180, 5);
    Bumblebee.set(1, 180, 5);
    Bumblebee.set(2, 180, 5);
    Bumblebee.set(3, 180, 5);
    Bumblebee.set(4, 180, 5);
    Bumblebee.set(5, 180, 5);
    Bumblebee.load();
    
    Bumblebee.set(0, 0, 10);
    Bumblebee.set(1, 0, 50);
    Bumblebee.set(2, 0, 100);
    Bumblebee.set(3, 0, 100);
    Bumblebee.set(4, 0, 50);
    Bumblebee.set(5, 0, 10);
    Bumblebee.load();
}

void debug_display(void) {
    for (uint8_t i = 0; i < AMOUNT; i++) {
        Serial.print("(");
        Serial.print(i);
        Serial.print(") ");
        Serial.print(Bumblebee.Motor[i].read());
        Serial.print("\t");
    }
    Serial.println();
}
