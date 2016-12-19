/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 14:18:52 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/18 19:04:49 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "State.hpp"
#include "Heuristics.hpp"
#include <random>
#include <limits>

indexer				State::get_index = indexer_astar;
const Cube			State::solution = _calculate_solution();
const Finder		State::solution_finder = _calculate_finder(solution);
const Color			State::solution_colors[] = {White, Green, Red, Blue, Orange, Yellow};

State::State():
	_id(),
	_movement(None),
	_weight(0),
	_distance(0) {}

State::State(bool is_del):
	_id({
		is_del ? UINT_MAX : UINT_MAX - 1,
		is_del ? UINT_MAX : UINT_MAX - 1,
		is_del ? UINT_MAX : UINT_MAX - 1}),
	_movement(None),
	_weight(UCHAR_MAX),
	_distance(UCHAR_MAX) {}


void State::update_weight() {
	_apply_cube(cube_from_id(_id));
}
#include <iomanip>

void State::_apply_cube(const Cube& cube) {
	_id = id_from_cube(cube);
	Data d = data_from_id(_id);
	ID id = id_from_data(d);
	if (id != _id) {
		std::cerr << "diff"<< std::endl;
		std::cerr << (id.corners / (uint)pow(3, 8)) << " " << std::setbase(3 )<< (id.corners % (uint)pow(3, 8)) << " " << std::setbase(2) << id.borders_rot << " " << id.borders_pos << std::endl;
		std::cerr << (_id.corners / (uint)pow(3, 8)) << " " << std::setbase(3 )<< (_id.corners % (uint)pow(3, 8)) << " " << std::setbase(2) << _id.borders_rot << " " << _id.borders_pos << std::endl;
	}
	//else
	//	std::cerr << "same" << std::endl;
	_weight = Heuristics::HeuristicFunction(cube);
}

State::State(const std::string& scramble):State(){
	Cube cube = solution;
	_apply_scramble(cube, scramble);
	_apply_cube(cube);
}

State::State(int scramble_count):State(){
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(Movement_Start, Movement_End-1);
	std::uniform_int_distribution<int> bo(0,2);

	Cube cube = solution;
	Movement previous = None;
	for (int i = 0; i < scramble_count; i++) {
		Movement m = (Movement)uni(rng);
		while (m == previous)
			m = (Movement)uni(rng);
		int c = bo(rng);
		if (c == 1)
			m = (Movement)(m | Reversed);
		else if (c == 2)
			m = (Movement)(m | Halfturn);

		_apply_movement(cube, m);
	}
	_apply_cube(cube);
}

State::State(const State& parent, State::Movement m, const Cube& cube) {
	_movement = m;
	_distance = parent._distance + 1;
	_apply_cube(cube);
}

State::State(const State& parent, State::Movement m) {
	if (m == None)
		throw std::logic_error("None is not an allowed move, use copy constructor");

	_movement = m;
	_distance = parent._distance + 1;

	Cube cube = cube_from_id(parent._id);
	_apply_movement(cube, m);
	_apply_cube(cube);
}

State::State(const State& clone) {
	*this = clone;
}

State& State::operator=(const State& ra) {
	_id = ra._id;
	_weight = ra._weight;
	_movement = ra._movement;
	_distance = ra._distance;
	return *this;
}

void	State::get_candidates(std::vector<State>& candidates) const
{
	//get cube of current state
	Cube cube = cube_from_id(_id);
	//get movement of current state
	Movement m = (Movement)(_movement & Mask);

	//foreach possible movement family
	for (int n = Movement_Start; n < Movement_End; n++) {
		//if the movement is in same family as current, skip
		if (m == n)
			continue;
		//rotate 90d, build, then repeat
		_apply_movement(cube, (Movement)n);
		candidates.push_back(State(*this, (Movement)n, cube));
		_apply_movement(cube, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Halfturn), cube));
		_apply_movement(cube, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Reversed), cube));
		//reset cube, can be changed for stored cube copy, or another cube_from_id
		_apply_movement(cube, (Movement)n);
	}
}

bool State::is_final() const {
	return _weight == 0;
}

const ID&	State::get_id(void) const
{
	return this->_id;
}

uint State::get_weight(void) const
{
	return this->_weight;
}

State::Movement State::get_movement(void) const
{
	return (Movement)_movement;
}

uint State::get_distance() const
{
	return _distance;
}

ID&	State::_get_id(void){
	return this->_id;
}

std::ostream& operator<<(std::ostream& s, const State::Movement c)
{
	bool reversed = c & State::Reversed;
	bool halfturn = c & State::Halfturn;

	switch (c & State::Mask) {
		case State::Left:  return s << (reversed ? "L'" : (halfturn ? "L2" : "L"));
		case State::Right: return s << (reversed ? "R'" : (halfturn ? "R2" : "R"));
		case State::Up:    return s << (reversed ? "U'" : (halfturn ? "U2" : "U"));
		case State::Down:  return s << (reversed ? "D'" : (halfturn ? "D2" : "D"));
		case State::Front: return s << (reversed ? "F'" : (halfturn ? "F2" : "F"));
		case State::Back:  return s << (reversed ? "B'" : (halfturn ? "B2" : "B"));
		default: return s << " ERROR:" << (int)c << ' ';
	}
}

bool State::operator==(const State& ra) const
{
	return (_id.corners == ra._id.corners);
}

size_t custom_hash::operator()(const State& l) const noexcept {
	const ID& id = l.get_id();

	return (id.borders_rot ^ id.borders_pos) | ((size_t)id.corners << 32);
}

Score State::indexer_astar(const State& state)
{
	return state.get_weight() + state.get_distance() * score_multiplier;
}

Score State::indexer_greedy(const State& state)
{
	return state.get_weight();
}

Score State::indexer_uniform(const State& state)
{
	return state.get_distance();
}

#include "State_Encoding.cpp"
