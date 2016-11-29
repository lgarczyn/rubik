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

string get_char(Color c, bool correct) {

    string output;

    if (correct)
        output = "\e[32m";
    else
        output = "\e[31m";

    switch (c) {
        case White: return output + "\e[107m  \e[0m";
        case Red: return output + "\e[41m  \e[0m";
        case Blue: return output + "\e[44m  \e[0m";
        case Orange: return output + "\e[7;49;33m  \e[0m";
        case Green: return output + "\e[42m  \e[0m";
        case Yellow: return output + "\e[103m  \e[0m";
    }
    return "\e[32mE\e[0m";
}

void print_line(const Data& data, const Data& solution, int s, int x) {
    for (int y = 0; y < size; y++)
    {
        Color c = data[s][x][y];
        Color cs = solution[s][x][y];
        bool correct = c == cs;

        std::cout << get_char(c, correct);
    }
}

void	print_map(const Data& data, const Data& solution)
{
    for (int x = 0; x < size; x++) {
        std::cout << std::string(6, ' ');
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
        std::cout << std::string(6, ' ');
        print_line(data, solution, 5, x);
        std::cout << std::endl;
    }
}
