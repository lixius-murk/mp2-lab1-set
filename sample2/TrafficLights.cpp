#include "TrafficLights.h"
// 3 бита: авто - зел, авто - жел, пеш - зел


TrafficLights::TrafficLights() :state(3), phase(0) { state.SetBit(0); };
bool TrafficLights::isCarRed() const { return !state.GetBit(0) && !state.GetBit(1); };
bool TrafficLights::isCarYellow() const { return state.GetBit(1); };
bool TrafficLights::isCarGreen() const { return state.GetBit(0); };

bool TrafficLights::isPedRed() const { return !state.GetBit(2); };
bool TrafficLights::isPedGreen() const { return state.GetBit(2); };

void TrafficLights::setCarState(COLORS color) {
	state.ClrBit(0);
	state.ClrBit(1);
	switch (color) {
	case GREEN: state.SetBit(0); setPedState(RED);  break;
	case RED: setPedState(GREEN);  break;
	case YELLOW: state.SetBit(1); setPedState(RED);  break;
	}
};

void TrafficLights::setPedState(COLORS color) {
	switch (color) {
	case GREEN: state.SetBit(2); break;
	case RED: state.ClrBit(2); break;
	case YELLOW: state.ClrBit(2); break;
	}

};
void TrafficLights::printState() const {
	cout << "Автомобиль: ";
	if (isCarGreen()) cout << "зеленый;";
	else if (isCarYellow()) cout << "желтый;";
	else if (isCarRed()) cout << "красный;";
	cout << endl;

	cout << "Пешеход: ";
	if (isPedGreen()) cout << "зеленый;";
	else cout << "красный;";
	cout << endl;
	cout << endl;


};
void TrafficLights::switchState() {
	phase = (phase + 1) % 3;

	switch (phase) {
	case 0:
		setCarState(GREEN);
		break;
	case 1:
		setCarState(YELLOW);
		break;
	case 2:
		setCarState(RED);
		break;
	}
};


void TrafficLights::clrState() {
	for (int i = 0; i < state.GetLength(); i++) {
		state.ClrBit(i);
	}
};