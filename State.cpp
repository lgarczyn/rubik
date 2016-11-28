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
#include <sstream>

int					State::stateCount = 0;
score				State::initial_score = 0;
indexer				State::get_index = indexer_astar;

State::State(const std::string& scramble = ""){
	_data[Index_Up] = {White};
	_data[Index_Right] = {Red};
	_data[Index_Down] = {Yellow};
	_data[Index_Left] = {Orange};
	_data[Index_Front] = {Green};
	_data[Index_Back] = {Blue};

	applyScramble(scramble);

	_weight = Heuristics::HeuristicFunction(_data);
	_distance = 0;
	_movement = None;
	_parent = nullptr;
	stateCount++;
}

void	State::get_candidates(State** candidates, State* from)
{
	//TODO remove previous move reversed
	candidates[1] = new State(from, (Movement)(Up | Reversed));
	candidates[2] = new State(from, (Movement)(Up | Halfturn));
	candidates[3] = new State(from, Right);
	candidates[4] = new State(from, (Movement)(Right | Reversed));
	candidates[5] = new State(from, (Movement)(Right | Halfturn));
	candidates[6] = new State(from, Down);
	candidates[7] = new State(from, (Movement)(Down | Reversed));
	candidates[8] = new State(from, (Movement)(Down | Halfturn));
	candidates[9] = new State(from, Left);
	candidates[10] = new State(from, (Movement)(Left | Reversed));
	candidates[11] = new State(from, (Movement)(Left | Halfturn));
	candidates[12] = new State(from, Front);
	candidates[13] = new State(from, (Movement)(Front | Reversed));
	candidates[14] = new State(from, (Movement)(Front | Halfturn));
	candidates[15] = new State(from, Back);
	candidates[16] = new State(from, (Movement)(Back | Reversed));
	candidates[17] = new State(from, (Movement)(Back | Halfturn));
}

void State::applyScramble(string& scramble) {

	std::stringstream ss = stringstream(scramble);

	while (ss) {
		Movement m;
		int c = ss.get();

		switch (c) {
			case 'F': m = Front; break;
			case 'R': m = Front; break;
			case 'U': m = Front; break;
			case 'B': m = Front; break;
			case 'L': m = Front; break;
			case 'D': m = Front; break;
			default: continue;
		}
		c = ss.peek();
		switch (c) {
			case '\'': m = (Movement)(m | Reversed); break;
			case '2': m = (Movement)(m | Halfturn); break;
		}
		applyMovement(m);
	}
}

void State::applyMovement(State::Movement m) {
	//TODO apply movement
}

State::State(State* parent, State::Movement m) {
	_data = parent->_data;
	_parent = parent;
	_distance = parent->_distance + 1;
	_movement = m;

	applyMovement(m);

	_weight = Heuristics::HeuristicFunction(_data);
	stateCount++;
}

State::~State()
{
	stateCount--;
}


std::vector<State::Movement>* State::get_movements() const {
	std::vector<State::Movement>* movements = new std::vector<State::Movement>(_distance);

	const State* node = this;
	int counter = _distance;

	while (--counter >= 0 && node)
	{
		(*movements)[counter] = node->_movement;
		node = node->_parent;
	}
	return (movements);
}

int State::get_distance() const
{
	return (_distance);
}

bool State::is_final() const {
	return _weight == 0;
}

Data&	State::get_data(void)
{
	return (this->_data);
}

const Data&	State::get_data(void) const
{
	return (this->_data);
}

std::ostream& operator<< (std::ostream& s, const State::Movement c)
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
		default return c << " ERROR ";
	}
}

void			State::set_parent(State* p)
{
	this->_parent = p;
}

State*			State::get_parent(void) const
{
	return (this->_parent);
}

void 			State::set_distance(int d)
{
	this->_distance = d;
}

score 			State::get_weight(void) const
{
	return (this->_weight);
}

void 			State::set_weight(score s)
{
	this->_weight = s;
}

State::Movement		State::get_movement() const {
	return (this->_movement);
}

void			State::init()
{
	State::solution = State();
}

size_t custom_hash::operator()(const State* x) const noexcept
{
	std::hash<Data> hash;
	return (hash(x->get_data()));
}

bool custom_equal_to::operator()(const State* a, const State* b) const noexcept
{
	return (a->get_data() == b->get_data());
}

score State::indexer_astar(const State* state)
{
	return state->get_weight() + state->get_distance();
}

score State::indexer_greedy(const State* state)
{
	return state->get_weight();
}

score State::indexer_uniform(const State* state)
{
	return state->get_distance();
}
