/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:41:57 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/21 01:49:59 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Database.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "Types.hpp"
#include "tools.hpp"
#include <chrono>
#include <ctime>

void print_state(Solver::Result &solverResult, struct timespec start) {
	struct timespec end;

	clear_screen();
	print_map(solverResult.state);
	std::cout << "Iteration count: " << solverResult.timeComplexity << std::endl;
	std::cout << "Solution [Score: "
	          << (int)solverResult.weight
	          << "] [ Distance: "
	          << (int)solverResult.movements.size()
	          << "]"
	          << std::endl;
	for (auto &l : solverResult.movements)
		std::cout << l;
	std::cout << std::endl;
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_timediff("Time elapsed", start, end);
}

#include <iomanip>
#include <unistd.h>

Solver::Result solve_loop(State &initial) {
	Solver puzzle(initial);
	Solver::Result solverResult(0, 0);
	struct timespec start;

	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		solverResult = puzzle.step();
		print_state(solverResult, start);
		//std::cout << "Memory repartition:" << std::endl;
		//puzzle.print_mem();
	} while (solverResult.finished == false);

	return solverResult;
}

int main(int ac, char **av) {
	State initial;
	Parser::ParseResult parseResult;

	clear_screen();

	//Create scrambles cube, from arg data or randomly
	parseResult = Parser::parse_args(ac, av);
	vector<Move> scramble = parseResult.get_data();
	initial = initial.get_scrambled(scramble);

	std::cout << "GENERATED CUBE" << std::endl;
	print_map(initial);
	std::cout << "ATTEMPTING SOLUTION" << std::endl;

	//Solves the cube
	Solver::Result solverResult = solve_loop(initial);

	//Offer multiple data visualisation options
	bool displayHelp = true;
	while (1) {
		char c;
		if (displayHelp)
			std::cout << "Press:" << std::endl
			          << "\t[q] to quit" << std::endl
			          << "\t[d] to display data" << std::endl
			          << "\t[s] to display solution" << std::endl
			          << "\t[a] to display animation" << std::endl
			          << std::endl
			          << std::flush;

		displayHelp = true;
		c = std::getchar();
		switch (c) {
		case 'q':
			return (0);
		case 'd':

			std::cout << "Total number of states ever in open set: "
			          << solverResult.sizeComplexity << std::endl
			          << "Max number of states concurrent in memory: "
			          << solverResult.timeComplexity << std::endl
			          << "Solution move count: "
			          << solverResult.movements.size() << std::endl
			          << std::endl
			          << std::flush;
			break;
		case 's':
			clear_screen();
			for (auto &l : solverResult.movements)
				std::cout << l << std::endl;
			std::cout << std::endl
			          << std::flush;
			break;
		case 'a': {
			print_animation(initial, solverResult.movements);
			break;
		}
		default:
			displayHelp = false;
			break;
		}
	}
}
