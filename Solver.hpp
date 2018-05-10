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

#include "State.hpp"
#include <list>
#include <map>
#include <set>
#include <string>

//using set = std::unordered_set<State, custom_hash>;
using Set = vector<pair<State, Distance>>;
using Map = std::map<int, Set>;

//#define DENSE_MAP
//#define SPARSE_MAP
//#define BTREE_MAP

#ifdef DENSE_MAP
#include <sparsehash/dense_hash_set>
using Universe = google::dense_hash_set<State, custom_hash>;
#elif defined SPARSE_MAP
#include <sparsehash/sparse_hash_set>
using Universe = google::sparse_hash_set<State, custom_hash>;
#elif defined BTREE_MAP
#include "lib/cpp-btree/btree_set.h"
using Universe = btree::btree_set<State, custom_cmp>;
#else
#include <unordered_set>
using Universe = std::unordered_set<State, custom_hash, custom_pred>;
#endif

class Solver {
  public:
	class Result {
	  public:
		int timeComplexity;
		int sizeComplexity;
		State actual_state;
		Score actual_weight;
		Distance actual_distance;
		vector<Move> movements;
		bool finished;
		Result(int timeComplexity, int sizeComplexity);
		Result();
	};

	Solver();
	Solver(State initial, bool forget);
	void setup(State initial, bool forget);
	Result step();
	int clean_duplicates();
	void store_state(const State &state, Distance dist, Score score);
	pair<State, Distance> get_smallest_state(bool *is_final);
	int get_real_open_count();
	void print_mem();
	~Solver();

	int _openCount;

  private:
	Map _opened;
	Universe _universe;
	int _timeComplexity;
	int _sizeComplexity;
	bool _forget;
};
