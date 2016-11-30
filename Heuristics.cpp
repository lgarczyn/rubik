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
    //If b is on top or bottom, make it so a is anyway
    if (b.f == Index_Up || b.f == Index_Down)
        std::swap(a, b);

    //If the second point is front, turning from bottom or top won't change coord
    if (b.f == Index_Front) {
        if (a.c == b.c && a.l == b.l)
            return 1;
        return 2;
    }
    //If the second point is back, turning from bottom or top inverts coordinates
    if (b.f == Index_Back) {
        if (a.c == 2 - b.c && a.l == 2 - b.l)
            return 1;
        return 2;
    }
    return 1;

    //If the from left to up, or right to down
    if ((a.f == Index_Up && b.f == Index_Left) || (a.f == Index_Down && b.f == Index_Right)) {
        if (a.l == b.c && a.c == 2 - b.l)
            return 1;
        return 2;
    }

    //if (a.f == Index_Up && b.f == Index_Right)
    //    std::cout << "{"<<a.l<<","<<a.c<<"}{"<<b.l<<","<<b.c<<"}";
    //if () {

    if (a.l == 2 - b.c && a.c == b.l)
        return 1;
    return 2;
    //}
    //return 1;*/
    //TODO Take into account movements from UP or Down to Right or Left
}

Score Heuristics::Distance(const Data& data, const Finder& finder)
{
    Score score = 0;

    for (int s = Index_Start; s < Index_Len; s++)
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++) {
                int id = data[s][x][y].face_id;
                //if ((uint)id >= finder.size()) throw std::logic_error("fucka");
                //if ((uint)s >= data.size()) throw std::logic_error("fuckb" + std::to_string(data.size()));
                //if ((uint)x >= data[s].size()) throw std::logic_error("fuckc");
                //if ((uint)y >= data[s][x].size()) throw std::logic_error("fuckd");
                score += SquareDistance((Coord){s, x, y}, finder[id]);
            }

    return score / 20;
}
