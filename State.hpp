/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 14:16:53 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/18 19:03:56 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <list>
#include <vector>
#include <array>
#include <functional>
#include <iostream>
#include "Types.hpp"

class State;

using indexer = score (*)(const State*);

class State {
	public:
		static State solution;
		static int stateCount;
		static const int size = 3;
		static score initial_score;
		static indexer	get_index;

		enum Indexes {
			Index_Up = 0,
			Index_Right = 1,
			Index_Down = 2,
			Index_Left = 3,
			Index_Front = 4,
			Index_Back = 5,
		};

		enum Movement {
			None = 0,
			Up = 1,
			Right = 2,
			Down = 3,
			Left = 4,
			Front = 5,
			Back = 6,
			Mask = 15,
			Reversed = 16,
			Halfturn = 32,
		};

		State(const std::string& scramble);
		State(State* parent, const Movement direction);
		~State();

		void							applyScramble(const std::string& scramble);
		void							applyMovement(Movement m);
		std::vector<State::Movement>	*get_movements() const;
		Movement						get_movement() const;
		int 							get_distance() const;
		score 							get_weight() const;
		void 							set_weight(score s);
		void 							set_distance(int d);
		bool 							is_final() const;
		void							set_parent(State* p);
		State*							get_parent(void) const;
		Data&							get_data();
		const Data&						get_data() const;
		void							get_candidates(State** candidates, State* root);

		static void						init();
		static score					indexer_astar(const State *);
		static score					indexer_uniform(const State *);
		static score					indexer_greedy(const State *);

	private:
		std::string		_data;
		score			_weight;
		int 			_distance;
		Movement 		_movement;
		State*			_parent;
};

std::ostream& operator<< (std::ostream& s, const State::Movement c);

struct custom_hash
{
	public:
		size_t operator()(const State* x) const noexcept;
};

struct custom_equal_to
{
	public:
		bool operator()(const State* a, const State* b) const noexcept;
};

struct custom_less
{
	public:
		bool operator()(const State* a, const State* b);
};
