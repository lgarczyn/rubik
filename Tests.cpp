#include "Tests.hpp"
#include "State.hpp"
#include "Heuristics.hpp"

void Tests::encoding_tests() {
	ID id;

	id = ID();
	if (id != State::id_from_data(State::data_from_id(id)))
		throw std::logic_error("id conversion non reciprocical");
	State s = State();
	s = s.get_child(Move::Right);
	s = s.get_child(Move::Down);

	ID id_2 = State::id_from_data(State::data_from_id(s.get_id()));

	if (s.get_id() != id_2)
		throw std::logic_error("id conversion non reciprocical");

	s = s.get_child(Move(Move::Down, 3));
	s = s.get_child(Move(Move::Right, 3));

	if (id != State::id_from_data(State::data_from_id(s.get_id())))
		throw std::logic_error("id conversion non reciprocical");

	if (State::cube_from_id(id) != State::solution_cube)
		throw std::logic_error("cube conversion non reciprocical");
}

void Tests::heuristic_tests() {
	ID id;

	id = ID();
	if (Heuristics::ValidFunction(State::data_from_id(id)) != 0)
		throw std::logic_error("solution score is not 0");
	
	State s = State();
	s = s.get_child(Move::Right);

	if (Heuristics::ValidFunction(State::data_from_id(s.get_id())) != score_multiplier)
		throw std::logic_error("single move should result in a score of score_multiplier");
	
	s = s.get_child(Move::Right);

	if (Heuristics::ValidFunction(State::data_from_id(s.get_id())) != score_multiplier)
		throw std::logic_error("single half-turn should result in a score of score_multiplier");


	s = s.get_child(Move::Left);

	if (Heuristics::ValidFunction(State::data_from_id(s.get_id())) != 2* score_multiplier)
		throw std::logic_error("atomic turns should affect scores idependantly");


}