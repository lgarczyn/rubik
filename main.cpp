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
#include "Database.hpp"
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

Solver::Result	solve_loop(State& initial)//, Parser::ParseResult& parseResult)
{
	Solver			puzzle(initial);
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
			//std::cout << tgetstr((char*)"cl", NULL);
			print_map(solverResult.actual_state);
			std::cout << "Iteration count: " << it << std::endl;
			std::cout << "Solution [Score: " << solverResult.actual_state.get_weight() << "]" << std::endl;
            std::cout << "Memory repartition:" << std::endl;
            puzzle.print_mem();
		}
		++it;
	}
	//std::cout << tgetstr((char*)"cl", NULL);
	print_map(solverResult.actual_state);
	std::cout << "Iteration count: " << it << std::endl;
	std::cout << "Move count: " << solverResult.movements.size() << std::endl;

	return (solverResult);
}
constexpr int pow(int a, int b) {
    int r = 1;
    for (int it = 0; it < b; it++) {
        r *= a;
    }
    return r;
}
constexpr int fact(int i) {
    int r = 1;
    for (int it = 1; it <= i; it++) {
        r *= it;
    }
    return r;
}

const int corner_length = pow(3, 8) * fact(8);

int		                          main2(int ac, char **av)
{
	State						  initial;
	Parser::ParseResult           parseResult;

	parseResult = parse_args(ac, av);
    if (parseResult.is_random)
        initial = State(parseResult.iteration);
    else
        initial = State(parseResult.data);

    std::cout << "GENERATED CUBE" << std::endl;
    print_map(initial);
    std::cout << "ATTEMPTING SOLUTION" << std::endl;

	Solver::Result solverResult = solve_loop(initial);//, parseResult);

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
				//std::cout << tgetstr((char*)"cl", NULL);
				std::cout
				<< "Total number of states ever in open set: " << solverResult.sizeComplexity << std::endl
				<< "Max number of states concurrent in memory: " << solverResult.timeComplexity << std::endl
				<< "Solution move count: " << solverResult.movements.size() << std::endl
				<< std::endl << std::flush;
				break;
			case 's':
				//std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				for (auto &l:solverResult.movements)
					std::cout << l << std::endl;
				std::cout << std::endl << std::flush;
				break;
			case 'a': {
				State current = State(initial);

				for (uint16_t l:solverResult.movements) {
					//std::cout << tgetstr((char*)"cl", NULL) << std::endl;
					print_map(current);
					std::cout << std::endl;
					usleep(500000);

					current = State(current, (State::Movement)l);
				}
				//std::cout << tgetstr((char*)"cl", NULL) << std::endl;
				print_map(current);
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

uint floor_index_upper_corners(uint u) {

	uint p = u / pow(3, 8);
	uint r = u % pow(3, 8);

	uint pu = p / fact(4);
	uint ru = r / pow(3, 4);
	return (pu * fact(4) * pow(3, 8)) + ru *  pow(3, 4);
}

int main2() {;

	/*{
		std::ifstream f = std::ifstream("upper_corners.db");
		Databases::upper_corners = Database(corner_length);
		f >> Databases::upper_corners;
	}
	{
		std::ifstream f = std::ifstream("lower_corners.db");
		Databases::lower_corners = Database(corner_length);
		f >> Databases::lower_corners;
	}
	{
		std::ifstream f = std::ifstream("corners.db");
		Databases::corners = Database(corner_length);
		f >> Databases::corners;
	}*/

	/*for (int i = 0; i < corner_length; i++) {
		uchar uc = Databases::upper_corners[i];
		uchar lc = Databases::lower_corners[i];
		uchar c = Databases::corners[i];

		std::cerr << i << " " << (int)c << " " << (int)uc << " " << (int)lc << std::endl;
	}*/


	Database d = Database(corner_length);

	std::cerr << d.size() << " " << Databases::upper_corners.size() << " " << Databases::lower_corners.size() << "\n";

	State s = State();
	Solver solver;
	for (uint i = 0; i < corner_length; i++) {

		uint pre_i = floor_index_upper_corners(i);

		if (pre_i < i) { //if the last result is the same as the current one, use it
			d[i] = d[pre_i];
		} else {//else, calculate it

			s._get_id().corners = i;
			s.update_weight();
			solver.setup(s);
			Solver::Result res;
			while (1) {
				res = solver.step();
				if (res.finished)
					break;
			}

			d[i] = res.movements.size();//Store the movement solution length to the databse, and to prev_result
			//Store the index of the last solved positon, to allow current database lookup
		}
		Databases::current_index = i;
		if (i % 100 == 0) {
			s._get_id().corners = i;
			s.update_weight();
			print_map(s);
			std::cout << i << " " << (int)d[i] << std::endl;
		}
	}
	//f.close();
	std::ofstream of = std::ofstream("corners.db");
	of << d;
	return (0);
}

int main10() {
	State s;

	print_map(s);
	while (1) {
		string line;

		std::cin >> line;

		s = State(line);

		print_map(s);
	}
}

int main() {
	Data d;
	Cube c;

	print_map(State::solution);
	while (1) {
		c = State::solution;
		d = State::data_from_id(ID());

		string line;

		std::cin >> line;


		State::_apply_scramble(c, line);
		State::_apply_scramble(d, line);

		print_map(c);
		std::cout << std::endl;
		print_map(State::cube_from_id(State::id_from_data(d)));

		std::cout << std::endl;
	}
}


/*
int main() {

	State a = State(100);
	Cube d1 = a.get_data();

	uint n2 = a.get_id().borders_rot;
	uint n1 = a.get_id().corners % pow(3, 8);


	a.deflate();
	a.inflate();


	Cube d2 = a.get_data();

	std::cout
	<< (int)d1[Index_U][0][0].rot_id << " "
	<< (int)d1[Index_U][0][2].rot_id << " "
	<< (int)d1[Index_U][2][0].rot_id << " "
	<< (int)d1[Index_U][2][2].rot_id << " "
	<< (int)d1[Index_D][0][0].rot_id << " "
	<< (int)d1[Index_D][0][2].rot_id << " "
	<< (int)d1[Index_D][2][0].rot_id << " "
	<< (int)d1[Index_D][2][2].rot_id << std::endl;

	for (int i = 7; i >= 0; i--)
		std::cout << (n1 / pow(3, i)) % 3 << " ";
	std::cout << std::endl;

	std::cout
	<< (int)d2[Index_U][0][0].rot_id << " "
	<< (int)d2[Index_U][0][2].rot_id << " "
	<< (int)d2[Index_U][2][0].rot_id << " "
	<< (int)d2[Index_U][2][2].rot_id << " "
	<< (int)d2[Index_D][0][0].rot_id << " "
	<< (int)d2[Index_D][0][2].rot_id << " "
	<< (int)d2[Index_D][2][0].rot_id << " "
	<< (int)d2[Index_D][2][2].rot_id << std::endl;

	std::cout
	<< (int)d1[Index_U][0][1].rot_id << " "
	<< (int)d1[Index_U][1][0].rot_id << " "
	<< (int)d1[Index_U][2][1].rot_id << " "
	<< (int)d1[Index_U][1][2].rot_id << " "
	<< (int)d1[Index_F][1][0].rot_id << " "
	<< (int)d1[Index_R][1][0].rot_id << " "
	<< (int)d1[Index_B][1][0].rot_id << " "
	<< (int)d1[Index_L][1][0].rot_id << " "
	<< (int)d1[Index_D][0][1].rot_id << " "
	<< (int)d1[Index_D][1][0].rot_id << " "
	<< (int)d1[Index_D][2][1].rot_id << " "
	<< (int)d1[Index_D][1][2].rot_id << std::endl;

	for (int i = 11; i >= 0; i--)
		std::cout << (n2 / pow(2, i)) % 2 << " ";
	std::cout << std::endl;

	std::cout
	<< (int)d2[Index_U][0][1].rot_id << " "
	<< (int)d2[Index_U][1][0].rot_id << " "
	<< (int)d2[Index_U][2][1].rot_id << " "
	<< (int)d2[Index_U][1][2].rot_id << " "
	<< (int)d2[Index_F][1][0].rot_id << " "
	<< (int)d2[Index_R][1][0].rot_id << " "
	<< (int)d2[Index_B][1][0].rot_id << " "
	<< (int)d2[Index_L][1][0].rot_id << " "
	<< (int)d2[Index_D][0][1].rot_id << " "
	<< (int)d2[Index_D][1][0].rot_id << " "
	<< (int)d2[Index_D][2][1].rot_id << " "
	<< (int)d2[Index_D][1][2].rot_id << std::endl;


	for (int s = Index_Start; s < Index_End; s++) {
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
