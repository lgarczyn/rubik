#pragma once
#include "Display.hpp"
#include "Encoding.hpp"
#include "Heuristics.hpp"
#include "State.hpp"

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
		score_cmp = Heuristics::ValidFunction(State<>::data_from_id(id));
		if (score_cmp != 0)
			throw std::logic_error("solution score is not 0");

		State<> s = State<>();
		s = s.get_child(Move::Right);

		score_cmp = Heuristics::ValidFunction(State<>::data_from_id(s.get_id()));
		if (score_cmp != score_multiplier)
			throw std::logic_error("single move should result in a score of score_multiplier");

		s = s.get_child(Move::Right);

		score_cmp = Heuristics::ValidFunction(State<>::data_from_id(s.get_id()));
		if (score_cmp != score_multiplier)
			throw std::logic_error("single half-turn should result in a score of score_multiplier");

		s = s.get_child(Move::Left);

		score_cmp = Heuristics::ValidFunction(State<>::data_from_id(s.get_id()));
		if (score_cmp != 2 * score_multiplier)
			throw std::logic_error("atomic turns should affect scores idependantly");
	}
} // namespace Tests