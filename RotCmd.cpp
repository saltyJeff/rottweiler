//
// Created by saltyJeff on 2/19/2020.
//

#include "RotCmd.h"
#include <stdexcept>
#include <sstream>
#include "RotErrs.h"

RotCmd::RotCmd(std::string &cmdStr) {
	this->cmdStr = cmdStr;
	std::istringstream cmdStream(cmdStr);
	std::string c;
	cmdStream >> c;
	if(c == "get_pos" || c == "p") {
		cmd = CMD::GET_POS;
	}
	else if(c == "set_pos" || c == "P") {
		cmd = CMD::SET_POS;
	}
	else if(c == "stop" || c == "S") {
		cmd = CMD::STOP;
	}
	else if(c == "quit" || c == "q") {
		cmd = CMD::QUIT;
	}
	else {
		cmd = CMD::UNKNOWN;
	}
	if(cmd == CMD::SET_POS) {
		float az;
		float el;
		if(!(cmdStream >> az) || !(cmdStream >> el)) {
			throw RotErr(RIG_ERRORS::RIG_EINVAL, "Missing azimuth or elevation parameters");
		}
		coords = new RotCoord((int)az, (int)el);
	}
}
RotCmd::~RotCmd() {
	if(coords != nullptr) {
		delete coords;
		coords = nullptr;
	}
}