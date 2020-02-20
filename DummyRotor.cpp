//
// Created by jeffe on 2/19/2020.
//

#include "DummyRotor.h"

void DummyRotor::setPosition(RotCoord &coords) {
	state = coords;
}

RotCoord DummyRotor::getPosition() {
	return state;
}