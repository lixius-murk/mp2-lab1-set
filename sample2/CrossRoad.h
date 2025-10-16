#pragma once

#include "TrafficLights.h"


class CrossRoad {
	TrafficLights *light;
public:
	CrossRoad(TrafficLights* tr);
	void runCR();

};