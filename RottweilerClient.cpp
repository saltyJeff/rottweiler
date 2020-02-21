//
// Created by saltyJeff on 2/20/2020.
//

#include "RottweilerClient.h"
#include "RotCmd.h"
#include "RotErrs.h"
#include <spdlog/spdlog.h>
RottweilerClient::RottweilerClient(sockpp::tcp_socket sock, RottweilerServer *server, bool compatMode) {
	// compatibility shim for rotctld-compliant devices
	if(compatMode) {
		keepalive = true;
		fancyOutput = false;
	}
	this->sock = sock.clone();
	this->server = server;
	remoteAddr = this->sock.peer_address().to_string();
	buffer = (char*) calloc(BUFFER_SZ, sizeof(char));
	spdlog::info("Client connected: {}", remoteAddr);
	this->sock.read_timeout(timeout());
	int t = 1;
	this->sock.set_option(SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t));
}
std::string RottweilerClient::handleCommand(std::string &cmd) {
	spdlog::debug("Rcvd command: {}", cmd);
	if(cmd == "keepalive") {
		keepalive = !keepalive;
		sock.read_timeout(timeout());
		return std::string("keepalive=") + (keepalive ? "true" : "false");
	}
	else if(cmd == "fancy") {
		fancyOutput = !fancyOutput;
		return std::string("fancyOutput=") + (fancyOutput ? "true" : "false");
	}
	// handle other options
	RotCmd rotCmd(cmd);
	RotCoord pos; // holds the output from the get command
	switch(rotCmd.cmd) {
	case RotCmd::CMD::SET_POS:
		server->rotor->setPosition(*rotCmd.coords);
		return RPRT("set");
		break;
	case RotCmd::CMD::GET_POS:
		pos = server->rotor->getPosition();
		return fancyOutput ? ( static_cast<std::string>(pos) ) : std::to_string(pos.az) +"\n"+ std::to_string(pos.el);
	case RotCmd::CMD::STOP:
		server->rotor->stop();
		return RPRT("stopped");
	case RotCmd::CMD::QUIT:
		keepalive = false;
		markDead();
		return RPRT("goodbye");
	}
	throw RotErr(RIG_ERRORS::RIG_EINVAL, "Unknown command: "+cmd);
}
void RottweilerClient::tick() {
	int i = 0;
	int n;
	while ( (n = sock.read(buffer + i, BUFFER_SZ - i)) > 0) {
		bool succ = false;
		for(int j = i; j < i + n; j++) {
			if(buffer[j] == '\n') {
				i = j;
				succ = true;
				break;
			}
		}
		if(succ) {
			break;
		}
		i += n;
	}
	if(n <= 0) {
		spdlog::warn("Error reading from client (they probably disconnected): {}", sock.last_error_str());
		markDead();
		return;
	}
	std::string inputStr(buffer, i-1);
	try {
		sock.write(handleCommand(inputStr));
		sock.write("\n");
	}
	catch(RotErr &e) {
		sock.write(fancyOutput ? e.what() : e.asHamlib());
		sock.write("\n");
	}
	catch(std::exception &e) {
		RotErr err(RIG_ERRORS::RIG_EINVAL, e.what());
		sock.write(fancyOutput ? err.what() : err.asHamlib());
		sock.write("\n");
	}
	if(!keepalive) {
		markDead();
	}
}
void RottweilerClient::markDead() {
	dead = true;
}
bool RottweilerClient::closed() {
	return dead;
}
RottweilerClient::~RottweilerClient() {
	bool shutdown = sock.shutdown();
	bool closed = sock.close();
	spdlog::debug("Closed socket for {}", remoteAddr);
}