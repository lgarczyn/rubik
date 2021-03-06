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

enum Color {
	White = Index_U,
	Green = Index_F,
	Red = Index_R,
	Blue = Index_B,
	Orange = Index_L,
	Yellow = Index_D
};

enum SquareType {
	st_corner = 0,
	st_border = 1,
	st_center = 2,
};

enum Corner {
	Corner_URF = 0,
	Corner_ULF = 1,
	Corner_ULB = 2,
	Corner_URB = 3,
	Corner_DRF = 4,
	Corner_DLF = 5,
	Corner_DLB = 6,
	Corner_DRB = 7,
};

enum Border {
	Border_UR = 0,
	Border_UF = 1,
	Border_UL = 2,
	Border_UB = 3,

	Border_DR = 4,
	Border_DF = 5,
	Border_DL = 6,
	Border_DB = 7,

	Border_RF = 8,
	Border_FL = 9,
	Border_LB = 10,
	Border_BR = 11,
	Border_Start = 0,
	Border_End = 12,
	Border_UD_Start = 8,
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
	uchar f : 4;
	uchar l : 2;
	uchar c : 2;
};

struct ID {
	uint32_t borders_pos;
	uint16_t borders_rot;
	uint16_t corners_pos;
	uint16_t corners_rot;
};

struct IDG1 {
	uint16_t corners_rot : 12;
	uint16_t borders_rot : 11;
	uint16_t ud_slice : 9;
};

struct IDG2 {
	uint16_t corners_pos;
	uint16_t borders_crown_pos;
	uint16_t borders_ud_pos : 5;
};

using DataCorners = std::array<Square, 8>;
using DataBorders = std::array<Square, 12>;
using DataCornersRot = std::array<uchar, 8>;
using DataBordersRot = std::array<uchar, 12>;
using DataCornersPos = std::array<uchar, 8>;
using DataBordersPos = std::array<uchar, 12>;
using DataUD = std::array<bool, 12>;

struct Data {
	DataCorners corners;
	DataBorders borders;
};

template <class T>
struct SpeData {
};

template <>
struct SpeData<ID> {
	DataCorners corners;
	DataBorders borders;
};

template <>
struct SpeData<IDG1> {
	DataCornersRot corners_rot;
	DataBordersRot borders_rot;
	DataUD ud;
};

template <>
struct SpeData<IDG2> {
	DataCornersPos corners_pos;
	DataBordersPos borders_pos;
};

constexpr bool operator==(const ID &a, const ID &b);
constexpr bool operator==(const IDG1 &a, const IDG1 &b);
constexpr bool operator==(const IDG2 &a, const IDG2 &b);
constexpr bool operator==(const Square &sa, const Square &sb);
constexpr bool operator==(const Coord &ca, const Coord &cb);
constexpr bool operator!=(const ID &a, const ID &b);
constexpr bool operator!=(const IDG1 &a, const IDG1 &b);
constexpr bool operator!=(const IDG2 &a, const IDG2 &b);
constexpr bool operator!=(const Square &sa, const Square &sb);
constexpr bool operator!=(const Coord &ca, const Coord &cb);
inline std::ostream &operator<<(std::ostream &s, const Coord &cb);

using Column = std::array<Square, size>;
using Face = std::array<Column, size>;
using Cube = std::array<Face, 6>;

using Finder = std::array<Coord, size * size * 6>;

//the cost of a movement, compared to moving one block
//since score is max(corner_score, border_score),
//you can only increase it by 4 at a time
static const int score_multiplier = 4;
