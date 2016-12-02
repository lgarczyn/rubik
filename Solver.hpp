//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#pragma once

#include <string>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include "State.hpp"
#define MAX_SOLUTION_LENGTH ((int)10000)
#define SOLVER_BUCKET_SIZE ((int)10000)

using set = std::unordered_set<StateRef>;//, custom_hash, custom_equal_to>;
using map = std::map<int, set>;

class Solver {
	public:

		class Result
		{
			public:
				int timeComplexity;
				int sizeComplexity;
				StateRef actual_state;
				std::vector<State::Movement> movements;
				bool finished;
				Result(int timeComplexity, int sizeComplexity);
		};

		struct Node
		{
			StateRef value;
			std::unique_ptr<Node> left;
			std::unique_ptr<Node> right;
		};
		using NodeRef = std::unique_ptr<Node>;

		Solver(State& initial, bool forget);
		Result step();
		set& get_opened_set(StateRef state);
		StateRef *get_universe_position(StateRef state);
		StateRef get_smallest_state();
		~Solver();

	private:
		map _opened;
		std::unique_ptr<Node> _universe;

		int _timeComplexity;
		int _sizeComplexity;
		int _openCount;
		bool _forget;
};
