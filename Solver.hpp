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
#include <sparsehash/sparse_hash_map>
#include <string>
#include <unordered_map>

// using set = std::unordered_set<StateRef, custom_hash, custom_equal_to>;
using Set = std::vector<State>;
using Map = std::map<int, Set>;

using Movements = std::vector<uint16_t>;
//using Universe = std::unordered_map<State, Movements, custom_hash>;
using Universe = google::sparse_hash_map<State, Movements, custom_hash>;

class Solver {
  public:
	class Result {
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
	// StateRef *get_universe_position(StateRef state);

	Solver();
	Solver(State initial, bool forget);
	void setup(State initial, bool forget);
	Result step();
	int clean_duplicates();
	Set &get_opened_set(const State &state);
	State get_smallest_state();
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
