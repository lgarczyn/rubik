//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#include <set>
#include "Solver.hpp"
#include "Heuristics.hpp"
#include "tools.hpp"
#include <string.h>


Set& Solver::get_opened_set(const State& state) {

	int index = State::get_index(state);

	return _opened[index];
}

State Solver::get_smallest_state() {
	auto set_it = _opened.begin();

	if (_openCount > _sizeComplexity)//LOCK ?
		_sizeComplexity = _openCount;//

	//Iterate over set of lists
	while (set_it != _opened.end()) {

		//Remove any possible empty lists
		while (set_it->second.empty()) {
			set_it = _opened.erase(set_it);
		}

		//Return and pop last element
		auto& list = set_it->second;
		auto value = list.back();
		list.pop_back();
		std::cout << _openCount << std::endl;
		_openCount--;

		return value;
	}
	throw std::logic_error("No opened state, scount is " + std::to_string(_openCount));
}

void		Solver::print_mem() {

	for (auto pair:_opened) {
		std::cout << pair.first << ": " << pair.second.size() << std::endl;
	}
	std::cout << std::flush;
}

Solver::Solver(){}

Solver::Solver(State initial) {
	setup(initial);
}

void Solver::setup(State initial) {
	_opened.clear();
	get_opened_set(initial).push_back(initial);

	_universe = Universe();
	_universe.set_empty_key(State(false));
	_universe.set_deleted_key(State(true));
	_universe.insert(std::make_pair(initial, Movements()));

	_openCount = 1;
	_sizeComplexity = 1;
	_timeComplexity = 1;
	
	std::cout << "setup" << std::endl;
}

Solver::Result Solver::step() {
	Result result = Result(_timeComplexity, _sizeComplexity);
	std::vector<State> candidates;


	if (_openCount <= 0)
		throw std::logic_error("No opened state, scount is " + std::to_string(_openCount));


	State e = get_smallest_state();//LOCK
	_openCount--;//LOCK

	result.actual_state = e;//ONLY ON MAIN THREAD

	if (e.is_final()) {//HAVE ACTUAL STATE && FINISHED STATE
		std::cout << "final" << std::endl;
		result.finished = true;
		result.movements = _universe[e];
		return result;
	}
	//TODO remove e from _universe, put in _grave
	//check to see if it's worth it to get from _grave too

	const Movements& moves = _universe.find(e)->second;
	uint prev_distance = moves.size();
	Movements childmoves = Movements(prev_distance + 1);
	//memmove(&childmoves[0], &moves[0], prev_distance * sizeof(uint16_t));
	if (prev_distance)
		std::copy(moves.begin(), moves.end(), childmoves.begin());

	if (moves.size() != e.get_distance())
		return result;

	//get children
	e.get_candidates(candidates);
	//for every children
	for (State s:candidates) {
		if (State::get_index == State::indexer_astar && Heuristics::HeuristicFunction == Heuristics::ValidFunction)
			if (State::get_index(s) > 20 * score_multiplier)//TODO maybe 19 instead of 20?
				continue;

		/*auto position = _universe.find(s);

		if (position != _universe.end()) {

			const State& previous = position->first;
			if (State::get_index(s) < State::get_index(previous)) {//replace with get_distance?
				_universe.erase(previous);
				_openCount--;
			} else {
				continue;
			}
		}*/
		childmoves[prev_distance] = s.get_movement();
		_universe.insert(std::make_pair(s, childmoves));
		get_opened_set(s).push_back(s);//LOCK

		_openCount++;//ATOMIC
		_timeComplexity++;//ATOMIC
	}
	candidates.clear();

	return (result);
}

Solver::~Solver() {}

Solver::Result::Result(int timeComplexity, int sizeComplexity):
		timeComplexity(timeComplexity),
		sizeComplexity(sizeComplexity),
		actual_state(false),
		finished(false) {
}

Solver::Result::Result():
		timeComplexity(0),
		sizeComplexity(0),
		actual_state(false),
		finished(false) {
}
