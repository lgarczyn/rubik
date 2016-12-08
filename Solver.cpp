//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#include <set>
#include "Solver.hpp"
#include "tools.hpp"


set&	Solver::get_opened_set(StateRef state) {

	if (state == nullptr)
		throw std::logic_error("Storing null to set");

	int index = State::get_index(*state);

	return _opened[index];
}

StateRef 	Solver::get_smallest_state() {

	auto set_it = _opened.begin();

	//Iterate over set of lists
	while (set_it != _opened.end()) {

		//Remove any possible empty lists
		while (set_it->second.empty()) {
			set_it = _opened.erase(set_it);
		}

		//Iterate over list until either empty, or value was found
		auto& list = set_it->second;

		while (list.empty() == false) {

			StateRef value = list.front();
			list.pop_front();

			//If value is found, remove and return
			if (value->is_alive())
				return value;
		}
	}
	throw std::logic_error("No opened state, scount is " + std::to_string(_openCount));
}

void		Solver::print_mem() {

	for (auto pair:_opened) {
		std::cout << pair.first << ": " << pair.second.size() << std::endl;
	}
	std::cout << std::flush;
}

Solver::Solver(State& initial, bool forget) : _opened(), _forget(forget) {

	StateRef root = StateRef(new State(initial));

	State::initial_score = root->get_weight();
	get_opened_set(root).push_front(root);

	if (!_forget)
		_universe.insert(root);
	std::cerr << "started" << std::endl;

	_openCount = 1;
	_sizeComplexity = 1;
	_timeComplexity = 1;
}

Solver::Result Solver::step() {
	Result result = Result(0, 0);
	std::vector<StateRef > candidates;

	if (_openCount <= 0)
		throw std::logic_error("No opened State");

	StateRef e = get_smallest_state();
	_openCount--;

	result.actual_state = e;

	if (e->is_final()) {
		result.finished = true;
		result.movements = e->get_movements();
		result.sizeComplexity = _sizeComplexity;
		result.timeComplexity = _timeComplexity;
		return result;
	}

	//get children
	e->get_candidates(candidates);

	//update size complexity
	if (State::stateCount > _sizeComplexity)
		_sizeComplexity = State::stateCount;

	//for every children
	for (StateRef s:candidates) {

		//if (astar && valid heuristic)
		if (State::get_index(*s) > 20 * score_multiplier)//TODO maybe 19 instead of 20?
			continue;

		if (!_forget) {
			auto position = _universe.find(s);

			if (position != _universe.end()) {

				const StateRef& previous = *position;
				if (State::get_index(*s) < State::get_index(*previous)) {
					previous->kill();
					_universe.erase(previous);
					_openCount--;
				} else {
					continue;
				}
			}
			_universe.insert(s);
		}
		get_opened_set(s).push_front(s);

		_openCount++;
		_timeComplexity++;
	}
	candidates.clear();

	result.sizeComplexity = _sizeComplexity;
	result.timeComplexity = _timeComplexity;
	return (result);
}

Solver::~Solver() {}

Solver::Result::Result(int timeComplexity, int sizeComplexity):
		timeComplexity(timeComplexity),
		sizeComplexity(sizeComplexity),
		actual_state(nullptr),
		finished(false) {
}
