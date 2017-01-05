#pragma once

#include <array>

#include <string>
using string = std::string;
using uchar = uint8_t;
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

enum SquareType {
	st_corner,
	st_border,
	st_center,
};

static const uchar max_uid_corner = 8 * 3;
static const uchar max_uid_border = 8 * 3 + 12 * 2;
static const uchar max_uid = 8 * 3 + 12 * 2 + 6;
struct Square {
	uchar cube_id;
	uchar rot_id;
	constexpr Square (uchar cid, uchar rid):cube_id(cid),rot_id(rid){}
	constexpr Square ():cube_id(0),rot_id(0){}

	inline uchar get_uid(int c, int l) const{
		if (c == 1 && l == 1)
			return get_uid(st_center);
		if (c == 1 || l == 1)
			return get_uid(st_border);
		return get_uid(st_corner);
	}
	inline uchar get_uid(SquareType type) const{
		return get_uid(cube_id, rot_id, type);
	}
	static constexpr uchar get_uid(int cube_id, int rot_id, SquareType type) {
		if (type == st_corner)
			return cube_id * 3 + rot_id;
		if (type == st_border)
			return max_uid_corner + cube_id * 2 + rot_id;
		return max_uid_border + cube_id;
	}
	static constexpr Square get_square(uchar uid, SquareType& type) {
		if (uid >= max_uid)
			throw std::logic_error("uid >= max_uid");
		if (uid >= max_uid_border) {
			uid -= max_uid_border;
			type = st_center;
			return Square(uid, 0);
		}
		if (uid >= max_uid_corner) {
			uid -= max_uid_corner;
			type = st_border;
			return Square(uid / 2, uid % 2);
		}
		if (uid >= 0) {
			type = st_corner;
			return Square(uid / 3, (uchar)uid % 3);
		}
		throw std::logic_error("uid < 0");
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

using DataCorners = std::array<Square, 8>;
using DataBorders = std::array<Square, 12>;

struct Data{
	DataCorners corners;
	DataBorders borders;
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
