//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#pragma once

#include <string>
#include <list>
#include <set>
#include <map>
#include "State.hpp"
#include <unordered_map>
#include <sparsehash/dense_hash_map>

#define USE_LIST

//using set = std::unordered_set<StateRef, custom_hash, custom_equal_to>;
using Set = std::vector<State>;
using Map = std::map<int, Set>;

using Movements = std::vector<uint16_t>;
using Universe = std::unordered_map<State, Movements, custom_hash>;
//using Universe = google::dense_hash_map<State, Movements, custom_hash>;

class Solver {
	public:

		class Result
		{
			public:
				int timeComplexity;
				int sizeComplexity;
				State actual_state;
				std::vector<uint16_t> movements;
				bool finished;
				Result(int timeComplexity, int sizeComplexity);
				Result();
		};

		/*struct Node
		{
			StateRef value;
			std::unique_ptr<Node> left;
			std::unique_ptr<Node> right;
		};
		using NodeRef = std::unique_ptr<Node>;*/
		//StateRef *get_universe_position(StateRef state);

		Solver();
		Solver(State initial);
		void setup(State initial);
		Result step();
		Set& get_opened_set(const State& state);
		State get_smallest_state();
		void print_mem();
		~Solver();

	private:
		Map _opened;
		Universe _universe;
		int _timeComplexity;
		int _sizeComplexity;
		int _openCount;
};
