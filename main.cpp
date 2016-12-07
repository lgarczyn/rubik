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

#include "CliOptParser.hpp"
#include "Solver.hpp"
#include "Parser.hpp"
#include "tools.hpp"
#include "Types.hpp"
#include "Heuristics.hpp"
#include <random>
#include <unistd.h>
#include <termcap.h>
#include <unistd.h>


int		display_help(const char* path = "npuzzle")
{
	std::cout << "Usage: " << path << " [-h] " << std::endl
		<< "1: [-i ITERATION]" << std::endl
		<< "2: [-m MOVEMENTS]" << std::endl
	   	<< "[-f1] [-f2] [-f3]" << std::endl
	   	<< "[--greedy] [--uniform]" << std::endl;
	return (0);
}

Parser::ParseResult	parse_args(int ac, char **av)
{
	Parser::ParseResult result;
	try {
		char	buf[255];
		tgetent(buf, getenv("TERM"));

		// ARGS
		if (is_cmd_opt(av, av + ac, "-h"))
			exit(display_help(av[0]));
		// if (is_cmd_opt(av, av + ac, "-f1"))
		// 	Heuristics::HeuristicFunction = Heuristics::ManhattanDistance;
		// if (is_cmd_opt(av, av + ac, "-f2"))
		// 	Heuristics::HeuristicFunction = Heuristics::LinearConflict;
		// if (is_cmd_opt(av, av + ac, "-f3"))
		// 	Heuristics::HeuristicFunction = Heuristics::SuperSmartDistance;
		if (is_cmd_opt(av, av + ac, "--uniform"))
			State::get_index = State::indexer_uniform;
		if (is_cmd_opt(av, av + ac, "--greedy"))
			State::get_index = State::indexer_greedy;
		if (is_cmd_opt(av, av + ac, "--forget"))
			result.forget = true;
		if (is_cmd_opt(av, av + ac, "-i")) {
			result.iteration = std::stoi(get_cmd_opt(av, av + ac, "-i"));
            result.is_random = true;
        }
        if (is_cmd_opt(av, av + ac, "-m")) {
            result.data = get_cmd_opt(av, av + ac, "-m");
            result.is_random = false;
        }
		return (result);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

Solver::Result	solve_loop(State& initial, Parser::ParseResult& parseResult)
{
	Solver			puzzle(initial, parseResult.forget);
	Solver::Result	solverResult(0, 0);
	size_t 			it;

	it = 0;
	while (1)
	{
		solverResult = puzzle.step();
		if (solverResult.finished)
			break;

		if (it % 100000 == 0)
		{
			std::cout << tgetstr((char*)"cl", NULL);
			if (solverResult.actual_state != nullptr)
				print_map(*solverResult.actual_state);
			else
				std::cout << "null current state" << std::endl;
			std::cout << "Iteration count: " << it << std::endl;
			std::cout << "Solution [Score: " << solverResult.actual_state->get_weight() << "]" << std::endl;
            //std::cout << "Memory repartition:" << std::endl;
            //puzzle.print_mem();
		}
		++it;
	}
	std::cout << tgetstr((char*)"cl", NULL);
	if (solverResult.actual_state != nullptr)
		print_map(*solverResult.actual_state);
	else
		std::cout << "null current state" << std::endl;
	std::cout << "Iteration count: " << it << std::endl;
	std::cout << "Move count: " << solverResult.movements.size() << std::endl;

	return (solverResult);
}

int		                           main(int ac, char **av)
{
	StateRef					  initial;
	Parser::ParseResult           parseResult;

	parseResult = parse_args(ac, av);
    if (parseResult.is_random)
        initial = StateRef(new State(parseResult.iteration));
    else
        initial = StateRef(new State(parseResult.data));

    /*std::cout << "GENERATED CUBE" << std::endl;
    print_map(*initial);
    std::cout << "ATTEMPTING SOLUTION" << std::endl;*/

	Solver::Result solverResult = solve_loop(*initial, parseResult);

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
				<< "Solution move count: " << solverResult.movements.size() << std::endl
				<< std::endl << std::flush;
				break;
			case 's':
				std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				for (auto &l:solverResult.movements)
					std::cout << l << std::endl;
				std::cout << std::endl << std::flush;
				break;
			case 'a': {
				StateRef current = StateRef(new State(*initial));

				for (auto &l:solverResult.movements) {
					std::cout << tgetstr((char*)"cl", NULL) << std::endl;
					print_map(*current);
					std::cout << std::endl;
					usleep(500000);

					current = StateRef(new State(current.get(), l));
				}
				std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				print_map(*current);
				std::cout << std::endl << std::flush;
				usleep(500000);

				break;
			}
			default:
				displayHelp = false;
				break;
		}
	}
}
/*

constexpr int fact(int i) {
    int r = 1;
    for (int it = 1; it <= i; it++) {
        r *= it;
    }
    return r;
}
int main() {

	State a;
	Data d1 = a.get_data();




	a.deflate();
	a.inflate();


	Data d2 = a.get_data();

	std::cout
	<< (int)d1[Index_Up][0][0].cube_id << " "
	<< (int)d1[Index_Up][0][2].cube_id << " "
	<< (int)d1[Index_Up][2][0].cube_id << " "
	<< (int)d1[Index_Up][2][2].cube_id << " "
	<< (int)d1[Index_Down][0][0].cube_id << " "
	<< (int)d1[Index_Down][0][2].cube_id << " "
	<< (int)d1[Index_Down][2][0].cube_id << " "
	<< (int)d1[Index_Down][2][2].cube_id << std::endl;

	std::cout
	<< (int)d2[Index_Up][0][0].cube_id << " "
	<< (int)d2[Index_Up][0][2].cube_id << " "
	<< (int)d2[Index_Up][2][0].cube_id << " "
	<< (int)d2[Index_Up][2][2].cube_id << " "
	<< (int)d2[Index_Down][0][0].cube_id << " "
	<< (int)d2[Index_Down][0][2].cube_id << " "
	<< (int)d2[Index_Down][2][0].cube_id << " "
	<< (int)d2[Index_Down][2][2].cube_id << std::endl;

	std::cout
	<< (int)d1[Index_Up][0][1].cube_id << " "
	<< (int)d1[Index_Up][1][0].cube_id << " "
	<< (int)d1[Index_Up][2][1].cube_id << " "
	<< (int)d1[Index_Up][1][2].cube_id << " "
	<< (int)d1[Index_Front][1][0].cube_id << " "
	<< (int)d1[Index_Right][1][0].cube_id << " "
	<< (int)d1[Index_Back][1][0].cube_id << " "
	<< (int)d1[Index_Left][1][0].cube_id << " "
	<< (int)d1[Index_Down][0][1].cube_id << " "
	<< (int)d1[Index_Down][1][0].cube_id << " "
	<< (int)d1[Index_Down][2][1].cube_id << " "
	<< (int)d1[Index_Down][1][2].cube_id << std::endl;

	std::cout
	<< (int)d2[Index_Up][0][1].cube_id << " "
	<< (int)d2[Index_Up][1][0].cube_id << " "
	<< (int)d2[Index_Up][2][1].cube_id << " "
	<< (int)d2[Index_Up][1][2].cube_id << " "
	<< (int)d2[Index_Front][1][0].cube_id << " "
	<< (int)d2[Index_Right][1][0].cube_id << " "
	<< (int)d2[Index_Back][1][0].cube_id << " "
	<< (int)d2[Index_Left][1][0].cube_id << " "
	<< (int)d2[Index_Down][0][1].cube_id << " "
	<< (int)d2[Index_Down][1][0].cube_id << " "
	<< (int)d2[Index_Down][2][1].cube_id << " "
	<< (int)d2[Index_Down][1][2].cube_id << std::endl;

	for (int s = Index_Start; s < Index_Len; s++) {
		for (int l = 0; l < size; l++) {
			for (int c = 0; c < size; c++)
            {
				Square as = d1[s][l][c];
				Square bs = d2[s][l][c];
				std::cout << "  {" << (int)as.cube_id << "=" << (int)bs.cube_id << " " << (int)as.face_id << "=" << (int)bs.face_id << "}  ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	if (d1 == d2)
		std::cout << "FUCK YES" << std::endl;
}*/

/*
int main(int argc, char const *argv[]) {

	if (argc != 2)
		return (1);

	string scramble = string(argv[1]);
	State initial = State(scramble);
	solve_loop(initial);


	(void)argc;
	(void)argv;

	State s;

	while (1) {
		string line;

		std::cin >> line;

		s.applyScramble(line);

		print_map(s);
	}

	//Test heuristic validity/quality
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0,20); // guaranteed unbiased

	for (int i = 0; i < 100000; i++) {

		if (i % 1000 == 0)
			std::cout << i << std::endl;
		int s_count = uni(rng);

		State *s = get_random_state(s_count);
		int r = Heuristics::HeuristicFunction(s->get_data(), s->get_finder());

		if (r > s_count)
			std::cout << "Error, heuristic result is bigger than solution" << std::endl;

	}

	//Test every Movement
	check_movement("");
	check_movement("U");
	check_movement("D");
	check_movement("R");
	check_movement("L");
	check_movement("F");
	check_movement("B");
	check_movement("U'");
	check_movement("D'");
	check_movement("R'");
	check_movement("L'");
	check_movement("F'");
	check_movement("B'");
	check_movement("U2");
	check_movement("D2");
	check_movement("R2");
	check_movement("L2");
	check_movement("F2");
	check_movement("B2");

	//isplay distance for entry position and every other
	while (1) {
		int f;
		int l;
		int c;

		std::cin >> f;
		std::cin >> l;
		std::cin >> c;

		print_dist((Coord){f, l, c});
	}


	return 0;
}*/
