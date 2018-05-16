/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:41:03 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/21 20:25:10 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <cstdio>
#include <limits.h>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

using std::pair;
using std::string;
using std::vector;
typedef uint8_t uchar;
typedef unsigned int uint;
typedef uint8_t Score;
typedef uint8_t Distance;

// DO NOT CHANGE
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
	st_corner = 0,
	st_border = 1,
	st_center = 2,
};

enum Corner {
	Corner_ULB = 0,
	Corner_URB = 1,
	Corner_ULF = 2,
	Corner_URF = 3,
	Corner_DLF = 4,
	Corner_DRF = 5,
	Corner_DLB = 6,
	Corner_DRB = 7,
};

/*enum CornersKociemba {
	URF,
	UFL,
	ULB,
	UBR,
	DFR,
	DLF,
	DBL,
	DRB
};*/

enum Border {
	Border_UB = 0,
	Border_UL = 1,
	Border_UR = 2,
	Border_UF = 3,

	Border_FL = 4,
	Border_RF = 5,
	Border_BR = 6,
	Border_LB = 7,

	Border_DF = 8,
	Border_DL = 9,
	Border_DR = 10,
	Border_DB = 11,
};

/*enum EdgeKociemba {
	UR,
	UF,
	UL,
	UB,
	DR,
	DF,
	DL,
	DB,
	FR,
	FL,
	BL,
	BR
};*/

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
	uint32_t borders_pos;
	uint16_t borders_rot;
	uint16_t corners_pos;
	uint16_t corners_rot;
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

#include "Types.cpp"
