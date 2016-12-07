#pragma once

#include <array>

#include <string>
using string = std::string;
using uchar = unsigned char;
using uint = unsigned int;
using Score = int;

 //DO NOT CHANGE
static const int size = 3;
static const int score_multiplier = 20;

enum Color {
	White = 0,
	Red = 1,
	Blue = 2,
	Orange = 4,
	Green = 5,
	Yellow = 6
};

enum Index {
	Index_Up = 0,
	Index_Front = 1,
	Index_Right = 2,
	Index_Back = 3,
	Index_Left = 4,
	Index_Down = 5,

	Index_Start = 0,
	Index_Len = 6,
};

struct Square {
	uchar cube_id;
	uchar rot_id;
	uchar face_id;
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

bool operator==(const ID& a, const ID& b);
bool operator==(const Square& sa, const Square& sb);
bool operator==(const Coord& ca, const Coord& cb);
bool operator!=(const ID& a, const ID& b);
bool operator!=(const Square& sa, const Square& sb);
bool operator!=(const Coord& ca, const Coord& cb);
std::ostream& operator<<(std::ostream& s, const Coord& cb);

using Column = std::array<Square, size>;
using Face = std::array<Column, size>;
using Data = std::array<Face, 6>;
//TODO remove * 100
using CornerUIDFinder = std::array<std::array<uchar, 3 * 100>, 8 * 100>;
using BorderUIDFinder = std::array<std::array<uchar, 2 * 100>, 12 * 100>;
using CenterUIDFinder = std::array<uchar, 6 * 100>;
struct UIDFinder {
	CornerUIDFinder corners;
	BorderUIDFinder borders;
	CenterUIDFinder centers;
};

using Finder = std::array<Coord, size * size * 6>;
using weighter = Score (*)(const Data& data);
