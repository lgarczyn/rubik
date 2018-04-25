/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:40:00 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/21 01:41:35 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"
#include "Heuristics.hpp"
#include "State.hpp"
#include <iomanip>

void display_square(Square sq, int dist, bool correct, Color color) {
	string background;
	string foreground;
	(void)dist;
	switch (color) {
	case White:
		background = "\e[107m";
		break;
	case Red:
		background = "\e[41m";
		break;
	case Blue:
		background = "\e[44m";
		break;
	case Orange:
		background = "\e[45m";
		break;
	case Green:
		background = "\e[42m";
		break;
	case Yellow:
		background = "\e[103m";
		break;
	default:
		std::cout << "\e[32mERR\e[0m";
		return;
	}

	(void)sq;
	(void)correct;

	/* if (correct)
		foreground = "\e[92m";
	else
		foreground = "\e[30m"; */

	std::cout << background << "  "
	          << "\e[0m";
	// std::cout << foreground << background << " " << dist << " " <<
	// std::setw(2) << id << " \e[0m";
	// std::cout << foreground << background << " " << std::setw(2)
	//           << (int)sq.cube_id << " " << std::setw(2) << (int)sq.rot_id
	//           << " \e[0m ";
	// std::cout << foreground << background << " " << (int)sq.rot_id << " \e[0m
	// ";
}

void print_line(const Cube &cube, int s, int l) {
	for (int c = 0; c < size; c++) {
		Square sq = cube[s][l][c];
		int uid = sq.get_uid(l, c);
		int dist = Heuristics::SquareDistance(
		    State::solution_cube[s][l][c].get_uid(c, l),
		    uid);
		Color color = State::solution_colors[Encoding::coord_from_uid(uid).f];

		bool correct = State::cube_from_id(ID())[s][l][c] == sq;

		display_square(sq, dist, correct, color);
	}
}

void print_map(const Cube &cube) {
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line(cube, 0, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		print_line(cube, 4, l);
		print_line(cube, 1, l);
		print_line(cube, 2, l);
		print_line(cube, 3, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line(cube, 5, l);
		std::cout << std::endl;
	}
}

void display_square_diff(bool correct, Color color) {
	string background;
	string foreground;

	switch (color) {
	case White:
		background = "\e[107m";
		break;
	case Red:
		background = "\e[41m";
		break;
	case Blue:
		background = "\e[44m";
		break;
	case Orange:
		background = "\e[43m";
		break;
	case Green:
		background = "\e[42m";
		break;
	case Yellow:
		background = "\e[103m";
		break;
	default:
		std::cout << "\e[32mERR\e[0m";
		return;
	}

	if (correct)
		foreground = "  ";
	else
		foreground = "\e[30m><";

	std::cout << background << foreground << "\e[0m";
}

void print_line_diff(const Cube &cube, const Cube &old, int s, int l) {
	for (int c = 0; c < size; c++) {
		Square sq = cube[s][l][c];
		Square sq_old = old[s][l][c];

		int uid = sq.get_uid(l, c);
		int uid_old = sq_old.get_uid(l, c);

		Color color = State::solution_colors[Encoding::coord_from_uid(uid).f];
		bool correct = uid == uid_old;

		display_square_diff(correct, color);
	}
}

void print_diff(const Cube &cube, const Cube &old) {
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line_diff(cube, old, 0, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		print_line_diff(cube, old, 4, l);
		print_line_diff(cube, old, 1, l);
		print_line_diff(cube, old, 2, l);
		print_line_diff(cube, old, 3, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line_diff(cube, old, 5, l);
		std::cout << std::endl;
	}
}

void print_map(const State &state) {
	Cube cube = state.get_cube();

	print_map(cube);
}

void print_line_dist(Coord pos, int s, int l) {
	for (int c = 0; c < size; c++) {
		Coord p = (Coord){s, l, c};
		int dist = Heuristics::SquareDistance(pos, p);
		int rdist = Heuristics::SquareDistance(p, pos);

		if (rdist != dist)
			std::cout << "\e[35m";
		if (dist == 0)
			if (p == pos)
				std::cout << " \e[44m0\e[0m";
			else
				std::cout << " _";
		else if (dist == 1)
			std::cout << " \e[46;30m1\e[0m";
		else if (dist == 2)
			std::cout << " \e[45;30m2\e[0m";
		else
			std::cout << dist << dist;
		if (rdist != dist)
			std::cout << "\e[0m";
	}
}

void print_dist(Coord pos) {
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line_dist(pos, 0, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		print_line_dist(pos, 4, l);
		print_line_dist(pos, 1, l);
		print_line_dist(pos, 2, l);
		print_line_dist(pos, 3, l);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		print_line_dist(pos, 5, l);
		std::cout << std::endl;
	}
}
