//
// Created by saltyJeff on 2/20/2020.
//

#ifndef ROTTWEILER_ROTTWEILERSERVER_H
#define ROTTWEILER_ROTTWEILERSERVER_H

#include <netinet/in.h>
#include "Rotor.h"
#include <sockpp/tcp_acceptor.h>
#include <vector>
class RottweilerClient;
using namespace sockpp;
class RottweilerServer {
public:
	Rotor *rotor;
	RottweilerServer(Rotor *rotor, int portNum=3433, bool compatMode=false);
	void listen(bool *die);
	void tick();
	~RottweilerServer();
private:
	bool compatMode;
	tcp_acceptor acceptor;
	std::vector<RottweilerClient*> clients;
};


#endif //ROTTWEILER_ROTTWEILERSERVER_H
