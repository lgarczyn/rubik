/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   State.cpp										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: edelangh <edelangh@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2016/03/04 14:18:52 by edelangh		  #+#	#+#			 */
/*   Updated: 2016/03/18 19:04:49 by edelangh		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "State.hpp"
#include "Heuristics.hpp"
#include <random>
#include <limits>
#include <sstream>
#include <iomanip>
#include <algorithm>

indexer				State::get_index = indexer_astar;
const Data			State::solution_data = data_from_id(ID());
const Cube			State::solution_cube = cube_from_id(ID());
const Finder		State::solution_finder = _calculate_finder(solution_cube);
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


State::State(const ID& id):State() {
	_apply_data(data_from_id(id));
}

void State::_apply_data(const Data& data) {
	_id = id_from_data(data);
	_weight = Heuristics::HeuristicFunction(data);
	_weight = std::max((int)_weight, Heuristics::DatabaseFunction(_id));
}

void State::_apply_scramble(Data& data, const string& scramble) {

	std::stringstream ss = std::stringstream(scramble);

	while (ss) {
		Movement m;
		int turns = 1;
		int c = ss.get();

		switch (c) {
			case 'F': m = Front; break;
			case 'R': m = Right; break;
			case 'U': m = Up; break;
			case 'B': m = Back; break;
			case 'L': m = Left; break;
			case 'D': m = Down; break;
			default: continue;
		}
		c = ss.peek();
		switch (c) {
			case '\'': turns = 3; break;
			case '2': turns  = 2; break;
		}
		_apply_movement(data, m, turns);
	}
}

void State::_apply_movement(Data& data, Movement m) { Encoding::_apply_movement(data, m);}
void State::_apply_movement(Data& data, Movement m, int turns) { Encoding::_apply_movement(data, m, turns);}

State::State(const std::string& scramble):State(){
	Data data = solution_data;
	_apply_scramble(data, scramble);
	_apply_data(data);
}

State::State(const State& parent, const std::string& scramble):State() {
	Data data = data_from_id(parent._id);
	_apply_scramble(data, scramble);
	_apply_data(data);
}

State::State(int scramble_count):State(){
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(Movement_Start, Movement_End-1);
	std::uniform_int_distribution<int> turns(1,3);

	Data data = solution_data;
	Movement previous = None;
	for (int i = 0; i < scramble_count; i++) {
		Movement m = (Movement)uni(rng);
		while (m == previous)
			m = (Movement)uni(rng);

		_apply_movement(data, m, turns(rng));
	}
	_apply_data(data);
}

State::State(const State& parent, State::Movement m, const Data& data):State() {
	_movement = m;
	_distance = parent._distance + 1;
	_apply_data(data);
}

State::State(const State& parent, State::Movement m):State() {
	if ((m & Mask) == None)
		throw std::logic_error("None is not an allowed move, use copy constructor");

	_movement = m;
	_distance = parent._distance + 1;

	Data data = data_from_id(parent._id);
	_apply_movement(data, m, _get_turns(m));
	_apply_data(data);
}

int State::_get_turns(Movement m) {
	switch (m & ~Mask) {
		case Reversed : return 3;
		case Halfturn : return 2;
		default: return 1;
	}
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

constexpr bool is_move_duplicate(State::Movement a, State::Movement b) {
	if (a == State::Down && b == State::Up)
		return true;
	if (a == State::Back && b == State::Front)
		return true;
	if (a == State::Left && b == State::Right)
		return true;
	return false;
}

void	State::get_candidates(std::vector<State>& candidates) const
{
	//get cube of current state
	Data data = data_from_id(_id);
	//get movement of current state
	Movement m = (Movement)(_movement & Mask);

	//foreach possible movement family
	for (int n = Movement_Start; n < Movement_End; n++) {
		//if the movement is in same family as current, skip
		if (m == n)
			continue;
		//if the previous movement was opposite to the current, and had priority, ignore this one
		if (is_move_duplicate(m, (Movement)n))
			continue;
		//rotate 90d, build, then repeat
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)n, data));
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Halfturn), data));
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Reversed), data));
		//reset data, can be changed for stored data copy, or another data_from_id
		_apply_movement(data, (Movement)n);
	}
}

bool State::is_final() const {
	return _weight == 0;
}

const ID&	State::get_id(void) const
{
	return _id;
}

Data State::get_data() const {
	return data_from_id(_id);
}
Cube State::get_cube() const {
	return cube_from_id(_id);
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

std::ostream& operator<<(std::ostream& s, const State::Movement c)
{
	bool reversed = c & State::Reversed;
	bool halfturn = c & State::Halfturn;

	switch (c & State::Mask) {
		case State::Left:  return s << (reversed ? "L'" : (halfturn ? "L2" : "L"));
		case State::Right: return s << (reversed ? "R'" : (halfturn ? "R2" : "R"));
		case State::Up:	return s << (reversed ? "U'" : (halfturn ? "U2" : "U"));
		case State::Down:  return s << (reversed ? "D'" : (halfturn ? "D2" : "D"));
		case State::Front: return s << (reversed ? "F'" : (halfturn ? "F2" : "F"));
		case State::Back:  return s << (reversed ? "B'" : (halfturn ? "B2" : "B"));
		default: return s << " ERROR:" << (int)c << ' ';
	}
}

bool State::operator==(const State& ra) const
{
	return (Encoding::floor_index_upper_corners(_id.corners) == Encoding::floor_index_upper_corners(ra._id.corners));//TODO change ==
}

size_t custom_hash::operator()(const State& l) const noexcept {
	const ID& id = l.get_id();

	return Encoding::floor_index_upper_corners(id.corners);
	//return id.corners;//TODO put bak normal hash
	//return (id.borders_rot ^ id.borders_pos) | ((size_t)id.corners << 32);
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
