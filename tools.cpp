/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 19:05:57 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/07 19:39:17 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"
#include "State.hpp"
#include "Heuristics.hpp"
#include <iomanip>

void display_square(Square c, int dist, bool correct) {

    string background;
    string foreground;

    Color color = State::solution_colors[State::solution_finder[c.face_id].f];

    switch (color) {
        case White: background = "\e[107m"; break;
        case Red: background = "\e[41m"; break;
        case Blue: background = "\e[44m"; break;
        case Orange: background = "\e[43m"; break;
        case Green: background = "\e[42m"; break;
        case Yellow: background = "\e[103m"; break;
        default: std::cout << "\e[32mERR\e[0m"; return;
    }

    if (correct)
        foreground = "\e[35m";
    else
        foreground = "\e[30m";

    std::cout << foreground << background << " " << dist << " " << std::setw(2) << (int)c.cube_id << " \e[0m";
}

void print_line(const Data& data, const Data& solution, int s, int l) {
    for (int c = 0; c < size; c++)
    {
        Square sq = data[s][l][c];
        int dist = Heuristics::SquareDistance((Coord){s, l, c}, State::solution_finder[sq.face_id]);
        bool correct = solution[s][l][c] == sq;

        display_square(sq, dist, correct);
    }
}

void	print_map(const State& state)
{
    const Data& solution = State::solution;
    Data *data = state.get_data_safe();

    for (int l = 0; l < size; l++) {
        std::cout << " _ __ " << " _ __ " << " _ __ ";
        print_line(*data, solution, 0, l);
        std::cout << std::endl;
    }
    for (int l = 0; l < size; l++) {
        print_line(*data, solution, 4, l);
        print_line(*data, solution, 1, l);
        print_line(*data, solution, 2, l);
        print_line(*data, solution, 3, l);
        std::cout << std::endl;
    }
    for (int l = 0; l < size; l++) {
        std::cout << " _ __ " << " _ __ " << " _ __ ";
        print_line(*data, solution, 5, l);
        std::cout << std::endl;
    }
    delete data;
}

void print_line_dist(Coord pos, int s, int l) {
    for (int c = 0; c < size; c++)
    {
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

void	print_dist(Coord pos)
{
    for (int l = 0; l < size; l++) {
        std::cout << "  " << "  " << "  ";
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
        std::cout << "  " << "  " << "  ";
        print_line_dist(pos, 5, l);
        std::cout << std::endl;
    }
}
