#pragma once

#include <array>

using uchar = unsigned char;

enum Color {
	White = 0,
	Red = 1,
	Blue = 2,
	Orange = 4,
	Green = 5,
	Yellow = 6
};

enum Indexes {
	Index_Up = 0,
	Index_Front = 1,
	Index_Right = 2,
	Index_Back = 3,
	Index_Left = 4,
	Index_Down = 5,

	Index_Start = 0,
	Index_Len = 6,
};

static const int score_multiplier = 20;

struct SquareFull {
	Color color;
	int face_id;
	int cube_id;
};

struct Square {
	uchar face_id;
	uchar spec_id;
};

struct Coord {
	int f;
	int l;
	int c;
};

bool operator==(const Square& sa, const Square& sb);
bool operator==(const Coord& ca, const Coord& cb);
std::ostream& operator<<(std::ostream& s, const Coord& cb);

 //SI CHANGE
 // CHANGER DE IMPAIRE A PAIRE CASSE TOUT
 // RECODER TOUT LE SYSTEME DE TRANSFORMATION AUSSI
static const int size = 3;

using ColumnFull = std::array<SquareFull, size>;
using FaceFull = std::array<ColumnFull, size>;
using DataFull = std::array<FaceFull, 6>;

using Column = std::array<Square, size>;
using Face = std::array<Column, size>;
using Data = std::array<Face, 6>;

using Finder = std::array<Coord, size * size * 6>;

using Score = long int;
using weighter = Score (*)(const Data& data);
using uchar = unsigned char;

#include <string>
using string = std::string;
