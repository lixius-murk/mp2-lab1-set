#include "CrossRoad.h"


void CrossRoad::runCR() {
    for (int i = 0; i < 8; i++) {
        light.printState();
        light.switchState();
    }
}