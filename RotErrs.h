//
// Created by saltyJeff on 2/21/2020.
//

#ifndef ROTTWEILER_ROTERRS_H
#define ROTTWEILER_ROTERRS_H
#include <stdexcept>
// adapted from: http://hamlib.sourceforge.net/manuals/1.2.15/group__rig.html#gga7e8e83074ddca12769cf89c9c1b13ed0ae0ae37639377aa56cf7db301d692a84c
enum class RIG_ERRORS {
	RIG_OK, // No error, operation completed sucessfully
	RIG_EINVAL, // invalid parameter
	RIG_ECONF, // invalid configuration (serial,..)
	RIG_ENOMEM, // memory shortage
	RIG_ENIMPL, // function not implemented, but will be
	RIG_ETIMEOUT, // communication timed out
	RIG_EIO, // IO error, including open failed
	RIG_EINTERNAL, // Internal Hamlib error, huh!
	RIG_EPROTO, // Protocol error
	RIG_ERJCTED, // Command rejected by the rig
	RIG_ETRUNC, // Command performed, but arg truncated
	RIG_ENAVAIL, // function not available
	RIG_ENTARGET, // VFO not targetable
	RIG_BUSERROR, // Error talking on the bus
	RIG_BUSBUSY, // Collision on the bus
	RIG_EARG, // NULL RIG handle or any invalid pointer parameter in get arg
	RIG_EVFO, // Invalid VFO
	RIG_EDOM // Argument out of domain of func
};

class RotErr: public std::runtime_error {
public:
	int code;
	RotErr(RIG_ERRORS errCode, const std::string& desc): code(-1 * static_cast<int>(errCode)),
		 std::runtime_error("ERROR code "+std::to_string(-1 * static_cast<int>(errCode))+" ("+desc+")") {};
	inline std::string asHamlib() {
		return "RPRT "+std::to_string(code);
	}
};

#endif //ROTTWEILER_ROTERRS_H
