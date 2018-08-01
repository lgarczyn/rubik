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

#include "Display.hpp"
#include "Encoding.hpp"
#include "Heuristics.hpp"
#include "State.hpp"
#include <cstdlib>
#include <iomanip>
#include <unistd.h>

const char *get_color(Color color) {
	switch (color) {
	case White:
		return "\e[107m";
	case Red:
		return "\e[41m";
	case Blue:
		return "\e[44m";
	case Orange:
		return "\e[45m";
	case Green:
		return "\e[42m";
	case Yellow:
		return "\e[103m";
	default:
		return "\e[32mERR\e[0m";
	}
}

void print_square(Square sq, int dist, bool correct, Color color) {
	string background;
	//string foreground;
	(void)dist;
	(void)sq;
	(void)correct;

	/* if (correct)
		foreground = "\e[92m";
	else
		foreground = "\e[30m"; */

	std::cout << get_color(color) << "  "
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
		    Encoding::solution_cube[s][l][c].get_uid(c, l),
		    uid);

		Color color = (Color)Encoding::solution_finder[uid].f;

		bool correct = Encoding::solution_cube[s][l][c] == sq;

		print_square(sq, dist, correct, color);
	}
}

void Display::print_map(const Cube &cube) {
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

void print_square_diff(Color color, const char *text) {
	const char *background;
	const char *foreground;

	background = get_color(color);
	foreground = "  ";

	if (text)
		foreground = text;

	std::cout << background << "\e[30m" << foreground << "\e[0m";
}

void print_line_diff(const Cube &cube, int face, int line, const char **text) {

	for (int c = 0; c < size; c++) {
		Square sq = cube[face][line][c];

		int uid = sq.get_uid(line, c);

		Color color = (Color)Encoding::solution_finder[uid].f;

		if (text)
			print_square_diff(color, text[c]);
		else
			print_square_diff(color, nullptr);
	}
}

int get_face(Move::Direction dir) {
	switch (dir) {
	case Move::Up:
		return Index_U;
	case Move::Right:
		return Index_R;
	case Move::Down:
		return Index_D;
	case Move::Left:
		return Index_L;
	case Move::Front:
		return Index_F;
	case Move::Back:
		return Index_B;
	default:
		return 0;
	}
}

const char *arrows[3][3][3] = {
    {{"↗ ", "-→", " ↘"},
        {"↑ ", "  ", " ↓"},
        {"↖ ", "←-", " ↙"}},
    {{"↗ ", "-→", " ↘"},
        {"↑ ", "X2", " ↓"},
        {"↖ ", "←-", " ↙"}},
    {{"↙ ", "←-", " ↖"},
        {"↓ ", "  ", " ↑"},
        {"↘ ", "-→", " ↗"}}};

void Display::print_diff(const Cube &cube, Move m) {
	int turns = m.get_turns() - 1;
	int face = get_face(m.direction);

	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		const char **arrow = arrows[turns][l];
		print_line_diff(cube, Index_U, l, face == Index_U ? arrow : NULL);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		const char **arrow = arrows[turns][l];
		print_line_diff(cube, Index_L, l, face == Index_L ? arrow : NULL);
		print_line_diff(cube, Index_F, l, face == Index_F ? arrow : NULL);
		print_line_diff(cube, Index_R, l, face == Index_R ? arrow : NULL);
		print_line_diff(cube, Index_B, l, face == Index_B ? arrow : NULL);
		std::cout << std::endl;
	}
	for (int l = 0; l < size; l++) {
		std::cout << "  "
		          << "  "
		          << "  ";
		const char **arrow = arrows[turns][l];
		print_line_diff(cube, Index_D, l, face == Index_D ? arrow : NULL);
		std::cout << std::endl;
	}
}

void Display::print_map(const IState &state) {
	Cube cube = state.get_cube();

	print_map(cube);
}

void print_line_dist(Coord pos, uchar s, uchar l) {
	for (uchar c = 0; c < size; c++) {
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

void Display::clear_screen() {
	//int r = system("clear");
	//(void)r;
}

void Display::print_timediff(
    const char *prefix,
    const struct timespec &start,
    const struct timespec &end) {
	double milliseconds = (end.tv_nsec - start.tv_nsec) / 1e6 + (end.tv_sec - start.tv_sec) * 1e3;
	printf("%s: %lf seconds\n", prefix, milliseconds / 1000.0f);
}