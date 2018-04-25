#include <iostream>

#include "Database.hpp"
#include "OptParser.hpp"
#include "Parser.hpp"
#include "Solver.hpp"
#include "Types.hpp"
#include "tools.hpp"
#include <random>
#include <termcap.h>
#include <unistd.h>
#include <unistd.h>

int main_create_databases() {

	Databases::corners = Database(Encoding::corners_max);
	Database &d = Databases::corners;

	std::cerr << d.size() << " " << Databases::upper_corners.size() << " "
	          << Databases::lower_corners.size() << "\n";

	State s = State();
	ID id = ID();
	// Solver solver;
	Solver solver = Solver();
	for (uint i = 0; i < Encoding::corners_max; i++) {
		if (d.exist(i) == false) {
			std::cout << "start " << i << "\n";
			id.corners = i;
			s = State(id);
			if (s.is_solvable() == false) {
				std::cout << "end: unsolvable\n";
				continue;
			}
			solver.setup(s, 1);
			Solver::Result res;
			while (1) {
				res = solver.step();
				if (res.finished)
					break;
			}
			s = State();
			int len = 0;
			for (int m : res.movements) {
				s = State(s, (State::Movement)m);
				d.store(s.get_id().corners, len,
				    (uint)len == res.movements.size() - 1);
				len++;
			}
			std::cout << "end: " << len << "\ntotal saved : " << Databases::total_saved
			          << "\ntotal overridden : " << Databases::total_overridden
			          << "\ntotal saved first : " << Databases::total_saved_first
			          << "\ntotal overridden first : " << Databases::total_overridden_first
			          << std::endl;
			for (int m : res.movements)
				std::cout << (State::Movement)m << " ";
			std::cout << std::endl;
			// std::cout <<  << len << "\n";
		} else
			std::cout << "end: already in\n";
		// if (i % 100 == 0) {
		//	s._get_id().corners = i;
		//	s.update_weight();
		//	print_map(s);
		//	std::cout << i << " " << (int)d[i] << std::endl;
		//}
	}
	// f.close();
	std::ofstream of = std::ofstream("corners.db");
	of << d;
	return 0;
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

int maintry() {
	State s;

	print_map(Encoding::cube_from_id(ID()));
	while (1) {
		string line;

		std::cin >> line;

		s = State(s, line);

		print_map(s);

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
				std::cout << "  {" << (int)as.cube_id << "=" <<
(int)bs.cube_id << " " << (int)as.face_id << "=" << (int)bs.face_id << "}  ";
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
		return 1;

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
	std::random_device rd;	 // only used once to initialise (seed) engine
	std::mt19937 rng(rd());	// random-number engine used
(Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0,20); // guaranteed unbiased

	for (int i = 0; i < 100000; i++) {

		if (i % 1000 == 0)
			std::cout << i << std::endl;
		int s_count = uni(rng);

		State *s = get_random_state(s_count);
		int r = Heuristics::HeuristicFunction(s->get_data(),
s->get_finder());

		if (r > s_count)
			std::cout << "Error, heuristic result is bigger than
solution" << std::endl;

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