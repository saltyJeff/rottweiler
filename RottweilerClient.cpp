//
// Created by jeffe on 2/20/2020.
//

#include "RottweilerClient.h"
#include "RotCmd.h"
#include <spdlog/spdlog.h>
RottweilerClient::RottweilerClient(sockpp::tcp_socket sock, RottweilerServer *server) {
	this->sock = sock.clone();
	this->server = server;
	remoteAddr = this->sock.peer_address().to_string();
	buffer = (char*) calloc(BUFFER_SZ, sizeof(char));
	spdlog::info("Client connected: {}", remoteAddr);
	this->sock.read_timeout(std::chrono::seconds(3));
	int t = 1;
	this->sock.set_option(SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t));
}
std::string RottweilerClient::handleCommand(std::string &cmd) {
	spdlog::debug("Rcvd command: {}", cmd);
	if(cmd == "keepalive") {
		keepalive = !keepalive;
		sock.read_timeout(std::chrono::seconds(keepalive ? 0 : 3));
		return std::string("keepalive=") + (keepalive ? "true" : "false");
	}
	// handle other options
	RotCmd rotCmd(cmd);
	switch(rotCmd.cmd) {
	case RotCmd::CMD::SET_POS:
		server->rotor->setPosition(*rotCmd.coords);
		return "set";
	case RotCmd::CMD::GET_POS:
		return static_cast<std::string>(server->rotor->getPosition());
	case RotCmd::CMD::STOP:
		server->rotor->stop();
		return "stopped";
	}
	return "Unknown command";
}
void RottweilerClient::tick() {
	int i = 0;
	int n;
	while ( (n = sock.read(buffer + i, BUFFER_SZ - i)) > 0) {
		for(int j = i; j < i + n; j++) {
			if(buffer[j] == '\n') {
				i = j;
				goto READ_DONE;
			}
		}
		i += n;
	}
	if(n < 0) {
		spdlog::error("Error reading from client: {}", sock.last_error_str());
		markDead();
		return;
	}
	if(n == 0) {
		spdlog::warn("Either read timed out or socket buffer was over-flowed");
		markDead();
		return;
	}
READ_DONE:
	std::string inputStr(buffer, i-1);
	try {
		sock.write(handleCommand(inputStr));
		sock.write("\n");
	}
	catch(std::exception &e) {
		sock.write("ERROR: " + std::string(e.what()));
		sock.write("\n");
		markDead();
		return;
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
	spdlog::debug("Closed socket: shutdown status: {} | closed status: {}", shutdown, closed);
}