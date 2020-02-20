#include "tests.h"
#include "../RotCmd.h"
#include <cassert>
#include <string>

void testRotCmd() {
	std::string getCoord = "p";
	std::string stop = "S";
	std::string quit = "q";
	std::string validSet = "P 40.6 20";
	std::string invalidSet = "P 2100";

	RotCmd getCoordCmd(getCoord);
	AssertThat(getCoordCmd.cmd, Equals(RotCmd::CMD::GET_POS));
	RotCmd stopCmd(stop);
	AssertThat(getCoordCmd.cmd, Equals(RotCmd::CMD::STOP));
	RotCmd quitCmd(quit);
	AssertThat(getCoordCmd.cmd, Equals(RotCmd::CMD::QUIT));
	RotCmd validSetCmd(validSet);
	AssertThat(getCoordCmd.cmd, Equals(RotCmd::CMD::SET_POS));
	AssertThat(getCoordCmd.coords->az, Equals(40));
	AssertThat(getCoordCmd.coords->el, Equals(20));

	AssertThrows(std::runtime_error, new RotCmd(invalidSet));
}