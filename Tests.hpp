#pragma once
#include "Display.hpp"
#include "Encoding.hpp"
#include "Heuristics.hpp"
#include "State.hpp"
#include <random>

namespace Tests {
	template <class ID>
	void encoding_tests() {
		ID id, id_cmp;

		id = ID();
		id_cmp = Encoding::id_from_data<ID>(Encoding::data_from_id(id));
		if (id != id_cmp)
			throw std::logic_error("id conversion non reciprocical");
		State<ID> s = State<ID>();
		s = s.get_child(Move::Up);
		s = s.get_child(Move(Move::Right, 2));

		ID id_2 = Encoding::id_from_data<ID>(Encoding::data_from_id(s.get_id()));

		if (s.get_id() != id_2)
			throw std::logic_error("id conversion non reciprocical");

		s = s.get_child(Move(Move::Right, 2));
		s = s.get_child(Move(Move::Up, 3));

		id_cmp = Encoding::id_from_data<ID>(Encoding::data_from_id(s.get_id()));
		if (id != id_cmp)
			throw std::logic_error("id conversion non reciprocical");

		if (Encoding::cube_from_id(id) != Encoding::solution_cube)
			throw std::logic_error("cube conversion non reciprocical");
	}

	void heuristic_tests() {
		ID id;
		int score_cmp;

		id = ID();
		score_cmp = Heuristics::ValidFunction(Encoding::data_from_id(id));
		if (score_cmp != 0)
			throw std::logic_error("solution score is not 0");

		State<> s = State<>();
		s = s.get_child(Move::Right);

		score_cmp = Heuristics::ValidFunction(Encoding::data_from_id(s.get_id()));
		if (score_cmp != score_multiplier)
			throw std::logic_error("single move should result in a score of score_multiplier");

		s = s.get_child(Move::Right);

		score_cmp = Heuristics::ValidFunction(Encoding::data_from_id(s.get_id()));
		if (score_cmp != score_multiplier)
			throw std::logic_error("single half-turn should result in a score of score_multiplier");

		s = s.get_child(Move::Left);

		score_cmp = Heuristics::ValidFunction(Encoding::data_from_id(s.get_id()));
		if (score_cmp != 2 * score_multiplier)
			throw std::logic_error("atomic turns should affect scores idependantly");
	}

	static vector<Move> get_random_scramble(int scramble_count) {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(Move::Direction_Start, Move::Direction_End);
		std::uniform_int_distribution<int> turns(1, 3);

		vector<Move> moves;

		int previous = Move::None;
		for (int i = 0; i < scramble_count; i++) {
			int m = uni(rng);
			while (m == previous || Move::is_commutative(previous, m))
				m = uni(rng);
			previous = m;
			int t = turns(rng);
			Move move = Move((Move::Direction)m, t);

			moves.push_back(move);
		}
		return moves;
	}

	static vector<Move> get_random_scramble_g2(int scramble_count) {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(Move::Direction_Start, Move::Direction_End);
		std::uniform_int_distribution<int> turns(1, 3);

		vector<Move> moves;

		int previous = Move::None;
		for (int i = 0; i < scramble_count; i++) {
			int m = uni(rng);
			while (m == previous || Move::is_commutative(previous, m))
				m = uni(rng);
			previous = m;
			int t = turns(rng);

			while (t != 2 && m != Move::Up && m != Move::Down)
				t = turns(rng);

			Move move = Move((Move::Direction)m, t);

			moves.push_back(move);
		}
		return moves;
	}

	void commutativity_tests() {
		for (int i = 0; i < 10000; i++) {
			State<ID> s = State<ID>(ID(), Move::None);
			s = s.get_scrambled(get_random_scramble(100));

			State<ID> c = s.get_child(get_random_scramble(1).front());

			State<ID> s2 = c.get_parent();

			if (s.get_id() != s2.get_id())
				throw std::logic_error("wtf1");
		}
		for (int i = 0; i < 10000; i++) {
			State<IDG1> s = State<IDG1>(IDG1(), Move::None);
			s = s.get_scrambled(get_random_scramble(100));

			State<IDG1> c = s.get_child(get_random_scramble(1).front());

			State<IDG1> s2 = c.get_parent();

			if (s.get_id() != s2.get_id())
				throw std::logic_error("wtf1");
		}
		for (int i = 0; i < 10000; i++) {
			State<IDG2> s = State<IDG2>(IDG2(), Move::None);
			s = s.get_scrambled(get_random_scramble_g2(100));

			State<IDG2> c = s.get_child(get_random_scramble_g2(1).front());

			State<IDG2> s2 = c.get_parent();

			if (s.get_id() != s2.get_id())
				throw std::logic_error("wtf1");
		}
	}

	void rotation_tests() {
		State<ID> a;
		State<IDG1> b;
		State<IDG2> c;
		char last_c = '\0';

		while (1) {
			char c;
			std::cin >> c;

			State<ID> a_old = a;
			State<IDG1> b_old = b;
			State<IDG2> c_old = c;

			switch (c) {
				case 'U': a = a.get_child(Move(std::make_pair(c, last_c))); break;
				case 'D': a = a.get_child(Move(std::make_pair(c, last_c))); break;
				case 'L': a = a.get_child(Move(std::make_pair(c, last_c))); break;
				case 'R': a = a.get_child(Move(std::make_pair(c, last_c))); break;
				case 'F': a = a.get_child(Move(std::make_pair(c, last_c))); break;
				case 'B': a = a.get_child(Move(std::make_pair(c, last_c))); break;
			}
			last_c = c;

			clear_screen();
			print_diff(a_old, a.get_movement());
			usleep(200000);

			clear_screen();
			print_diff(a, a.get_movement());
			usleep(200000);
			
			print_map(a);
		}

	}
} // namespace Tests