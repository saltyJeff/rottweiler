//
// Created by jeffe on 2/19/2020.
//

#include "GreenHeron.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
GreenHeron::GreenHeron(std::string &azFile, std::string &elFile, int baudRate) {
	azRotor.SetBaudRate(static_cast<BaudRate>(baudRate));
	elRotor.SetBaudRate(static_cast<BaudRate>(baudRate));
	azRotor.Open(azFile);
	elRotor.Open(elFile);
	if(!azRotor.IsOpen()) {
		throw std::runtime_error("Could not open azimuth rotor");
	}
	if(!elRotor.IsOpen()) {
		throw std::runtime_error("Could not open elevation rotor");
	}
}
void GreenHeron::setPosition(RotCoord &coords) {
	std::string azCmd = posCmd(coords.az);
	std::string elCmd = posCmd(coords.el);

	azRotor.Write(azCmd);
	elRotor.Write(elCmd);
}
RotCoord GreenHeron::getPosition() {
	azRotor.Write(getCmd);
	elRotor.Write(getCmd);
	std::string azRead = "TIMEOUT";
	std::string elRead = "TIMEOUT";
	azRotor.Read(azRead, 4);
	elRotor.Read(elRead, 4);
	if(azRead == "TIMEOUT" || elRead == "TIMEOUT") {
		spdlog::error("Timeout when reading from the green heron. Rotor is now in undefined state");
	}
	int az, el;
	sscanf(azRead.c_str(), "%d;", &az);
	sscanf(elRead.c_str(), "%d;", &el);
	return RotCoord(az, el);
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