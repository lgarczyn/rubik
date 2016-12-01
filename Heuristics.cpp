//
// created by louis garczynski on 3/4/16.
//

#include "Heuristics.hpp"
#include <algorithm>

weighter	Heuristics::HeuristicFunction = Heuristics::Distance;

inline int _get_opposite(int a) {
    switch (a) {
        case Index_Up: return Index_Down;
        case Index_Down: return Index_Up;
        case Index_Back: return Index_Front;
        case Index_Front: return Index_Back;
        case Index_Right: return Index_Left;
        case Index_Left: return Index_Right;
    }
    return (-1);
}
//{UP, 0, 0} {RIGHT, 0, 2}
//{5, 0, 0} {1, 2, 0}
int Heuristics::SquareDistance(Coord a, Coord b) {

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
    if (a.f != Index_Up && a.f != Index_Down && b.f != Index_Up && b.f != Index_Down) {
        if (a.c == b.c && a.l == b.l)
            return 1;
        return 2;
    }

    //If the one point is front and the other on top or bottom, no coord change
    if (a.f == Index_Front || b.f == Index_Front) {
        if (a.c == b.c && a.l == b.l)
            return 1;
        return 2;
    }

    //If the one point is back, turning from bottom or top inverts coordinates
    if (a.f == Index_Back || b.f == Index_Back) {
        if (a.c == 2 - b.c && a.l == 2 - b.l)
            return 1;
        return 2;
    }

    bool clockwise = true;
    switch (a.f) {
        case Index_Up: if (b.f == Index_Left) clockwise = false; break;
        case Index_Right: if (b.f == Index_Up) clockwise = false; break;
        case Index_Down: if (b.f == Index_Right) clockwise = false; break;
        case Index_Left: if (b.f == Index_Down) clockwise = false; break;
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

Score Heuristics::Distance(const Data& data)
{
    Score score = 0;

    for (int s = Index_Start; s < Index_Len; s++)
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++)
                if (x != 1 || y != 1) {
                    int id = data[s][x][y].face_id;
                    //Coord p = (Coord){s, x, y};
                    int dist = SquareDistance((Coord){s, x, y}, State::solution_finder[id]);
                    score += dist;
                    //std::cout << p << finder[id] << std::endl;
                }
    //if (State::compare(data, State::solution) != 0)
    return score;
}
