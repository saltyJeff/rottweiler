#pragma once

#include <string>
#include <sockpp/tcp_socket.h>
class RotArgs {
public:
	int modelId;
	std::string azDevFile;
	std::string elDevFile;
	int baudRate = 9600;
	in_addr_t listenAddr = INADDR_ANY;
	int portNum = 4553;
	bool verbose = false;
	bool version = false;
	bool test = false;
	float azOffset = 0;
	float elOffset = 0;
    RotArgs(int argc, char** argv);
};
