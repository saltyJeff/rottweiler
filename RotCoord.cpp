//
// Created by jeffe on 2/18/2020.
//

#include "RotCoord.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

double deg2rad (double degrees) {
	return (degrees / 360) * 2 * M_PI;
}
double rad2deg (double rads) {
	return (rads / 2 / M_PI) * 360;
}

RotCoord::RotCoord(int az, int el) {
	this->az = az;
	this->el = el;
	if(az < 0 || az > 360) {
		throw std::runtime_error("Azimuth is out of bounds");
	}
	if(el < 0 || el > 180) {
		throw std::runtime_error("Elevation is out of bounds");
	}
}
RotCoord RotCoord::operator+(const RotCoord &other) const {
	return RotCoord(az + other.az, el + other.el);
}
RotCoord RotCoord::operator-(const RotCoord &other) const {
	return RotCoord(az - other.az, el - other.el);
}
bool RotCoord::operator==(const RotCoord &other) const {
	return az == other.az && el == other.el;
}
bool RotCoord::operator<(const RotCoord &other) const {
	return az + el < other.az + other.el;
}
double RotCoord::angleBetween(const RotCoord &other) const {
	//wikipedia'd the answer: https://en.wikipedia.org/wiki/Great-circle_distance
	auto selfBound = standardized();
	auto otherBound = other.standardized();
	auto phi1 = deg2rad(selfBound.az);
	auto phi2 = deg2rad(otherBound.az);
	auto deltaPhi = deg2rad(abs(selfBound.az - otherBound.az));
	auto deltaLambda = deg2rad(abs(selfBound.el - otherBound.el));
	auto deltaSig = 2 * asin(sqrt(pow(sin(deltaPhi / 2), 2) + cos(phi1)*cos(phi2)*pow(sin(deltaLambda/2),2)));

	return rad2deg(deltaSig);
}
bool RotCoord::isStandard() const {
	return 0 <= az && az <= 360 && 0 <= el && el <= 180;
}
RotCoord RotCoord::standardized() const {
	if(!isStandard()) {
		return flipped();
	}
	return RotCoord(az, el);
}
RotCoord RotCoord::flipped() const {
	return RotCoord((180 - az % 360), 180 - el);
}
RotCoord::operator std::pair<int, int>() const {
	return std::pair(az, el);
}
RotCoord::operator std::string() const {
	std::stringstream ret;
	ret << '(';
	ret << az;
	ret << ",";
	ret << el;
	ret << ')';
	return ret.str();
}