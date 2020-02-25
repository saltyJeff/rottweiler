//
// Created by saltyJeff on 2/19/2020.
//

#ifndef ROTTWEILER_GREENHERON_H
#define ROTTWEILER_GREENHERON_H


#include "Rotor.h"
#include <string>
#include <libserial/SerialPort.h>

const int ROTOR_TIMEOUT = 2 * 1000;

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
	static std::string posCmd(int val);
	const std::string getCmd = "R11;";
	const std::string stopCmd = ";";
	bool isBusy = false;
	void checkBusy();
};


#endif //ROTTWEILER_GREENHERON_H
