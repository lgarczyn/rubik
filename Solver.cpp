/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:36:02 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:17 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Solver.hpp"
#include "Heuristics.hpp"
#include "tools.hpp"
#include <set>
#include <string.h>

Set &Solver::get_opened_set(const State &state) {
	int index = State::get_index(state);

	return _opened[index];
}

State Solver::get_smallest_state() {
	if (_openCount > _sizeComplexity) // LOCK ?
		_sizeComplexity = _openCount; //

	// Iterate over set of lists
	// while (set_it != _opened.end()) {

	auto set_it = _opened.begin();
	// while iterator list is empty and iterator has not reached end, remove
	// list
	while (set_it != _opened.end() && set_it->second.empty())
		set_it = _opened.erase(set_it);
	// if iterator has reached end, shout
	if (set_it == _opened.end())
		throw std::logic_error("No opened state, scount is " +
		                       std::to_string(_openCount));

	// Return and pop last element
	auto &list = set_it->second;
	auto value = list.back();
	list.pop_back();
	// std::cout << _openCount << std::endl;
	_openCount--;

	return value;
}

void Solver::print_mem() {
	for (auto pair : _opened) {
		std::cout << pair.first << ": " << pair.second.size() << std::endl;
	}
	std::cout << std::flush;
}

Solver::Solver() {}

Solver::Solver(State initial) { setup(initial); }

void Solver::setup(State initial) {
	_opened.clear();
	get_opened_set(initial).push_back(initial);

	_universe.clear();
	//_universe.set_empty_key(State(false));
	//_universe.set_deleted_key(State(true));
	_universe.insert(std::make_pair(initial, Movements()));

	_openCount = 1;
	_sizeComplexity = 1;
	_timeComplexity = 1;

	// std::cout << "setup" << std::endl;
}

Movements get_movement_clone(Movements &moves) {
	uint prev_distance = moves.size();
	Movements childmoves = Movements(prev_distance + 1);
	if (prev_distance)
		std::copy(moves.begin(), moves.end(), childmoves.begin());
	return childmoves;
}

Solver::Result Solver::step() {
	// std::cout << "wat\n";
	Result result = Result(_timeComplexity, _sizeComplexity);
	if (_openCount <= 0)
		throw std::logic_error("A No opened state, scount is " +
		                       std::to_string(_openCount));

	// pop best state
	State e = get_smallest_state(); // LOCK
	result.actual_state = e;        // ONLY ON MAIN THREAD

	// if final, stop step and signal full stop
	if (e.is_final()) { // HAVE ACTUAL STATE && FINISHED STATE
		// std::cout << "final" << std::endl;
		result.finished = true;
		result.movements = _universe[e];
		return result;
	}

	// get pos in universe, to get movement and check if state is still relevant
	auto it = _universe.find(e);
	// std::cout << e.get_id().corners << "\n";
	// if not in universe, shout loud
	if (it == _universe.end())
		throw std::logic_error("could not find e in universe");

	// clone movement for later use, remove from universe?
	//	if there are similar ids with insuficient scores, they will get ignored
	// but
	//	if the nodes are found again, they will be explored if no progress has
	// been made
	size_t distance = it->second.size();
	Movements childmoves = get_movement_clone(it->second);
	//_universe.erase(it);

	// if a better path to this node has been found, stop step
	if (distance < e.get_distance()) {
		// std::cout << distance << "<" << e.get_distance() << std::endl;
		return result;
	}
	if (distance > e.get_distance())
		throw std::logic_error(
		    "stored distance is greater than node distance: " +
		    std::to_string(distance) + ">" + std::to_string(e.get_distance()));

	// get children
	std::vector<State> candidates;
	e.get_candidates(candidates);
	// for every children
	for (State s : candidates) {
		// if (State::get_index == State::indexer_astar &&
		// Heuristics::HeuristicFunction == Heuristics::ValidFunction)
		if (State::get_index(s) > 20 * score_multiplier) // TODO maybe 19
			// instead of 20?
			// //TODO 11 instead
			// of 20 for corners?
			continue;

		auto position = _universe.find(s);

		if (position != _universe.end()) {
			const State &previous = position->first;
			if (State::get_index(s) <
			    State::get_index(previous)) { // replace with get_distance?
				_universe.erase(previous);
				// std::cout << "deleting: " << previous.get_id().corners <<
				// std::endl;
				_openCount--;
			} else {
				continue;
			}
		}
		// std::cout << "storing: " << s.get_id().corners << ":" <<
		// childmoves.size() << std::endl;

		childmoves[distance] = s.get_movement();
		_universe.insert(std::make_pair(s, childmoves));
		get_opened_set(s).push_back(s); // LOCK

		_openCount++;      // ATOMIC
		_timeComplexity++; // ATOMIC
	}
	candidates.clear();

	// std::cout << "R<\n";
	return (result);
}

Solver::~Solver() {}

Solver::Result::Result(int timeComplexity, int sizeComplexity)
    : timeComplexity(timeComplexity),
      sizeComplexity(sizeComplexity),
      actual_state(false),
      finished(false) {}

Solver::Result::Result()
    : timeComplexity(0),
      sizeComplexity(0),
      actual_state(false),
      finished(false) {}
