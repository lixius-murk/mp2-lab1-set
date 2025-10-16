#include "TrafficLights.h"
#include "CrossRoad.h"

void main() {
	setlocale(LC_ALL, "Russian");
	TrafficLights tr;
	CrossRoad cr(&tr);
	cr.runCR();

}