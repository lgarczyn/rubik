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
#include <algorithm>
#include <iomanip>
#include <limits>

template <class ID>
constexpr State<ID>::State() : _id(), _movement(Move::None) {}

template <class ID>
constexpr State<ID>::State(const ID &id, Move move)
    : _id(id), _movement(move) {}

template <class ID>
constexpr State<ID>::State(const Data &data, Move move) : State() {
	_movement = move;
	_id = id_from_data(data);
}

template <class ID>
constexpr State<ID>::State(const State &clone) : State() { *this = clone; }

template <class ID>
constexpr State<ID> &State<ID>::operator=(const State &ra) {
	_id = ra._id;
	_movement = ra._movement;
	return *this;
}

template <class ID>
constexpr State<ID> State<ID>::get_child(Move m) const {
	State<ID> r;

	r._movement = m;

	Data data = data_from_id(_id);
	_apply_movement(data, m);
	r._id = id_from_data(data);
	return r;
}

template <class ID>
inline State<ID> State<ID>::get_scrambled(const vector<Move> &moves) const {
	Data d = data_from_id(_id);

	for (uint i = 0; i < moves.size(); i++) {
		_apply_movement(d, moves[i]);
	}
	return State<ID>(d, Move());
}

template <class ID>
constexpr Score State<ID>::calculate_score() const {
	return Heuristics::ValidFunction(State<ID>::data_from_id(_id));
}

template <class ID>
constexpr int State<ID>::get_candidates(std::array<pair<State<ID>, Score>, 18> &candidates) const {
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

		// reset data to parent data
		Data data = data_copy;

		for (int i = 1; i <= 3; i++) {
			// rotate 90d, build, then repeat
			_apply_movement(data, (Move::Direction)n);

			Move move = Move((Move::Direction)n, i);
			candidates[count].first = State<ID>(data, move);
			candidates[count].second = Heuristics::ValidFunction(data);
			count++;
		}
	}
	return count;
}

//TODO burn this mess
template <>
constexpr int State<IDG2>::get_candidates(std::array<pair<State<IDG2>, Score>, 18> &candidates) const {
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

		// reset data to parent data
		Data data = data_copy;

		for (int i = 1; i <= 3; i++) {
			if (n != Move::Up && n != Move::Down && i == 3)
				break;

			// rotate 90d, build, then repeat
			_apply_movement(data, (Move::Direction)n);

			if (n != Move::Up && n != Move::Down && i == 1)
				continue;

			Move move = Move((Move::Direction)n, i);
			candidates[count].first = State<IDG2>(data, move);
			candidates[count].second = Heuristics::ValidFunction(data);
			count++;
		}
	}
	return count;
}

template <class ID>
constexpr State<ID> State<ID>::get_parent() const {
	// get cube of current state
	Data data = data_from_id(_id);
	// get movement of current state
	_apply_movement(data, _movement.inverse());
	State<ID> r = State<ID>(data, Move());
	return r;
}

template <>
constexpr bool State<ID>::is_solvable() const {
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

template <class ID>
constexpr const ID &State<ID>::get_id(void) const { return _id; }

template <class ID>
constexpr Data State<ID>::get_data() const { return data_from_id(_id); }

template <class ID>
constexpr Cube State<ID>::get_cube() const { return cube_from_id(_id); }

template <class ID>
constexpr Move State<ID>::get_movement(void) const {
	return _movement;
}

template <class ID>
constexpr bool State<ID>::operator==(const State<ID> &ra) const noexcept {
	const ID &lid = this->get_id();
	const ID &rid = ra.get_id();

	return lid == rid;
}

template <class ID>
inline bool custom_pred<ID>::operator()(const State<ID> &la, const State<ID> &ra) const noexcept {
	return (la == ra);
}

template <class ID>
inline bool custom_cmp<ID>::operator()(const State<ID> &la, const State<ID> &ra) const noexcept {
	const ID &lid = la.get_id();
	const ID &rid = ra.get_id();

	return (lid < rid);
}

template <class ID>
inline size_t custom_hash<ID>::operator()(const State<ID> &l) const noexcept {
	const ID &id = l.get_id();

	return id_hash(id);
}

template <class ID>
constexpr Cube State<ID>::cube_from_data(const Data &data) {
	return Encoding::cube_from_data(data);
}
template <class ID>
constexpr ID State<ID>::id_from_data(const Data &data) {
	return Encoding::id_from_data<ID>(data);
}
template <class ID>
constexpr Data State<ID>::data_from_id(const ID id) {
	return Encoding::data_from_id(id);
}
template <class ID>
constexpr Cube State<ID>::cube_from_id(const ID id) {
	return Encoding::cube_from_id(id);
}

template <class ID>
constexpr void State<ID>::_apply_movement(Data &data, Move::Direction m) {
	Encoding::_apply_movement(data, m);
}

template <class ID>
constexpr void State<ID>::_apply_movement(Data &data, Move m) {
	Encoding::_apply_movement(data, m);
}
