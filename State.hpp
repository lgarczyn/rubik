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

using indexer = Score (*)(const State&);
using StateRef = std::shared_ptr<State>;

class State {
	public:
		static const Data solution;
		static const DataFull solution_full;
		static const Finder solution_finder;
		static int stateCount;
		static Score initial_score;
		static indexer	get_index;

		enum Movement {
			None = 0,
			Up = 1,
			Right = 2,
			Down = 3,
			Left = 4,
			Front = 5,
			Back = 6,

			Movement_Start = 1,
			Movement_End = 7,

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

		State();
		State(const State& clone);
		State(const string& scramble);
		State(State* parent, const Movement direction);
		State(int scramble_count);
		~State();
		void							update();

		bool							operator<(const State& ra) const;

		void							apply_scramble(const string& scramble);
		void							apply_movement(Movement m);
		std::vector<Movement>			get_movements() const;
		Movement						get_movement() const;
		Score							get_distance() const;
		Score 							get_weight() const;
		const Data&						get_data() const;
		void							get_candidates(std::vector<StateRef>& candidates);
		bool 							is_final() const;

		static Score					indexer_astar(const State&);
		static Score					indexer_uniform(const State&);
		static Score					indexer_greedy(const State&);

		static int						compare(const Data& a, const Data& b);

	private:
		static Data						_calculate_solution();
		static DataFull					_calculate_solution_full();
		static Finder					_calculate_finder(const Data& data);

		Data							_data;
		Score							_weight;
		Score							_distance;
		Movement						_movement;
		StateRef						_parent;
};

std::ostream& operator<< (std::ostream& s, const State::Movement c);

struct custom_hash
{
	public:
		size_t operator()(const State* l) const noexcept;
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
