#include "TrafLights.h"


void main() {
    setlocale(LC_ALL, "Russian");
    TrafficLights traflights;

    for (int i = 0; i < 8; i++) {

        traflights.switchState();
        traflights.printState();
    }



}