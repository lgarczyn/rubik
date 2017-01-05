//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#ifndef _HEURISTICS_H
# define _HEURISTICS_H

#include <string>
#include "Types.hpp"
#include "State.hpp"
#include <algorithm>

#define MAX_SOLUTION_LENGTH ((int)10000)
static const int score_multiplier = 4;

using Buffer = std::array<std::array<uchar, max_uid>, max_uid>;

namespace Heuristics
{
	typedef Score				(*HeuristicIndexer)(const Data&);
	extern HeuristicIndexer		HeuristicFunction;
	extern HeuristicIndexer		HeuristicFunctionUnused;
	extern Score				ValidFunction(const Data& data);
	extern Score				InvalidFunction(const Data& data);
	static constexpr int		SquareDistance(int uid_a, int uid_b);
    static constexpr int		SquareDistance(Coord a, Coord b);

	extern Score				DatabaseFunction(const ID&);

	static constexpr Buffer		get_dist_table();
	static const Buffer			dist_table = get_dist_table();

};

inline int _get_opposite(int a) {
	switch (a) {
		case Index_U: return Index_D;
		case Index_D: return Index_U;
		case Index_B: return Index_F;
		case Index_F: return Index_B;
		case Index_R: return Index_L;
		case Index_L: return Index_R;
	}
	return (-1);
}

constexpr int Heuristics::SquareDistance(int uid_a, int uid_b) {
    Coord a = Encoding::coord_from_uid(uid_a);
    Coord b = Encoding::coord_from_uid(uid_b);
    return SquareDistance(a, b);
}

constexpr int Heuristics::SquareDistance(Coord a, Coord b){
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

	//If a and b are on opposite sides
	if (a.f == _get_opposite(b.f)) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		if (a.c == 2 - b.c && a.l == 2 - b.l)
			return 1;
		return 2;
	}

	//If no one is on top or bottom
	if (a.f != Index_U && a.f != Index_D && b.f != Index_U && b.f != Index_D) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		return 2;
	}

	//If the one point is front and the other on top or bottom, no coord change
	if (a.f == Index_F || b.f == Index_F) {
		if (a.c == b.c && a.l == b.l)
			return 1;
		return 2;
	}

	//If the one point is back, turning from bottom or top inverts coordinates
	if (a.f == Index_B || b.f == Index_B) {
		if (a.c == 2 - b.c && a.l == 2 - b.l)
			return 1;
		return 2;
	}

	bool clockwise = true;
	switch (a.f) {
		case Index_U: if (b.f == Index_L) clockwise = false; break;
		case Index_R: if (b.f == Index_U) clockwise = false; break;
		case Index_D: if (b.f == Index_R) clockwise = false; break;
		case Index_L: if (b.f == Index_D) clockwise = false; break;
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


#endif
