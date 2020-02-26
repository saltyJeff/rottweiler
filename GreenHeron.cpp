//
// Created by saltyJeff on 2/19/2020.
//

#include "GreenHeron.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <sstream>
#include "RotErrs.h"
GreenHeron::GreenHeron(std::string &azFile, std::string &elFile, int baudRate) {
	azRotor.SetBaudRate(static_cast<BaudRate>(baudRate));
	elRotor.SetBaudRate(static_cast<BaudRate>(baudRate));
	azRotor.Open(azFile);
	elRotor.Open(elFile);
	if(!azRotor.IsOpen()) {
		throw RotErr(RIG_ERRORS::RIG_ECONF, "Could not open azimuth rotor");
	}
	if(!elRotor.IsOpen()) {
		throw RotErr(RIG_ERRORS::RIG_ECONF, "Could not open elevation rotor");
	}
}
void GreenHeron::setPosition(RotCoord &coords) {
	checkBusy();
	std::string azCmd = posCmd(coords.az);
	std::string elCmd = posCmd(coords.el);

	azRotor.Write(azCmd);
	elRotor.Write(elCmd);
}
RotCoord GreenHeron::getPosition() {
	azRotor.Write(getCmd);
	elRotor.Write(getCmd);
	std::string azRead;
	std::string elRead;
	try {
		azRotor.Read(azRead, 4, ROTOR_TIMEOUT);
		elRotor.Read(elRead, 4, ROTOR_TIMEOUT);
		int az, el;
		int azBusy, elBusy;
		char throwaway;
		std::istringstream azStream (azRead);
		std::istringstream elStream (elRead);
		// structure of a message: SOH ascii0 NULL|SOH ascii> %3d value;
		azStream >> az;
		azStream >> azBusy;
		elStream >> el;
		elStream >> elBusy;
		isBusy = azBusy == 1 || elBusy == 1;
		return RotCoord(az, el);
	}
	catch(ReadTimeout &e) {
		spdlog::error(std::string("Timeout while reading from rotor: ") + e.what());
		throw RotErr(RIG_ERRORS::RIG_ETIMEOUT, std::string("Timeout while reading position from rotor") + e.what());
	}
}
void GreenHeron::stop() {
	azRotor.Write(stopCmd);
	elRotor.Write(stopCmd);
}
char commandBuffer[255];
std::string GreenHeron::posCmd(int val) {
	sprintf(commandBuffer, "AP1%03d\r;", val);
	return std::string(commandBuffer);
}

void GreenHeron::checkBusy() {
	if(!isBusy) {
		return;
	}
	getPosition();
	if(isBusy) {
		throw RotErr(RIG_ERRORS::RIG_BUSBUSY, std::string("Rotor is still moving"));
	}
}
