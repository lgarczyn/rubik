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

    switch (c.color) {
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

    std::cout << foreground << background << " " << dist << " " << std::setw(2) << c.cube_id << " \e[0m";
}

void print_line(const Data& data, const Data& solution, const Finder& finder, int s, int x) {
    for (int y = 0; y < size; y++)
    {
        Square c = data[s][x][y];
        int dist = Heuristics::SquareDistance((Coord){s, x, y}, finder[c.face_id]);
        bool correct = solution[s][x][y] == c;

        display_square(c, dist, correct);
    }
}

void	print_map(const State& state)
{
    const Data& solution = State::solution;
    const Data& data = state.get_data();
    const Finder& finder = state.get_finder();

    for (int x = 0; x < size; x++) {
        std::cout << " _ __ " << " _ __ " << " _ __ ";
        print_line(data, solution, finder, 0, x);
        std::cout << std::endl;
    }
    for (int x = 0; x < size; x++) {
        print_line(data, solution, finder, 4, x);
        print_line(data, solution, finder, 1, x);
        print_line(data, solution, finder, 2, x);
        print_line(data, solution, finder, 3, x);
        std::cout << std::endl;
    }
    for (int x = 0; x < size; x++) {
        std::cout << " _ __ " << " _ __ " << " _ __ ";
        print_line(data, solution, finder, 5, x);
        std::cout << std::endl;
    }
}
