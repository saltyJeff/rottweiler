//
// Created by jeffe on 2/20/2020.
//

#ifndef ROTTWEILER_ROTTWEILERCLIENT_H
#define ROTTWEILER_ROTTWEILERCLIENT_H
#include "RottweilerServer.h"
#include <sockpp/tcp_socket.h>
#include <sstream>
#include <string>
const int BUFFER_SZ = 1024;
class RottweilerClient {
public:
	sockpp::tcp_socket sock;
	RottweilerServer *server;
	char *buffer;
	std::string remoteAddr;
	bool keepalive = false;
	RottweilerClient(sockpp::tcp_socket sock, RottweilerServer *server);
	void tick();
	std::string handleCommand(std::string &cmd);
	void markDead();
	bool closed();
	~RottweilerClient();
private:
	bool dead = false;
};


#endif //ROTTWEILER_ROTTWEILERCLIENT_H
