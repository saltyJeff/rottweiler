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
		azRotor.ReadLine(azRead, ';', ROTOR_TIMEOUT);		
		elRotor.ReadLine(elRead, ';', ROTOR_TIMEOUT);
		
		std::pair<float, bool> azRes = readGetResponse(azRotor);
		std::pair<float, bool> elRes = readGetResponse(elRotor);

		isBusy = azRes.second || elRes.second;
		return RotCoord(azRes.first, elRes.first);
	}
	catch(ReadTimeout &e) {
		spdlog::error(std::string("Timeout while reading from rotor: ") + e.what());
		throw RotErr(RIG_ERRORS::RIG_ETIMEOUT, std::string("Timeout while reading position from rotor") + e.what());
	}
}
// structure of a message: SOH ascii0 NULL|SOH ascii> %3d value;
std::pair<float, bool> GreenHeron::readGetResponse(SerialPort &rotor) {
	std::string resLine;
	rotor.ReadLine(resLine, ';', ROTOR_TIMEOUT);
	char c; // throwaway
	int i; // throwaway

	float heading;
	bool busy;
	std::istringstream stream(resLine);
	stream >> c; // get rid of SOH
	stream >> i; // always 0
	stream >> c; // SOH or NULL
	busy = c == 1;
	stream >> c; // get rid of >
	stream >> heading;

	if(stream.fail()) {
		throw RotErr(RIG_ERRORS::RIG_ERJCTED, std::string("Response data non-conformant: ")+resLine);
	}
	return std::make_pair(heading, busy);
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
