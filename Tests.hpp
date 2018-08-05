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

	template <class ID>
	Data rotation_tests(Data init) {
		State<ID> a = State<ID>(Encoding::id_from_data<ID>(init), Move());
		char last_input = '\0';

		while (1) {
			Display::clear_screen();
			Display::print_map(a);
			Display::print_id(a.get_id());
			Display::print_data(a.get_data());
			char input;
			std::cin >> input;

			State<ID> a_old = a;

			switch (input) {
			case 'U':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'D':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'L':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'R':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'F':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'B':
				a = a.get_child(Move(std::make_pair(input, last_input)));
				break;
			case 'Q':
				return (a.get_data());
			default:
				last_input = input;
				continue;
			}
			last_input = input;

			Display::clear_screen();
			Display::print_diff(a_old.get_cube(), a.get_movement());
			Display::print_id(a_old.get_id());
			Display::print_data(a_old.get_data());
			usleep(1000000);

			Display::clear_screen();
			Display::print_diff(a.get_cube(), a.get_movement());
			Display::print_id(a.get_id());
			Display::print_data(a.get_data());
			usleep(1000000);
		}
	}
} // namespace Tests