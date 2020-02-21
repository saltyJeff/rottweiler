//
// Created by saltyJeff on 2/20/2020.
//

#ifndef ROTTWEILER_ROTTWEILERCLIENT_H
#define ROTTWEILER_ROTTWEILERCLIENT_H
#include "RottweilerServer.h"
#include <sockpp/tcp_socket.h>
#include <sstream>
#include <string>
const int BUFFER_SZ = 1024;
const int READ_TIMEOUT = 5;

class RottweilerClient {
public:
	sockpp::tcp_socket sock;
	RottweilerServer *server;
	char *buffer;
	std::string remoteAddr;
	bool compatMode = false;
	bool keepalive = false;
	bool fancyOutput = true;
	RottweilerClient(sockpp::tcp_socket sock, RottweilerServer *server, bool compatMode=false);
	void tick();
	std::string handleCommand(std::string &cmd);
	void markDead();
	bool closed();
	~RottweilerClient();
private:
	bool dead = false;
	inline std::chrono::seconds timeout() {
		return std::chrono::seconds(keepalive ? 0 : READ_TIMEOUT);
	}
	inline std::string RPRT(const char * message) {
		return fancyOutput ? message : "RPRT 0";
	}
};


#endif //ROTTWEILER_ROTTWEILERCLIENT_H
