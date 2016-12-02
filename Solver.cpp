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

	while (_opened.begin()->second.size() == 0) {

		_opened.erase(_opened.begin());
	}


	return *(_opened.begin()->second.begin());
}

StateRef* Solver::get_universe_position(StateRef state) {
	Node* node = _universe.get();

	if (state == nullptr)
		throw std::logic_error("Storing null");

	//While no empty or equivalent node has been found
	while (1) {

		//If the curent node is empty, return ptr
		if (node->value == nullptr)
			return &(node->value);
		//Get the difference
		int d = State::compare(state->get_data(), node->value->get_data());
		//If equal, return ptr
		if (d == 0)
			return &(node->value);
		else if (d == 1) {
			if (node->right == nullptr)
				node->right = NodeRef(new Node());
			node = node->right.get();
		} else {
			if (node->left == nullptr)
				node->left = NodeRef(new Node());
			node = node->left.get();
		}
	}
}

Solver::Solver(State& initial, bool forget) : _opened(), _forget(forget) {

	StateRef root = StateRef(new State(initial));

	State::initial_score = root->get_weight();
	get_opened_set(root).insert(root);

	/*universe*/
	_universe = NodeRef(new Node());
	if (!_forget)
		*get_universe_position(root) = root;
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

	e->update();//SEGFAULT CHECK

	result.actual_state = e;

	if (e->is_final()) {
		result.finished = true;
		result.movements = e->get_movements();
		result.sizeComplexity = _sizeComplexity;
		result.timeComplexity = _timeComplexity;
		return result;
	}

	//remove e from open nodes
	get_opened_set(e).erase(e);
	_openCount--;

	//get children
	candidates.clear();
	e->get_candidates(candidates);

	//update size complexity
	if (State::stateCount > _sizeComplexity)
		_sizeComplexity = State::stateCount;

	//for every children
	for (StateRef s:candidates) {
		if (!_forget && false) {
			StateRef* position = get_universe_position(s);

			if (*position != nullptr) {
				StateRef previous = *position;
				if (State::get_index(*s) < State::get_index(*previous)) {
					get_opened_set(previous).erase(previous);
					_openCount--;
				} else {
					continue;
				}
			}

			*position = s;
		}

		get_opened_set(s).insert(s);
		_openCount++;
		_timeComplexity++;
	}

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
