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
#include "Display.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "Tests.hpp"
#include "Types.hpp"
#include <chrono>
#include <ctime>

void print_update(Solver<ID>::Result &solverResult, struct timespec start) {
	struct timespec end;

	Display::clear_screen();
	Display::print_map(solverResult.state);
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
	Display::print_timediff("Time elapsed", start, end);
}

Solver<ID>::Result solve_loop(State<> &initial) {
	Solver<ID> puzzle(initial);
	Solver<ID>::Result solverResult(0, 0);
	struct timespec start;

	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		solverResult = puzzle.step();
		print_update(solverResult, start);
		//std::cout << "Memory repartition:" << std::endl;
		//puzzle.print_mem();
	} while (solverResult.finished == false);

	return solverResult;
}

int main(int ac, char **av) {
	State<> initial;
	Parser::ParseResult parseResult;

	Tests::encoding_tests<ID>();
	Tests::encoding_tests<IDG1>();
	Tests::encoding_tests<IDG2>();
	Tests::heuristic_tests();

	Display::clear_screen();

	//Create scrambles cube, from arg data or randomly
	parseResult = Parser::parse_args(ac, av);
	vector<Move> scramble = parseResult.get_data();
	initial = initial.get_scrambled(scramble);

	std::cout << "GENERATED CUBE" << std::endl;
	Display::print_map(initial);
	std::cout << "ATTEMPTING SOLUTION" << std::endl;

	//Solves the cube
	Solver<ID>::Result solverResult = solve_loop(initial);

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
			Display::clear_screen();
			for (auto &l : solverResult.movements)
				std::cout << l << std::endl;
			std::cout << std::endl
			          << std::flush;
			break;
		case 'a': {
			Display::print_animation(initial, solverResult.movements);
			break;
		}
		default:
			displayHelp = false;
			break;
		}
	}
}

#include "Database.cpp"
#include "Display.cpp"
#include "Heuristics.cpp"
#include "Move.cpp"
#include "Parser.cpp"
#include "State.cpp"
#include "Types.cpp"