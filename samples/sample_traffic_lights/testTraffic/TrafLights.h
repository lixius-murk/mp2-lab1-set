#pragma once


#include "tbitfield.h"

enum COLORS { RED, GREEN, YELLOW };

class TrafficLights {
    TBitField state;
    int phase;
public:
    TrafficLights();// 3 бита: авто - зел, авто - жел, пеш - зел
    bool isCarRed() const;
    bool isCarYellow() const;
    bool isCarGreen() const;

    bool isPedRed() const;
    bool isPedGreen() const;

    void setCarState(COLORS color);

    void setPedState(COLORS color);
    void printState() const;
    void switchState();
    void clrState();
};