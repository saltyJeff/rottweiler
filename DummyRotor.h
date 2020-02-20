//
// Created by jeffe on 2/19/2020.
//

#ifndef ROTTWEILER_DUMMYROTOR_H
#define ROTTWEILER_DUMMYROTOR_H

#include "Rotor.h"

class DummyRotor: public Rotor {
public:
	void setPosition(RotCoord &coords) override;
	RotCoord getPosition() override;
	void stop() override {};
private:
	RotCoord state;
};

#endif //ROTTWEILER_DUMMYROTOR_H
