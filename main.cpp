#include "RotArgs.h"
#include "GreenHeron.h"
#include "DummyRotor.h"
#include <spdlog/spdlog.h>
#include "tests/tests.h"
#include "RottweilerServer.h"
#include <execinfo.h>
bool die = false;
void segfaultCatcher(int i) {
	die = true;
	spdlog::warn("Caught SIGINT... shutting down");
}
int runMain(int argc, char **argv) {
	RotArgs args(argc, argv);
	Rotor *rotor;
	spdlog::set_level(args.verbose ? spdlog::level::debug : spdlog::level::warn);
	if (args.test) {
		rotor = new DummyRotor();
	}
	else {
		rotor = new GreenHeron(args.azDevFile, args.elDevFile, args.baudRate);
	}
	spdlog::info("Rotor established");
	signal(SIGINT, segfaultCatcher);
	RottweilerServer server(rotor, args.portNum);
	server.listen(&die);
	delete rotor;
	return 0;
}

int main(int argc, char **argv) {
	try {
		return runMain(argc, argv);
	}
	catch (std::exception &e) {
		spdlog::error("Fatal exception: {}", e.what());
		void *array[20];
		size_t size;
		size = backtrace(array, 10);
		backtrace_symbols_fd(array, size, STDERR_FILENO);
		return 1;
	}
}