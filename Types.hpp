#pragma once

#include <array>

#include <string>
using string = std::string;
using uchar = unsigned char;
using uint = unsigned int;
using Score = int;

 //DO NOT CHANGE
static const int size = 3;

enum Color {
	White = 0,
	Red = 1,
	Blue = 2,
	Orange = 4,
	Green = 5,
	Yellow = 6
};

enum Index {
	Index_U = 0,
	Index_F = 1,
	Index_R = 2,
	Index_B = 3,
	Index_L = 4,
	Index_D = 5,

	Index_Start = 0,
	Index_End = 6,
};

struct Square {
	uchar cube_id;
	uchar rot_id;
	uchar get_uid(int c, int l) const{
		if (c == 1 && l == 1)
			return 8 * 3 + 12 * 2 + cube_id;
		if (c == 1 || l == 1)
			return 8 * 3 + cube_id * 2 + rot_id;
		return cube_id * 3 + rot_id;
	}
};

struct Coord {
	int f;
	int l;
	int c;
};

struct ID {
	uint borders_pos;
	uint borders_rot;
	uint corners;
};

using DataCorners = std::array<uchar, 8>;
using DataBorders = std::array<uchar, 12>;

struct Data {
	DataCorners corners_pos;
	DataCorners corners_rot;
	DataBorders borders_pos;
	DataBorders borders_rot;
};

bool operator==(const ID& a, const ID& b);
bool operator==(const Square& sa, const Square& sb);
bool operator==(const Coord& ca, const Coord& cb);
bool operator!=(const ID& a, const ID& b);
bool operator!=(const Square& sa, const Square& sb);
bool operator!=(const Coord& ca, const Coord& cb);
std::ostream& operator<<(std::ostream& s, const Coord& cb);

using Column = std::array<Square, size>;
using Face = std::array<Column, size>;
using Cube = std::array<Face, 6>;

using Finder = std::array<Coord, size * size * 6>;
using weighter = Score (*)(const Cube& data);
