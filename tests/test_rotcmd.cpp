#include "tests.h"
#include "../RotCmd.h"
#include <string>

TEST_CASE("get coord commands parsed", "[rot cmd]") {
	std::string input = "p";
	RotCmd cmd(input);
	REQUIRE(cmd.cmd == RotCmd::CMD::GET_POS);
}
TEST_CASE("stop commands parsed", "[rot cmd]") {
	std::string input = "S";
	RotCmd cmd(input);
	REQUIRE(cmd.cmd == RotCmd::CMD::STOP);
}
TEST_CASE("valid set commands parsed", "[rot cmd]") {
	std::string input = "P 40.6 20";
	RotCmd cmd(input);
	REQUIRE(cmd.cmd == RotCmd::CMD::SET_POS);
	REQUIRE(cmd.coords->az == 40);
	REQUIRE(cmd.coords->el == 20);
}
TEST_CASE("invalid set commands parsed", "[rot cmd]") {
	std::string input = "P 2100";
	REQUIRE_THROWS(new RotCmd(input));
}