#include "Heuristics.hpp"

Score Heuristics::ValidFunction(const Data& data)
{
    Score corners = get_dist_corners(data);
    return corners;
    //Score borders = get_dist_borders(data);
    //    std::cerr << "H " << corners << " " << borders << std::endl;

    //return std::max(corners, borders);;
}

Score Heuristics::InvalidFunction(const Data& data)
{
    Score dist = 0;
    dist += get_dist_crown(data) * 1000000;
    dist += get_dist_belt(data) * 10000;
    dist += get_dist_cross(data) * 100;
    dist += get_dist_floor(data);
    return dist;
}

 Heuristics::HeuristicIndexer Heuristics::HeuristicFunction = &Heuristics::ValidFunction;
 Heuristics::HeuristicIndexer Heuristics::HeuristicFunctionUnused = &Heuristics::InvalidFunction;
