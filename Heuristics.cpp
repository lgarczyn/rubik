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

constexpr Buffer Heuristics::get_dist_table() {
	Buffer buff = Buffer();

	for (int uid_a = 0; uid_a < max_uid; uid_a++)
		for (int uid_b = 0; uid_b < max_uid; uid_b++)
			buff[uid_a][uid_b] = SquareDistance(uid_a, uid_b);
	return buff;
}

constexpr int get_dist(const Data &data, int i, SquareType st) {
	int id_sol = 0; // ID of destination
	if (st == st_border)
		id_sol = data.borders[i].get_uid(st);
	else if (st == st_corner)
		id_sol = data.corners[i].get_uid(st);
	else
		throw std::logic_error("Can't get dist for centers");

	int id_src = Square::get_uid(i, 0, st); // ID of where the square was found

	return Heuristics::dist_table[id_sol][id_src];
}

constexpr int get_dist_crown(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 0, st_corner);
	dist += get_dist(data, 1, st_corner);
	dist += get_dist(data, 2, st_corner);
	dist += get_dist(data, 3, st_corner);
	dist += get_dist(data, 0, st_border);
	dist += get_dist(data, 1, st_border);
	dist += get_dist(data, 2, st_border);
	dist += get_dist(data, 3, st_border);
	return dist;
}

constexpr int get_dist_belt(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 4, st_border);
	dist += get_dist(data, 5, st_border);
	dist += get_dist(data, 6, st_border);
	dist += get_dist(data, 7, st_border);
	return dist;
}

constexpr int get_dist_cross(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 8, st_border);
	dist += get_dist(data, 9, st_border);
	dist += get_dist(data, 10, st_border);
	dist += get_dist(data, 11, st_border);
	return dist;
}

constexpr int get_dist_floor(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 4, st_corner);
	dist += get_dist(data, 5, st_corner);
	dist += get_dist(data, 6, st_corner);
	dist += get_dist(data, 7, st_corner);
	return dist;
}

constexpr int get_dist_borders(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 0, st_border);
	dist += get_dist(data, 1, st_border);
	dist += get_dist(data, 2, st_border);
	dist += get_dist(data, 3, st_border);
	dist += get_dist(data, 4, st_border);
	dist += get_dist(data, 5, st_border);
	dist += get_dist(data, 6, st_border);
	dist += get_dist(data, 7, st_border);
	dist += get_dist(data, 8, st_border);
	dist += get_dist(data, 9, st_border);
	dist += get_dist(data, 10, st_border);
	dist += get_dist(data, 11, st_border);
	return dist;
}

constexpr int get_dist_corners(const Data &data) {
	int dist = 0;
	dist += get_dist(data, 0, st_corner);
	dist += get_dist(data, 1, st_corner);
	dist += get_dist(data, 2, st_corner);
	dist += get_dist(data, 3, st_corner);

	dist += get_dist(data, 4, st_corner);
	dist += get_dist(data, 5, st_corner);
	dist += get_dist(data, 6, st_corner);
	dist += get_dist(data, 7, st_corner);
	return dist;
}

constexpr Score Heuristics::ValidFunction(const Data &data) {
	int corners = get_dist_corners(data);
	int borders = get_dist_borders(data);
	if (corners > 0xFF)
		throw std::logic_error("WTF1");
	if (corners > 0xFFFF)
		throw std::logic_error("WTF2");
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

constexpr Score Heuristics::HeuristicFunction(const Data &data) {
	Score score = ValidFunction(data);
	return score;
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

constexpr Score Heuristics::InvalidFunction(const Data &data) {
	Score dist = 0;
	dist += get_dist_crown(data) * 1000000;
	dist += get_dist_belt(data) * 10000;
	dist += get_dist_cross(data) * 100;
	dist += get_dist_floor(data);
	return dist;
}
