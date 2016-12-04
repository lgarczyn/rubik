//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#ifndef _HEURISTICS_H
# define _HEURISTICS_H

#include <string>
#include "Types.hpp"
#include "State.hpp"

using Buffer = std::array<std::array<std::array<std::array<std::array<std::array<char, 3>, 3>, 6>, 3>, 3>, 6>;

namespace Heuristics
{
    static Score	HeuristicFunction(const Data& data);
    static constexpr int  	SquareDistance(Coord a, Coord b);

    static constexpr Buffer get_dist_table();
    static const Buffer dist_table = get_dist_table();
};

//
// created by louis garczynski on 3/4/16.
//
#include <algorithm>

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

constexpr Buffer Heuristics::get_dist_table() {
    Buffer buff = Buffer();

    (void)HeuristicFunction;
    for (int f = Index_Start; f < Index_Len; f++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
                for (int _f = Index_Start; _f < Index_Len; _f++)
                    for (int _l = 0; _l < size; _l++)
                        for (int _c = 0; _c < size; _c++)
                        {
                            Coord a = (Coord){f, l, c};
                            Coord b = (Coord){_f, _l, _c};
                            buff[f][l][c][_f][_l][_c] = SquareDistance(a, b);
                        }
    return buff;
}

Score Heuristics::HeuristicFunction(const Data& data)
{
    Score score = 0;

    //TODO
    //FIRST LOOP HALF CAN BE REDUCED
    //SECOND LOOP HALF CAN BE BUFFERIZED FOR ONE SOLUTION

    for (int f = Index_Start; f < Index_Len; f++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
                if (l != 1 || c != 1) {
                    // if ((uint)f > data.size()) std::cerr << "overflow f" << std::endl;
                    // if ((uint)l > data[f].size()) std::cerr << "overflow l" << std::endl;
                    // if ((uint)c > data[l][c].size()) std::cerr << "overflow c" << std::endl;

                    //int id = data.at(f).at(l).at(c).face_id;
                    //if ((uint)id > State::solution_finder.size()) std::cout << "overflow id" << std::endl;

                    //Coord p = (Coord){f, l, c};
                    //int dist = SquareDistance((Coord){f, l, c}, State::solution_finder.at(id));

                    int id = data[f][l][c].face_id;
                    Coord sol = State::solution_finder[id];
                    int dist = dist_table[f][l][c][sol.f][sol.l][sol.c];

                    score += dist;
                    //std::cout << p << finder[id] << std::endl;
                }
    return score;
}

#endif
