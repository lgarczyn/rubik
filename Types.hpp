/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:41:03 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:30 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <limits.h>
#include <stdint.h>
#include <string>
#include <utility>

using std::string;
using std::pair;
typedef uint8_t uchar;
typedef unsigned int uint;
typedef int Score;

// DO NOT CHANGE
static const int size = 3;

enum Color { White = 0,
	Red = 1,
	Blue = 2,
	Orange = 4,
	Green = 5,
	Yellow = 6 };

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
	constexpr Square(uchar cid, uchar rid);
	constexpr Square();

	constexpr uchar get_uid(int c, int l) const;
	constexpr uchar get_uid(SquareType type) const;
	static constexpr uchar get_uid(int cube_id, int rot_id, SquareType type);
	static constexpr pair<Square, SquareType> get_square(uchar uid);
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

struct Data {
	DataCorners corners;
	DataBorders borders;
};

constexpr bool operator==(const ID &a, const ID &b);
constexpr bool operator==(const Square &sa, const Square &sb);
constexpr bool operator==(const Coord &ca, const Coord &cb);
constexpr bool operator!=(const ID &a, const ID &b);
constexpr bool operator!=(const Square &sa, const Square &sb);
constexpr bool operator!=(const Coord &ca, const Coord &cb);
inline std::ostream &operator<<(std::ostream &s, const Coord &cb);

using Column = std::array<Square, size>;
using Face = std::array<Column, size>;
using Cube = std::array<Face, 6>;

using Finder = std::array<Coord, size * size * 6>;
using weighter = Score (*)(const Cube &data);

#include "Types.cpp"
