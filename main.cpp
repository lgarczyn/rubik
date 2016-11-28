/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 14:43:29 by edelangh          #+#    #+#             */
/*   Updated: 2016/04/07 20:16:13 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Parser.hpp"
#include "CliOptParser.hpp"
#include "Generator.hpp"
#include "Solver.hpp"
#include "tools.h"
#include <unistd.h>
#include <termcap.h>
#include <unistd.h>

int		display_help(const char* path = "npuzzle")
{
	std::cout << "Usage: " << path << " [-h] " << std::endl
		<< "1: [-w WIDTH] [-i ITERATION] [-u] [-s]" << std::endl
		<< "2: [-m FILE]" << std::endl
		<< "3: [-s]" << std::endl
		<< "[--rect]" << std::endl
	   	<< "[-f1] [-f2] [-f3]" << std::endl
	   	<< "[--greedy] [--uniform]" << std::endl;
	return (0);
}

Parser::ParseResult	get_result(int ac, char **av)
{
	Parser::ParseResult	result;
	Parser				p;

	if (is_cmd_opt(av, av + ac, "-m")) {
		result = p.parse_file(get_cmd_opt(av, av + ac, "-m"));
	}
	else if (is_cmd_opt(av, av + ac, "-w")) {
		result.width = std::stoi(get_cmd_opt(av, av + ac, "-w"));
		if (result.width < 3) {
			std::cerr << av[0] << ": width too small" << std::endl;
			exit(1);
		}
		result.height = result.width;
		result.shouldGenerate = true;
	}
	else if (is_cmd_opt(av, av + ac, "-s"))
		result = p.parse_istream(std::cin);
	else
	{
		display_help(av[0]);
		exit(0);
	}
	return (result);
}

Parser::ParseResult	parse_args(int ac, char **av)
{
	Parser::ParseResult result;
	try {
		char	buf[255];
		// INIT
		std::srand(std::time(0));
		tgetent(buf, getenv("TERM"));

		// ARGS
		if (is_cmd_opt(av, av + ac, "-h"))
			exit(display_help(av[0]));
		if (is_cmd_opt(av, av + ac, "-f1"))
			Heuristics::HeuristicFunction = Heuristics::ManhattanDistance;
		if (is_cmd_opt(av, av + ac, "-f2"))
			Heuristics::HeuristicFunction = Heuristics::LinearConflict;
		if (is_cmd_opt(av, av + ac, "-f3"))
			Heuristics::HeuristicFunction = Heuristics::SuperSmartDistance;
		if (is_cmd_opt(av, av + ac, "--uniform"))
			State::get_index = State::indexer_uniform;
		if (is_cmd_opt(av, av + ac, "--greedy"))
			State::get_index = State::indexer_greedy;
		if (is_cmd_opt(av, av + ac, "--rect"))
			Parser::allow_rectangle = true;
		result = get_result(ac, av);
		if (is_cmd_opt(av, av + ac, "--forget"))
			result.forget = true;
		if (is_cmd_opt(av, av + ac, "-i"))
			Generator::iteration = std::stoi(get_cmd_opt(av, av + ac, "-i"));

		if (is_cmd_opt(av, av + ac, "-c"))
			sscanf(get_cmd_opt(av, av + ac, "-c"), "%zu", &result.search_step);
		State::init(result.width, result.height);

		if (result.shouldGenerate)
		{
			if (is_cmd_opt(av, av + ac, "-s"))
				result.data = Generator::gen_solvable(result);
			else if (is_cmd_opt(av, av + ac, "-u"))
				result.data = Generator::gen_unsolvable(result);
			else
				result.data = Generator::gen_random(result);
		}
		return (result);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

Solver::Result	solve_loop(State *initial, Parser::ParseResult&parseResult)
{
	Solver			puzzle(initial, parseResult.forget);
	Solver::Result	solverResult(0, 0);
	size_t 			it;

	it = 0;
	do {
		while (!(solverResult = puzzle.step()).finished)
		{
			if (it % 100000 == 0)
			{
				std::cout << tgetstr((char*)"cl", NULL);
				print_map(solverResult.actual_state->get_data(), State::solution);
				std::cout << "Iteration count: " << it << std::endl;
				std::cout << "Solution [score: " << solverResult.actual_state->get_weight() << "]" << std::endl;
			}
			++it;
		}
		std::cout << tgetstr((char*)"cl", NULL);
		print_map(solverResult.actual_state->get_data(), State::solution);
		std::cout << "Iteration count: " << it << std::endl;
		std::cout << "Move count: " << solverResult.movements->size() << std::endl;
	} while ((parseResult.search_step && solverResult.movements->size() > parseResult.search_step));

	return (solverResult);
}

int		main(int ac, char **av)
{
	State*					initial;
	Parser::ParseResult parseResult;

	parseResult = parse_args(ac, av);
	initial = new State(parseResult.data);
	if (initial->is_solvable() == State::Valid)
	{
		std::cout << av[0] << ": Puzzle is solvable" << std::endl << std::flush;
	}
	else if (initial->is_solvable() == State::Impossible)
	{
		std::cerr << av[0] << ": Puzzle is unsolvable" << std::endl << std::flush;
		exit(1);
	}
	else
	{
		std::cerr << av[0] << ": Puzzle is broken" << std::endl << std::flush;
		exit(1);
	}

	Solver::Result solverResult = solve_loop(initial, parseResult);

	bool displayHelp = true;
	while (1)
	{
		char	c;
		if (displayHelp)
			std::cout << "Press:" << std::endl
				<< "\t[q] to quit" << std::endl
				<< "\t[d] to display data" << std::endl
				<< "\t[s] to display solution" << std::endl
				<< "\t[a] to display animation" << std::endl
				<< std::endl << std::flush;

		displayHelp = true;
		c = std::getchar();
		switch(c)
		{
			case 'q':
				exit(0);
			case 'd':
				std::cout << tgetstr((char*)"cl", NULL);
				std::cout
				<< "Total number of states ever in open set: " << solverResult.sizeComplexity << std::endl
				<< "Max number of states concurrent in memory: " << solverResult.timeComplexity << std::endl
				<< "Solution move count: " << solverResult.movements->size() << std::endl
				<< std::endl << std::flush;
				break;
			case 's':
				std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				for (auto &x:*solverResult.movements)
					std::cout << x << std::endl;
				std::cout << std::endl << std::flush;
				break;
			case 'a': {
				State *current = new State(initial->get_data());

				for (auto &x:*solverResult.movements) {
					std::cout << tgetstr((char*)"cl", NULL) << std::endl;
					print_map(current->get_data(), State::solution);
					std::cout << std::endl;
					usleep(500000);

					State *tmp = new State(current, x);
					delete current;
					current = tmp;
				}
				std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				print_map(current->get_data(), State::solution);
				std::cout << std::endl << std::flush;
				usleep(500000);
				delete current;

				break;
			}
			default:
				displayHelp = false;
				break;
		}
	}
}
