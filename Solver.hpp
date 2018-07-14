/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:39:07 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/25 03:24:25 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Result.hpp"
#include "State.hpp"

#include <list>
#include <map>
#include <set>
#include <string>

//#define DENSE_MAP
//#define SPARSE_MAP
//#define BTREE_MAP

#ifdef DENSE_MAP
#include <sparsehash/dense_hash_set>
#elif defined SPARSE_MAP
#include <sparsehash/sparse_hash_set>
#elif defined BTREE_MAP
#include "lib/cpp-btree/btree_set.h"
#else
#include <unordered_set>
#endif

template <class ID = ID, class State = State<ID>>
class Solver {
	//using set = std::unordered_set<State, custom_hash<ID>>;
	using Set = vector<StateDistance<ID>>;
	using Map = std::map<int, Set>;

#ifdef DENSE_MAP
	using Universe = google::dense_hash_set<State, custom_hash<ID>>;
#elif defined SPARSE_MAP
	using Universe = google::sparse_hash_set<State, custom_hash<ID>>;
#elif defined BTREE_MAP
	using Universe = btree::btree_set<State, custom_cmp>;
#else
	using Universe = std::unordered_set<State, custom_hash<ID>, custom_pred<ID>>;
#endif

  private:
	Map _opened;
	int _openCount;
	Universe _universe;
	int _timeComplexity;
	int _sizeComplexity;

  public:
	void store_state(const State &state, Distance dist, Score score) {
		int index = dist * score_multiplier + score;

		_opened[index].push_back(StateDistance<ID>(state, dist));
	}

	StateDistance<ID> get_smallest_state(bool *is_final) {

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
		_openCount--;

		//If the returned state has a value == to distance, meaning its score is 0
		//Which in turns means that it is final
		if (value.distance * score_multiplier == set_it->first) {
			*is_final = true;
		}

		return value;
	}

	int get_real_open_count() {
		int count = 0;
		std::cerr << "debug function called" << std::endl;
		for (auto vec : _opened)
			count += vec.second.size();
		return count;
	}

	void print_mem() {
		std::cout << "universe: " << _universe.size() << std::endl;
		for (auto pair : _opened) {
			std::cout << pair.first << ": " << pair.second.size() << std::endl;
		}
		std::cout << std::flush;
	}

	Solver() {}

	Solver(State initial) { setup(initial); }

	void setup(State initial) {
		_opened.clear();
		store_state(initial, 0, initial.calculate_score());

		_universe.clear();
#ifdef DENSE_HASH
		_universe.set_empty_key(State::get_empty_key());
#endif
#if defined(DENSE_HASH) || defined(SPARSE_HASH)
		_universe.set_deleted_key(State::get_deleted_key());
#endif

		_openCount = 1;
		_sizeComplexity = 1;
		_timeComplexity = 1;
	}

	vector<Move> get_path(State e, Distance d) {
		vector<Move> moves;

		moves.reserve(d);
		while (e.get_movement().direction) {
			moves.push_back(e.get_movement());
			e = e.get_parent();
			e = *_universe.find(e);
		}
		std::reverse(moves.begin(), moves.end());
		return moves;
	}

	Result step() {
		Result result = Result(_timeComplexity, _sizeComplexity);
		StateDistance<ID> e;
		State state;
		Distance dist;
		std::array<pair<State, Score>, 18> children;
		bool is_final = false;

		for (int i = 0; i < 100000; i++) {
			if (_openCount <= 0)
				throw std::logic_error("A No opened state, scount is " +
				                       std::to_string(_openCount));
			// pop best state
			e = get_smallest_state(&is_final);
			state = e.state;
			dist = e.distance;

			// if final, stop step and signal full stop
			if (is_final) {
				result.finished = true;
				break;
			}

			// check if node was already expanded
			auto insert_result = _universe.insert(state);
			if (insert_result.second == false) {
				continue;
			}

			// get children
			int count = state.get_candidates(children);
			// for every children
			for (int i = 0; i < count; i++) {

				State &child = children[i].first;
				Score score = children[i].second;

				store_state(child, dist + 1, score);

				_openCount++;
				_timeComplexity++;
				if (_openCount > _sizeComplexity)
					_sizeComplexity = _openCount;
			}
		}
		result.movements = get_path(state, dist);
		result.cube = state.get_cube();
		result.weight = state.calculate_score();
		return result;
	}

	~Solver() {}
};
