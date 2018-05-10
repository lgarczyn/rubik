/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State_Header.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:39:39 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/25 03:38:47 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Heuristics.hpp"
#include "tools.hpp"
#include <algorithm>
#include <iomanip>
#include <limits>

constexpr State::State() : _id(), _movement(Move::None) {}

constexpr State::State(const ID &id, Move move)
    : _id(id), _movement(move) {}

constexpr State::State(const Data &data, Move move) : State() {
	_movement = move;
	_id = id_from_data(data);
}

constexpr State::State(const State &clone) : State() { *this = clone; }

constexpr State &State::operator=(const State &ra) {
	_id = ra._id;
	_movement = ra._movement;
	return *this;
}

constexpr State State::get_child(Move m) const {
	State r;

	r._movement = m;

	Data data = data_from_id(_id);
	_apply_movement(data, m);
	r._id = id_from_data(data);
	return r;
}

inline State State::get_scrambled(const vector<Move> &moves) const {
	Data d = data_from_id(_id);

	for (uint i = 0; i < moves.size(); i++) {
		_apply_movement(d, moves[i]);
	}
	return State(d, Move());
}

constexpr Score State::calculate_score() const {
	return Heuristics::ValidFunction(State::data_from_id(_id));
}

inline int State::get_candidates(std::array<pair<State, Score>, 18> &candidates) const {
	// get cube of current state
	Data data_copy = data_from_id(_id);
	// get movement of current state

	int count = 0;
	// foreach possible movement family
	for (int n = Move::Direction_Start; n <= Move::Direction_End; n++) {
		// if the movement is in same family as current, skip
		if (_movement.direction == n)
			continue;
		// if the previous movement was opposite to the current, and had
		// priority, ignore this one
		if (Move::is_commutative(_movement.direction, n))
			continue;
		Data data = data_copy;
		Move move = Move((Move::Direction)n);
		// rotate 90d, build, then repeat
		_apply_movement(data, move.direction);
		candidates[count].first = State(data, move);
		candidates[count].second = Heuristics::ValidFunction(data);
		count++;

		move.halfturn = true;
		_apply_movement(data, move.direction);
		candidates[count].first = State(data, move);
		candidates[count].second = Heuristics::ValidFunction(data);
		count++;

		move.halfturn = false;
		move.reversed = true;
		_apply_movement(data, move.direction);
		candidates[count].first = State(data, move);
		candidates[count].second = Heuristics::ValidFunction(data);
		count++;
	}
	return count;
}

constexpr State State::get_parent() const {
	// get cube of current state
	Data data = data_from_id(_id);
	// get movement of current state
	_apply_movement(data, _movement.inverse());
	State r = State(data, Move());
	return r;
}

constexpr bool State::is_solvable() const {
	Data data = data_from_id(_id);

	// corner check
	{
		int sum = 0;
		for (int i = 0; i < 8; i++)
			sum += data.corners[i].rot_id;
		if (sum % 3) {
			return false;
		}
	}
	//TODO use count_ones
	{
		int sum = 0;
		for (int i = 0; i < 12; i++)
			sum += data.borders[i].rot_id;
		if (sum % 2) {
			return false;
		}
	}
	int sumparity_corner = 0;
	{
		{
			for (int i = 0; i < 8; i++) {
				int j = data.corners[i].cube_id;
				if (j != i) {
					std::swap(data.corners[i], data.corners[j]);
					sumparity_corner++;
				}
			}
		}
	}
	int sumparity_border = 0;
	{
		for (int i = 0; i < 12; i++) {
			int j = data.borders[i].cube_id;
			if (j != i) {
				std::swap(data.borders[i], data.borders[j]);
				sumparity_border++;
			}
		}
	}
	if (sumparity_border % 2 != sumparity_corner % 2)
		return false;
	return true;
}

constexpr const ID &State::get_id(void) const { return _id; }

constexpr Data State::get_data() const { return data_from_id(_id); }

constexpr Cube State::get_cube() const { return cube_from_id(_id); }

constexpr Move State::get_movement(void) const {
	return _movement;
}

constexpr bool State::operator==(const State &ra) const noexcept {
	const ID &lid = this->get_id();
	const ID &rid = ra.get_id();

	return lid == rid;
}

inline bool custom_pred::operator()(const State &la, const State &ra) const noexcept {
	return (la == ra);
}

inline bool custom_cmp::operator()(const State &la, const State &ra) const noexcept {
	const ID &lid = la.get_id();
	const ID &rid = ra.get_id();

	if (lid.corners_pos != rid.corners_pos)
		return lid.corners_pos < rid.corners_pos;
	if (lid.corners_rot != rid.corners_rot)
		return lid.corners_rot < rid.corners_rot;
	if (lid.borders_pos != rid.borders_pos)
		return lid.borders_pos < rid.borders_pos;
	return lid.borders_rot < rid.borders_rot;
}

inline size_t custom_hash::operator()(const State &l) const noexcept {
	const ID &id = l.get_id();

	//TODO: check which is better
	//std::tr1::hash<size_t> hash;
	return (id.borders_rot ^ id.borders_pos) |
	       ((uint64_t)id.corners_rot << 32) |
	       ((uint64_t)id.corners_pos << 48);
}
