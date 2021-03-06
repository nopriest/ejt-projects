/*	Frost Open-Source World of Warcraft Bot
	Copyright (C) 2013 https://code.google.com/p/ejt-projects/

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _CLOCATION_H_
#define _CLOCATION_H_

#include <string>

using namespace std;

class CLocation {
public:
	float X, Y, Z;
	CLocation() {};
	CLocation(CLocation* l) { X = l->X; Y = l->Y; Z = l->Z; }
	CLocation(float x, float y, float z) { X = x; Y = y; Z = z; }

	double Bearing();
	double GetDistanceTo(CLocation l);
	double GetDistanceToFlat(CLocation l);
	double GetDistanceToSelf();
	double FacingTo(CLocation l);

	string ToString();
	double NegativeAngle(double angle);

	static const double PI;
};

#endif