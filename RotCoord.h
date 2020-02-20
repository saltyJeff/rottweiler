//
// Created by jeffe on 2/18/2020.
//

#ifndef ROTTWEILER_ROTCOORD_H
#define ROTTWEILER_ROTCOORD_H
#include <string>
#include <utility>

class RotCoord {
public:
	int az;
	int el;
	explicit RotCoord(int az=0, int el=0);
	RotCoord operator +(const RotCoord &other) const;
	RotCoord operator -(const RotCoord &other) const;
	bool operator ==(const RotCoord &other) const;
	double angleBetween(const RotCoord &other) const;
	bool isStandard() const;
	RotCoord standardized() const;
	RotCoord flipped() const;
	bool operator <(const RotCoord &other) const;
	explicit operator std::string() const;
	explicit operator std::pair<int, int>() const;
};


#endif //ROTTWEILER_ROTCOORD_H
