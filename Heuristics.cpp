#include "Database.hpp"
#include "Heuristics.hpp"

inline int get_dist(const Data& data, int i, SquareType st) {
	int id_sol;//ID of destination
	if (st == st_border)
		id_sol = data.borders[i].get_uid(st);
	else if (st == st_corner)
		id_sol = data.corners[i].get_uid(st);
	else
		throw std::logic_error("Can't get dist for centers");

	int id_src = Square::get_uid(i, 0, st);//ID of where the square was found

	return Heuristics::dist_table[id_sol][id_src];
}

inline Score get_dist_crown(const Data& data) {
	Score dist = 0;
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

inline Score get_dist_belt(const Data& data) {
	Score dist = 0;
	dist += get_dist(data, 4, st_border);
	dist += get_dist(data, 5, st_border);
	dist += get_dist(data, 6, st_border);
	dist += get_dist(data, 7, st_border);
	return dist;
}

inline Score get_dist_cross(const Data& data) {
	Score dist = 0;
	dist += get_dist(data, 8, st_border);
	dist += get_dist(data, 9, st_border);
	dist += get_dist(data, 10, st_border);
	dist += get_dist(data, 11, st_border);
	return dist;
}

inline Score get_dist_floor(const Data& data) {
	Score dist = 0;
	dist += get_dist(data, 4, st_corner);
	dist += get_dist(data, 5, st_corner);
	dist += get_dist(data, 6, st_corner);
	dist += get_dist(data, 7, st_corner);
	return dist;
}

inline Score get_dist_borders(const Data& data) {
	Score dist = 0;
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

inline Score get_dist_corners(const Data& data) {
	Score dist = 0;
	dist += get_dist(data, 0, st_corner);
	dist += get_dist(data, 1, st_corner);
	dist += get_dist(data, 2, st_corner);
	dist += get_dist(data, 3, st_corner);

	//dist += get_dist(data, 4, st_corner);
	//dist += get_dist(data, 5, st_corner);
	//dist += get_dist(data, 6, st_corner);
	//dist += get_dist(data, 7, st_corner);
	return dist;
}

Score Heuristics::ValidFunction(const Data& data)
{
	//Cube cube = State::cube_from_data(data);
	Score corners = get_dist_corners(data);
	return corners;
	//Score borders = get_dist_borders(data);
	//return std::max(corners, borders);
}

Score Heuristics::DatabaseFunction(const ID& id)
{
	//if (id.corners < Databases::current_index)
	return Databases::upper_corners[id.corners];
	//return 0;

	//Score data = std::max(
	//	Databases::upper_corners[id.corners],
	//	Databases::lower_corners[id.corners]);
	//Score data
	//return data;
	//Score borders = get_dist_borders(data);
	//	std::cerr << "H " << corners << " " << borders << std::endl;

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
