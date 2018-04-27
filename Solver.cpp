/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:36:02 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/20 21:06:11 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Solver.hpp"
#include "Heuristics.hpp"
#include "tools.hpp"
#include <set>
#include <string.h>
#include <unordered_set>

Set &Solver::get_opened_set(const State &state) {
	int index = state.get_index();

	return _opened[index];
}

State Solver::get_smallest_state() {
	if (_openCount > _sizeComplexity)
		_sizeComplexity = _openCount;

	auto set_it = _opened.begin();
	// while iterator list is empty and iterator has not reached end, remove
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

int Solver::get_real_open_count() {
	int count = 0;
	std::cerr << "debug function called" << std::endl;
	for (auto vec : _opened)
		count += vec.second.size();
	return count;
}

void Solver::print_mem() {
	std::cout << "universe: " << _universe.size() << std::endl;
	for (auto pair : _opened) {
		std::cout << pair.first << ": " << pair.second.size() << std::endl;
	}
	std::cout << std::flush;
}

Solver::Solver() {}

Solver::Solver(State initial, bool forget) { setup(initial, forget); }

void Solver::setup(State initial, bool forget) {
	_opened.clear();
	get_opened_set(initial).push_back(initial);

	_universe.clear();
#ifdef DENSE_HASH
	_universe.set_empty_key(State(false));
#endif
#if defined(DENSE_HASH) || defined(SPARSE_HASH)
	_universe.set_deleted_key(State(true));
#endif
	//_universe.insert(initial, Movements()));

	_forget = forget;
	_openCount = 1;
	_sizeComplexity = 1;
	_timeComplexity = 1;
}

Movements get_movement_clone(Movements &moves) {
	uint prev_distance = moves.size();
	Movements childmoves = {};
	if (prev_distance)
		std::copy(moves.begin(), moves.end(), childmoves.begin());
	return childmoves;
}

/*
DO NOT USE
int Solver::clean_duplicates() {
	std::cerr << "debug function called" << std::endl;
	static Universe universe;

	universe.clear();
	for (auto vec : _opened)
		for (auto st : vec.second)
			universe[st] = Movements();

	int prevCount = _openCount;
	_openCount = universe.size();

	_opened.clear();
	for (auto k : universe)
		get_opened_set(k.first).push_back(k.first);

	return prevCount - _openCount;
}*/
#include <vector>
static std::vector<uchar> get_path(Universe &u, State e) {
	std::vector<uchar> moves;

	moves.reserve(e.get_distance());
	while (e.get_movement() != 0) {
		moves.push_back((uchar)e.get_movement());
		e = e.get_parent();
		e = *u.find(e);
	}
	std::reverse(moves.begin(), moves.end());
	return moves;
}

Solver::Result Solver::step() {
	Result result = Result(_timeComplexity, _sizeComplexity);
	if (_openCount <= 0)
		throw std::logic_error("A No opened state, scount is " +
		                       std::to_string(_openCount));
	// pop best state
	State e = get_smallest_state();
	result.actual_state = e;

	// if final, stop step and signal full stop
	if (e.is_final()) {
		result.finished = true;
		result.movements = get_path(_universe, e);
		return result;
	}

	// check if node was already expanded
	auto pair = _universe.insert(e);
	if (pair.second == false) {
		return result;
	}

	// get children
	std::vector<State> candidates;
	e.get_candidates(candidates);
	// for every children
	for (State s : candidates) {
		get_opened_set(s).push_back(s);

		_openCount++;
		_timeComplexity++;
	}
	candidates.clear();
	return result;
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
