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

#include "Database.hpp"
#include "Display.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "Tests.hpp"
#include "Types.hpp"
#include <chrono>
#include <ctime>
#include <iostream>

void print_update(Result &solverResult, struct timespec start) {
	struct timespec end;

	Display::print_map(solverResult.cube);
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

Result solve_loop(State<> &initial) {
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);

	Solver<ID> puzzle(initial);
	Result solverResult;

	do {
		solverResult = puzzle.step();
		Display::clear_screen();
		print_update(solverResult, start);
		//std::cout << "Memory repartition:" << std::endl;
		//puzzle.print_mem();
	} while (solverResult.finished == false);

	return solverResult;
}

Result solve_loop_kociemba(State<> initial) {
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);

	Result resultG1;
	{
		Solver<IDG1> puzzle(State<IDG1>(Encoding::id_from_data<IDG1>(initial.get_data()), Move()));

		do {
			resultG1 = puzzle.step();
			Display::clear_screen();
			std::cout << "Phase 1:" << std::endl;
			print_update(resultG1, start);
			//std::cout << "Memory repartition:" << std::endl;
			//puzzle.print_mem();
		} while (resultG1.finished == false);
	}

	initial = initial.get_scrambled(resultG1.movements);
	if (resultG1.movements.size())
		initial = State<>(initial.get_id(), *resultG1.movements.rbegin());

	std::cout << "PHASE 1 OVER" << std::endl;
	for (auto &l : resultG1.movements)
		std::cout << l;
	std::cout << std::endl;
	if (Encoding::id_from_data<IDG1>(initial.get_data()) != IDG1())
		throw std::logic_error("Phase 1 reconstruction doesn't belong to G1");

	Result resultG2;
	{
		Solver<IDG2> puzzle(State<IDG2>(Encoding::id_from_data<IDG2>(initial.get_data()), Move()));
		do {
			resultG2 = puzzle.step();
			Display::clear_screen();
			std::cout << "Phase 2:" << std::endl;
			print_update(resultG2, start);
			//std::cout << "Memory repartition:" << std::endl;
			//puzzle.print_mem();
		} while (resultG2.finished == false);
	}
	initial = initial.get_scrambled(resultG2.movements);

	Result result = Result(resultG1, resultG2);
	result.cube = initial.get_cube();

	std::cout << "PHASE 2 OVER" << std::endl;
	for (auto &l : resultG2.movements)
		std::cout << l;
	std::cout << std::endl;
	if (Encoding::id_from_data<IDG2>(initial.get_data()) != IDG2())
		throw std::logic_error("Phase 2 reconstruction isn't solved");

	return result;
}

int main(int ac, char **av) {
	State<> initial;
	Parser::ParseResult parseResult;

	Tests::encoding_tests<ID>();
	Tests::encoding_tests<IDG1>();
	Tests::encoding_tests<IDG2>();
	Tests::heuristic_tests();
	Tests::commutativity_tests();

	//Create scrambles cube, from arg data or randomly
	parseResult = Parser::parse_args(ac, av);
	vector<Move> scramble = parseResult.get_data();
	initial = initial.get_scrambled(scramble);

	// Tests::rotation_tests<ID>();
	// Data tmp = initial.get_data();
	// Tests::rotation_tests<IDG1>(tmp);
	// tmp = Tests::rotation_tests<ID>(tmp);
	// tmp = Tests::rotation_tests<IDG2>(tmp);
	// std::cout << "rot test done: " << std::endl;
	// Display::print_map(State<>(Encoding::id_from_data<ID>(tmp), Move()));
	// Display::print_data(tmp);

	// int c;
	// std::cin >> c;

	Display::clear_screen();

	std::cout << "GENERATED CUBE" << std::endl;
	Display::print_map(initial);
	Display::print_data(initial.get_data());
	Display::print_id(initial.get_id());
	std::cout << "ATTEMPTING SOLUTION" << std::endl;

	//Solves the cube
	//Result solverResult = solve_loop(initial);
	Result solverResult = solve_loop_kociemba(initial);

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
			std::cout << std::endl;
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