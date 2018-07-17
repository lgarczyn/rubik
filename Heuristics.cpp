/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heuristics.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:29 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/21 01:38:07 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Heuristics.hpp"
#include "Database.hpp"
#include "Encoding.hpp"

constexpr int _get_opposite(int a) {
	switch (a) {
	case Index_U:
		return Index_D;
	case Index_D:
		return Index_U;
	case Index_B:
		return Index_F;
	case Index_F:
		return Index_B;
	case Index_R:
		return Index_L;
	case Index_L:
		return Index_R;
	}
	return -1;
}

constexpr int Heuristics::SquareDistance(int uid_a, int uid_b) {
	Coord a = Encoding::solution_finder[uid_a];
	Coord b = Encoding::solution_finder[uid_b];
	return SquareDistance(a, b);
}

// the basic function for heuristics, gives the minimal amount of moves
// to move a facelet from one position to another
// avoid running at runtime due to very poor performances
constexpr int Heuristics::SquareDistance(Coord a, Coord b) {
	bool isborder_a = (a.c == 1 || a.l == 1);
	bool isborder_b = (b.c == 1 || b.l == 1);
	if (isborder_a != isborder_b)
		return 9;
	bool iscenter_a = (a.c == 1 && a.l == 1);
	bool iscenter_b = (b.c == 1 && b.l == 1);
	if (iscenter_a || iscenter_b)
		return 9;
	if (a.f == b.f) {
		if (a.c == b.c && a.l == b.l)
			return 0;
		return 1;
	}

	// If a and b are on opposite sides
	if (a.f == _get_opposite(b.f)) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		if (a.c == 2 - b.c && a.l == 2 - b.l)
			return 1;
		return 2;
	}

	// If no one is on top or bottom
	if (a.f != Index_U && a.f != Index_D && b.f != Index_U && b.f != Index_D) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		return 2;
	}

	// If the one point is front and the other on top or bottom, no coord change
	if (a.f == Index_F || b.f == Index_F) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		return 2;
	}

	// If the one point is back, turning from bottom or top inverts coordinates
	if (a.f == Index_B || b.f == Index_B) {
		if (a.c == 2 - b.c && a.l == 2 - b.l)
			return 1;
		return 2;
	}

	bool clockwise = true;
	switch (a.f) {
	case Index_U:
		if (b.f == Index_L)
			clockwise = false;
		break;
	case Index_R:
		if (b.f == Index_U)
			clockwise = false;
		break;
	case Index_D:
		if (b.f == Index_R)
			clockwise = false;
		break;
	case Index_L:
		if (b.f == Index_D)
			clockwise = false;
		break;
	}

	if (clockwise)
		if (a.l == 2 - b.c && a.c == b.l)
			return 1;
		else
			return 2;
	else if (a.l == b.c && a.c == 2 - b.l)
		return 1;
	else
		return 2;
}

constexpr BufferCorner Heuristics::get_dist_table_corners() {
	BufferCorner buff = BufferCorner();

	for (int pos = 0; pos < 8; pos++)
		for (int id = 0; id < 8; id++)
			for (int rot = 0; rot < 3; rot++) {
				int src = Square(id, rot).get_uid(st_corner);
				int dst = Square(pos, 0).get_uid(st_corner);
				buff[pos][id][rot] = SquareDistance(src, dst);
			}
	return buff;
}

constexpr BufferBorder Heuristics::get_dist_table_borders() {
	BufferBorder buff = BufferBorder();

	for (int pos = 0; pos < 12; pos++)
		for (int id = 0; id < 12; id++)
			for (int rot = 0; rot < 2; rot++) {
				int src = Square(id, rot).get_uid(st_border);
				int dst = Square(pos, 0).get_uid(st_border);
				buff[pos][id][rot] = SquareDistance(src, dst);
			}
	return buff;
}

constexpr int get_dist(const Data &data, int i, SquareType st) {

	// returns the buffered minimal distance between
	// a rotated cubie and its unrotated destination
	// tables takes the id where the cube was found,
	// this id of the cube (and therefore destination)
	// and its rotation
	// this order is necessary to avoid cache misses

	if (st == st_border)
		return Heuristics::dist_table_borders[i][data.borders[i].cube_id][data.borders[i].rot_id];
	else if (st == st_corner)
		return Heuristics::dist_table_corners[i][data.corners[i].cube_id][data.corners[i].rot_id];
	else
		throw std::logic_error("Can't get dist for centers");
}

constexpr int get_dist_borders(const Data &data) {
	int dist = 0;
	for (int i = 0; i < 12; i++)
		dist += get_dist(data, i, st_border);
	return dist;
}

constexpr int get_dist_corners(const Data &data) {
	int dist = 0;
	for (int i = 0; i < 8; i++)
		dist += get_dist(data, i, st_corner);
	return dist;
}

constexpr Score Heuristics::ValidFunction(const Data &data) {
	int corners = get_dist_corners(data);
	int borders = get_dist_borders(data);
	return std::max(corners, borders);
}

constexpr Score Heuristics::ValidFunctionG1(const Data &data) {
	int dist_borders = 0;
	for (int i = 0; i < 12; i++)
		if (data.borders[i].rot_id != 0)
			dist_borders++;

	int dist_corners = 0;
	for (int i = 0; i < 8; i++)
		if (data.corners[i].rot_id != 0)
			dist_corners++;

	int dist_ud = 0;
	for (int i = 0; i < Border_UD_Start; i++)
		if (data.borders[i].cube_id >= Border_UD_Start)
			dist_ud++;

	for (int i = Border_UD_Start; i < 12; i++)
		if (data.borders[i].cube_id < Border_UD_Start)
			dist_ud++;

	return std::max(std::max(dist_borders * 2, dist_corners), dist_ud * 2);
}

constexpr Score Heuristics::ValidFunctionG2(const Data &data) {
	int corners = get_dist_corners(data);
	int borders = get_dist_borders(data);
	return std::max(corners, borders);
}

constexpr Score Heuristics::CornerFunction(const Data &data) {
	Score corners = get_dist_corners(data);
	return corners;
}

constexpr Score Heuristics::BorderFunction(const Data &data) {
	Score borders = get_dist_borders(data);
	return borders;
}

inline Score Heuristics::DatabaseFunction(const IDG1 &id) {
	(void)id;
	return 0;
}
inline Score Heuristics::DatabaseFunction(const IDG2 &id) {
	(void)id;
	return 0;
}

inline Score Heuristics::DatabaseFunction(const ID &id) {
	(void)id;
	// if (id.corners < Databases::current_index)
	// int id_upper = Encoding::floor_index_upper_corners(id.corners);
	//return Databases::corners[id.corners];
	return 0;

	// Score data = std::max(
	//	Databases::upper_corners[id.corners],
	//	Databases::lower_corners[id.corners]);
	// Score data
	// return data;
	// Score borders = get_dist_borders(data);
	//	std::cerr << "H " << corners << " " << borders << std::endl;

	// return std::max(corners, borders);;
}
