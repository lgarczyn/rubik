#include "Heuristics.hpp"

inline int get_dist(const Cube& data, Index f, int l, int c) {
    int id = data[f][l][c].get_uid(l, c);//TODO smart version
    Coord sol = State::solution_finder[id];
    return Heuristics::dist_table[f][l][c][sol.f][sol.l][sol.c];
}

inline Score get_dist_borders(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_U, 0, 1);
    dist += get_dist(data, Index_U, 1, 0);
    dist += get_dist(data, Index_U, 1, 2);
    dist += get_dist(data, Index_U, 2, 1);

    dist += get_dist(data, Index_F, 1, 0);
    dist += get_dist(data, Index_R, 1, 0);
    dist += get_dist(data, Index_B, 1, 0);
    dist += get_dist(data, Index_L, 1, 0);

    dist += get_dist(data, Index_D, 0, 1);
    dist += get_dist(data, Index_D, 1, 0);
    dist += get_dist(data, Index_D, 1, 2);
    dist += get_dist(data, Index_D, 2, 1);
    return dist;
}

inline Score get_dist_crown(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_U, 0, 0);
    dist += get_dist(data, Index_U, 0, 2);
    dist += get_dist(data, Index_U, 2, 0);
    dist += get_dist(data, Index_U, 2, 2);
    dist += get_dist(data, Index_U, 0, 1);
    dist += get_dist(data, Index_U, 1, 0);
    dist += get_dist(data, Index_U, 1, 2);
    dist += get_dist(data, Index_U, 2, 1);
    return dist;
}

inline Score get_dist_belt(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_F, 1, 0);
    dist += get_dist(data, Index_R, 1, 0);
    dist += get_dist(data, Index_B, 1, 0);
    dist += get_dist(data, Index_L, 1, 0);
    return dist;
}

inline Score get_dist_cross(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_D, 0, 1);
    dist += get_dist(data, Index_D, 1, 0);
    dist += get_dist(data, Index_D, 1, 2);
    dist += get_dist(data, Index_D, 2, 1);
    return dist;
}

inline Score get_dist_floor(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_D, 0, 0);
    dist += get_dist(data, Index_D, 0, 2);
    dist += get_dist(data, Index_D, 2, 0);
    dist += get_dist(data, Index_D, 2, 2);
    return dist;
}

inline Score get_dist_corners(const Cube& data) {
    Score dist = 0;
    dist += get_dist(data, Index_U, 0, 0);
    dist += get_dist(data, Index_U, 0, 2);
    dist += get_dist(data, Index_U, 2, 0);
    dist += get_dist(data, Index_U, 2, 2);

    dist += get_dist(data, Index_D, 0, 0);
    dist += get_dist(data, Index_D, 0, 2);
    dist += get_dist(data, Index_D, 2, 0);
    dist += get_dist(data, Index_D, 2, 2);
    return dist;
}

#include "Database.hpp"

Score Heuristics::ValidFunction(const Data& data)
{
    Cube cube = State::cube_from_data(data);
    Score corners = get_dist_corners(cube);
    return corners;
    //Score borders = get_dist_borders(data);
    //return std::max(corners, borders);
}

Score Heuristics::DatabaseFunction(const ID& id)
{
    if (id.corners < Databases::current_index)
        return Databases::upper_corners[id.corners];
    return 0;

    //Score data = std::max(
    //    Databases::upper_corners[id.corners],
    //    Databases::lower_corners[id.corners]);
    //Score data
    //return data;
    //Score borders = get_dist_borders(data);
    //    std::cerr << "H " << corners << " " << borders << std::endl;

    //return std::max(corners, borders);;
}

Score Heuristics::InvalidFunction(const Data& data)
{
    Cube cube = State::cube_from_data(data);
    Score dist = 0;
    dist += get_dist_crown(cube) * 1000000;
    dist += get_dist_belt(cube) * 10000;
    dist += get_dist_cross(cube) * 100;
    dist += get_dist_floor(cube);
    return dist;
}

 Heuristics::HeuristicIndexer Heuristics::HeuristicFunction = &Heuristics::ValidFunction;
 Heuristics::HeuristicIndexer Heuristics::HeuristicFunctionUnused = &Heuristics::InvalidFunction;
