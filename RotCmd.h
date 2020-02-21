//
// Created by saltyJeff on 2/19/2020.
//

#ifndef ROTTWEILER_ROTCMD_H
#define ROTTWEILER_ROTCMD_H
#include <vector>
#include <string>
#include "RotCoord.h"

class RotCmd {
public:
	enum class CMD {
		UNKNOWN,
		GET_POS,
		SET_POS,
		QUIT,
		STOP
	};
	CMD cmd;
	RotCoord *coords = nullptr;
	std::string cmdStr;
	RotCmd(std::string &cmdStr);
	~RotCmd();
};


#endif //ROTTWEILER_ROTCMD_H
