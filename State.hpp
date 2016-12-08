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

#include "ThreadPool.hpp"

using indexer = Score (*)(const State&);
using StateRef = std::shared_ptr<State>;

class State {
	public:

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


		struct MovementNode;
		using MovementRef = std::shared_ptr<MovementNode>;
		struct MovementNode{


				Movement value;
				MovementRef parent;

				MovementNode(Movement _m, MovementRef& _p);
				MovementNode(Movement _m);
		};

		struct ThreadData
		{
			ThreadData(){};
			ThreadData(State* _parent, State::Movement _move):
				parent(_parent),
				move(_move)
			{}

			~ThreadData(){};

			State* parent;
			State::Movement move;
		};
		using Pool = ThreadPool<ThreadData, StateRef>;

		static Pool pool;
		static const Data solution;
		static const Finder solution_finder;
		static const UIDFinder uid_finder;
		static const Color solution_colors[];
		static int stateCount;
		static Score initial_score;
		static indexer	get_index;

		State();
		State(const State& clone);
		State(const string& scramble);
		State(State* parent, const Movement direction);
		State(int scramble_count);
		~State();

		void							kill();

		std::vector<Movement>			get_movements() const;
		Movement						get_movement() const;
		Score							get_distance() const;
		Score 							get_weight() const;
		const Data&						get_data() const;
		Data&							get_data();
		Data*							get_data_safe() const;
		const ID&						get_id() const;
		void							get_candidates(std::vector<StateRef>& candidates);
		bool 							is_final() const;
		bool 							is_alive() const;

		static Score					indexer_astar(const State&);
		static Score					indexer_uniform(const State&);
		static Score					indexer_greedy(const State&);

		ID								_id;
	private:
		void							apply_scramble(const string& scramble);
		void							apply_movement(Movement m);
		void							inflate();
		void							inflate(Data& data) const;
		void							deflate();

		void							_init();
		void							_finish();

		static constexpr Data			_calculate_solution();
		static constexpr UIDFinder		_calculate_uid_finder(const Data& data);
		static constexpr Finder			_calculate_finder(const Data& data);

		Data*							_data;
		Score							_weight;
		Score							_distance;
		MovementRef						_movement;
};

std::ostream& operator<< (std::ostream& s, const State::Movement c);

struct custom_hash
{
	public:
		size_t operator()(const StateRef& l) const noexcept;
};

struct custom_equal_to
{
	public:
		bool operator()(const StateRef& a, const StateRef& b) const noexcept;
};

struct custom_less
{
	public:
		bool operator()(const StateRef& a, const StateRef& b);
};
