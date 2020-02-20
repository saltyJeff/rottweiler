//
// Created by jeffe on 2/19/2020.
//

#ifndef ROTTWEILER_GREENHERON_H
#define ROTTWEILER_GREENHERON_H


#include "Rotor.h"
#include <string>
#include <libserial/SerialPort.h>
using namespace LibSerial;
class GreenHeron: public Rotor {
public:
	SerialPort azRotor;
	SerialPort elRotor;
	GreenHeron(std::string &azFile, std::string &elFile, int baudRate=9600);
	void setPosition(RotCoord &coords) override;
	RotCoord getPosition() override;
	void stop() override;
private:
	std::string posCmd(int val);
	bool safeCoords(RotCoord &coords);
	const std::string getCmd = "AI1;";
	const std::string stopCmd = ";";
};


#endif //ROTTWEILER_GREENHERON_H
