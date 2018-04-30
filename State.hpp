/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 22:24:12 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/25 03:39:55 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_HPP
#define STATE_HPP

#include "Move.hpp"
#include "Types.hpp"
#include <bitset>
#include <functional>
#include <iostream>

class State {
  public:
	// constructors
	// construct solved state
	constexpr State();
	// construct state from members
	constexpr State(const ID &id, Score score, Distance distance, Move move);
	constexpr State(const Data &data, Distance distance, Move move);
	// construct state from another state
	constexpr State(const State &clone);
	constexpr State &operator=(const State &ra);
	// factories
	static constexpr State get_empty_key();
	static constexpr State get_deleted_key();

	// getters
	constexpr Move get_movement() const;
	constexpr uint get_distance() const;
	constexpr uint get_weight() const;
	constexpr Score get_index() const;
	constexpr const ID &get_id() const;
	constexpr bool is_final() const;
	constexpr bool is_solvable() const;

	// logic wrappers
	constexpr void get_candidates(vector<State> &candidates) const;
	constexpr State get_scrambled(const vector<Move> &moves) const;
	constexpr State get_parent() const;
	constexpr State get_child(Move m) const;

	// ID Cube conversions
	constexpr Data get_data() const;
	constexpr Cube get_cube() const;
	constexpr static ID id_from_data(const Data &data);
	constexpr static Data data_from_id(const ID id);
	constexpr static Cube cube_from_data(const Data &data);
	constexpr static ID id_from_cube(const Cube &cube);
	constexpr static Cube cube_from_id(const ID data);

	// Operators
	constexpr bool operator==(const State &ra) const noexcept;

  private:
	// calculations
	constexpr void _apply_data(const Data &data);

	constexpr static void _apply_movement(Data &data, Move m);
	constexpr static void _apply_movement(Data &data, Move::Direction m);
	constexpr static Finder _calculate_finder(const Cube &Cube);

	// members, should add up to 128bits :D
	ID _id;
	Score _weight;
	Distance _distance;
	Move _movement;
	std::bitset<18> _mask;

  public:
	static const Data solution_data;
	static const Cube solution_cube;
	static const Finder solution_finder;
	static const Color solution_colors[];
};

struct custom_hash {
  public:
	size_t operator()(const State &l) const noexcept;
};

struct custom_pred {
  public:
	bool operator()(const State &la, const State &ra) const noexcept;
};

struct custom_cmp {
  public:
	bool operator()(const State &la, const State &ra) const noexcept;
};

#include "State_Encoding.cpp"
#include "State_Header.cpp"

#endif
