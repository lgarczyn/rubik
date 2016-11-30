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

#include <vector>
#include <array>
#include <functional>
#include <iostream>
#include "Types.hpp"

class State;

using indexer = score (*)(const State&);

class State {
	public:
		static const Data solution;
		static int stateCount;
		static score initial_score;
		static indexer	get_index;

		enum Indexes {
			Index_Up = 0,
			Index_Front = 1,
			Index_Right = 2,
			Index_Back = 3,
			Index_Left = 4,
			Index_Down = 5,
			Index_Start = 0,
			Index_Len = 6,
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

			R_None = Reversed | None,
			R_Up = Reversed | Up,
			R_Right = Reversed | Right,
			R_Down = Reversed | Down,
			R_Left = Reversed | Left,
			R_Front = Reversed | Front,
			R_Back = Reversed | Back,

			H_None = Halfturn | None,
			H_Up = Halfturn | Up,
			H_Right = Halfturn | Right,
			H_Down = Halfturn | Down,
			H_Left = Halfturn | Left,
			H_Front = Halfturn | Front,
			H_Back = Halfturn | Back,
		};

		State(const string& scramble = "");
		State(State* parent, const Movement direction);
		~State();

		void							applyScramble(const string& scramble);
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
		void							get_candidates(State** candidates);

		static score					indexer_astar(const State&);
		static score					indexer_uniform(const State&);
		static score					indexer_greedy(const State&);

	private:
		static Data		_get_solution();

		Data			_data;
		string			_id;
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
