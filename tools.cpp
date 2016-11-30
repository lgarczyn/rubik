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
#include <iomanip>

void display_square(Square c, bool correct) {

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

    std::cout << foreground << background << " " << std::setw(2) << c.cube_id << " \e[0m";
}

void print_line(const Data& data, const Data& solution, int s, int x) {
    for (int y = 0; y < size; y++)
    {
        Square c = data[s][x][y];
        bool correct = solution[s][x][y] == c;

        display_square(c, correct);
    }
}

void	print_map(const Data& data, const Data& solution)
{
    for (int x = 0; x < size; x++) {
        std::cout << " __ " << " __ " << " __ ";
        print_line(data, solution, 0, x);
        std::cout << std::endl;
    }
    for (int x = 0; x < size; x++) {
        print_line(data, solution, 4, x);
        print_line(data, solution, 1, x);
        print_line(data, solution, 2, x);
        print_line(data, solution, 3, x);
        std::cout << std::endl;
    }
    for (int x = 0; x < size; x++) {
        std::cout << " __ " << " __ " << " __ ";
        print_line(data, solution, 5, x);
        std::cout << std::endl;
    }
}
