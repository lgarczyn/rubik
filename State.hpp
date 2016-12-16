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

#ifndef STATE_HPP
# define STATE_HPP

#include <functional>
#include <vector>
#include <iostream>
#include "Types.hpp"

class State;

using indexer = Score (*)(const State&);

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

			Mask = 7,
			Reversed = 8,
			Halfturn = 16,

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

		//solution lookup
		static const Data				solution;
		static const Finder				solution_finder;
		static const UIDFinder			uid_finder;
		static const Color				solution_colors[];

		//constructors
		State();
		State(bool is_del);
		State(const State& clone);
		State(const string& scramble);
		State(int scramble_count);
		State(const State& parent, Movement direction);
		State& operator=(const State& ra);
		bool operator==(const State& ra) const;

		//getters
		Movement						get_movement() const;
		uint							get_distance() const;
		uint 							get_weight() const;
		const ID&						get_id() const;
		bool 							is_final() const;

		//logic wrappers
		void							get_candidates(std::vector<State>& candidates) const;

		//ID Data conversions
		//void							inflate(Data& data) const;
		Data							to_data() const;
		static constexpr ID				id_from_data(const Data data);
		static constexpr Data			data_from_id(const ID id);

		//indexers
		static indexer					get_index;
		static Score					indexer_astar(const State&);
		static Score					indexer_uniform(const State&);
		static Score					indexer_greedy(const State&);

		//debug
		ID&								_get_id();
		void							update_weight();
	private:
		//optimized constructor
		State(const State& parent, Movement direction, const Data& data);

		//calculations
		void							_apply_data(const Data& data);
		static void						_apply_scramble(Data& data, const string& scramble);
		static void						_apply_movement(Data& data, Movement m);
		static constexpr Data			_calculate_solution();
		static constexpr UIDFinder		_calculate_uid_finder(const Data& data);
		static constexpr Finder			_calculate_finder(const Data& data);

		//members, should add up to 128bits :D
		ID								_id;
		uint16_t						_movement:16;
		uchar							_weight:8;
		uchar							_distance:8;
};
std::ostream&					operator<<(std::ostream& s, const State::Movement c);

struct custom_hash
{
	public:
		size_t operator()(const State& l) const noexcept;
};

#endif
