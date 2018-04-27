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
#include "OptParser.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "Types.hpp"
#include "tools.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <termcap.h>
#include <unistd.h>
#include <unistd.h>

void clear_screen() {
	//system("clear");
	//std::cout << tgetstr((char *)"cl", NULL);
}

void print_timediff(const char *prefix, const struct timespec &start, const struct timespec &end) {
	double milliseconds = (end.tv_nsec - start.tv_nsec) / 1e6 + (end.tv_sec - start.tv_sec) * 1e3;
	printf("%s: %lf seconds\n", prefix, milliseconds / 1000.0f);
}

int display_help(const char *path = "npuzzle") {
	std::cout << "Usage: " << path << " [-h] " << std::endl
	          << "1: [-i ITERATION]" << std::endl
	          << "2: [-m MOVEMENTS]" << std::endl
	          << "[-c CLEAN_STEPS]" << std::endl
	          << "[-f1] [-f2] [-f3]" << std::endl
	          << "[--greedy] [--uniform]" << std::endl;
	return 0;
}

void display_animation(State state, std::vector<uchar> &movements) {
	Cube old_cube, cube;
	old_cube = cube = state.get_cube();

	for (uint16_t l : movements) {
		Cube new_cube;

		clear_screen();
		print_diff(cube, old_cube);
		usleep(1000000);

		state = State(state, (State::Movement)l);
		new_cube = state.get_cube();

		clear_screen();
		print_diff(cube, new_cube);
		usleep(1000000);

		old_cube = cube;
		cube = new_cube;
	}
	clear_screen();
	print_diff(cube, old_cube);
	usleep(1000000);
	clear_screen();
	print_map(cube);
	usleep(1000000);
}

Parser::ParseResult parse_args(unsigned int ac, char **av) {
	Parser::ParseResult result;
	try {
		char buf[255];
		tgetent(buf, getenv("TERM"));

		char *cmd;

		// skipping first arg
		ac--;
		av++;

		if (is_cmd_opt(av, ac, "-h"))
			exit(display_help(av[0]));
		cmd = get_cmd_opt(av, ac, "-i");
		if (cmd) {
			result.iteration = std::stoi(cmd);
			result.is_random = true;
		}
		cmd = get_cmd_opt(av, ac, "-m");
		if (cmd) {
			result.data = cmd;
			result.is_random = false;
		}
		cmd = get_cmd_opt(av, ac, "-c");
		if (cmd) {
			result.clean_steps = std::stoi(cmd);
		}
		return result;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

Solver::Result solve_loop(State &initial, int clean_steps) {
	Solver puzzle(initial, clean_steps == 0);
	Solver::Result solverResult(0, 0);
	size_t it;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);
	it = 1;
	while (1) {
		solverResult = puzzle.step();
		if (solverResult.finished)
			break;

		/*if (clean_steps > 1 && it % clean_steps == 0) {
			std::cout << "cleaning duplicates:\n";
			int c = puzzle.clean_duplicates();
			std::cout << c << "/" << puzzle._openCount << std::endl;
		}*/

		if (it % 100000 == 0) {
			clear_screen();
			print_map(solverResult.actual_state);
			std::cout << "Iteration count: " << it << std::endl;
			std::cout << "Solution [Score: "
			          << solverResult.actual_state.get_weight()
			          << "] [ Distance: "
			          << solverResult.actual_state.get_distance()
			          << "]"
			          << std::endl;
			//std::cout << "Memory repartition:" << std::endl;
			//puzzle.print_mem();
		}
		++it;
	}
	clear_screen();
	print_map(solverResult.actual_state);
	std::cout << "Iteration count: " << it << std::endl;
	std::cout << "Move count: " << solverResult.movements.size() << std::endl;
	clock_gettime(CLOCK_MONOTONIC, &end);
	print_timediff("Time elapsed", start, end);

	return solverResult;
}

int main(int ac, char **av) {
	State initial;
	Parser::ParseResult parseResult;

	clear_screen();

	parseResult = parse_args(ac, av);
	if (parseResult.is_random)
		initial = State(parseResult.iteration);
	else
		initial = State(parseResult.data);

	std::cout << "GENERATED CUBE" << std::endl;
	print_map(initial);
	std::cout << "ATTEMPTING SOLUTION" << std::endl;

	Solver::Result solverResult = solve_loop(initial, parseResult.clean_steps); //, parseResult);

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
			exit(0);
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
				std::cout << (State::Movement)l << std::endl;
			std::cout << std::endl
			          << std::flush;
			break;
		case 'a': {
			display_animation(initial, solverResult.movements);
			break;
		}
		default:
			displayHelp = false;
			break;
		}
	}
}
