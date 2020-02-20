//
// Created by jeffe on 2/20/2020.
//

#include "RottweilerServer.h"
#include "RottweilerClient.h"
#include <spdlog/spdlog.h>
#include <stdexcept>
class RottweilerClient;
RottweilerServer::RottweilerServer(Rotor *rotor, int portNum): rotor(rotor) {
	bool ok = acceptor.open(portNum);
	if(!ok) {
		spdlog::error("Could not open socket! {}", acceptor.last_error_str());
		throw std::runtime_error("Error opening socket: "+acceptor.last_error_str());
	}
	acceptor.set_non_blocking(true);
	spdlog::warn("Listening on port {}", portNum);
}
void RottweilerServer::listen(bool *die) {
	while(!(*die)) {
		tick();
	}
}
void RottweilerServer::tick() {
	auto i = std::begin(clients);
	while (i != std::end(clients)) {
		RottweilerClient *client = *i;
		if (client->closed()) {
			spdlog::info("Client disconnected: {}", client->remoteAddr);
			delete client;
			i = clients.erase(i);
		}
		else {
			client->tick();
			i++;
		}
	}
	inet_address peer;
	tcp_socket sock = acceptor.accept(&peer);
	if(sock) {
		clients.push_back(new RottweilerClient(sock.clone(), this));
	}
}
RottweilerServer::~RottweilerServer() {
	for(RottweilerClient *client : clients) {
		delete client;
	}
	clients.clear();
	acceptor.close();
	spdlog::info("Server closed");
}