#include "Tests.hpp"
#include "State.hpp"

void Tests::encoding_tests() {
	ID id;

	id = ID();
	if (id != Encoding::id_from_data(Encoding::data_from_id(id)))
		throw std::logic_error("id conversion non reciprocical");
	State s = State();
	s = s.get_child(Move::Right);
	s = s.get_child(Move::Down);

	ID id_2 = Encoding::id_from_data(Encoding::data_from_id(s.get_id()));

	if (s.get_id() != id_2)
		throw std::logic_error("id conversion non reciprocical");

	s = s.get_child(Move(Move::Down, 3));
	s = s.get_child(Move(Move::Right, 3));

	if (id != Encoding::id_from_data(Encoding::data_from_id(s.get_id())))
		throw std::logic_error("id conversion non reciprocical");

	if (Encoding::cube_from_id(id) != State::solution_cube)
		throw std::logic_error("cube conversion non reciprocical");
}