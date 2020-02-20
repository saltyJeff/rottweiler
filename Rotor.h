//
// Created by jeffe on 2/19/2020.
//

#ifndef ROTTWEILER_ROTOR_H
#define ROTTWEILER_ROTOR_H

#include "RotCoord.h"

class Rotor {
public:
	virtual ~Rotor() {};
	virtual void setPosition(RotCoord &coords) = 0;
	virtual RotCoord getPosition() = 0;
	virtual void stop() = 0;
};

#endif //ROTTWEILER_ROTOR_H
