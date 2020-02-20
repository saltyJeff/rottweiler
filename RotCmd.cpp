//
// Created by jeffe on 2/19/2020.
//

#include "RotCmd.h"
#include <stdexcept>
#include <sstream>
RotCmd::RotCmd(std::string &cmdStr) {
	this->cmdStr = cmdStr;
	std::istringstream cmdStream(cmdStr);
	char c;
	cmdStream >> c;
	switch(c) {
	case 'p':
		cmd = CMD::GET_POS;
		break;
	case 'P':
		cmd = CMD::SET_POS;
		break;
	case 'S':
		cmd = CMD::STOP;
		break;
	case 'q':
		cmd = CMD::QUIT;
		break;
	default:
		cmd = CMD::UNKNOWN;
		break;
	}
	if(cmd == CMD::SET_POS) {
		float az;
		float el;
		if(!(cmdStream >> az) || !(cmdStream >> el)) {
			throw std::runtime_error("Missing azimuth and elevation parameters");
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