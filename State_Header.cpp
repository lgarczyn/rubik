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
#include <random>
#include <sstream>
#include <tr1/functional>

constexpr State::State() : _id(), _movement(None), _weight(0), _distance(0) {}

constexpr State::State(bool is_del)
    : _id((ID){is_del ? UINT_MAX : UINT_MAX - 1,
          is_del ? UINT_MAX : UINT_MAX - 1,
          is_del ? UINT_MAX : UINT_MAX - 1}),
      _movement(None),
      _weight(UCHAR_MAX),
      _distance(UCHAR_MAX) {}

inline State::State(const ID &id) : State() { _apply_data(data_from_id(id)); }

inline void State::_apply_data(const Data &data) {
	_id = id_from_data(data);
	_weight = Heuristics::ValidFunction(data);
	// std::cout << "heuristics: " << (int)_weight << std::endl;
	//_weight = std::max((int)_weight, Heuristics::ValidFunction(data));
}

inline void State::_apply_scramble(Data &data, const string &scramble) {
	std::stringstream ss = std::stringstream(scramble);

	while (ss) {
		Movement m;
		int turns = 1;
		int c = ss.get();

		switch (c) {
		case 'F':
			m = Front;
			break;
		case 'R':
			m = Right;
			break;
		case 'U':
			m = Up;
			break;
		case 'B':
			m = Back;
			break;
		case 'L':
			m = Left;
			break;
		case 'D':
			m = Down;
			break;
		default:
			continue;
		}
		c = ss.peek();
		switch (c) {
		case '\'':
			turns = 3;
			break;
		case '2':
			turns = 2;
			break;
		}
		_apply_movement(data, m, turns);
	}
}

inline State::State(const std::string &scramble) : State() {
	Data data = data_from_id(ID());
	_apply_scramble(data, scramble);
	_apply_data(data);
}

inline State::State(const State &parent, const std::string &scramble)
    : State() {
	print_map(parent);
	print_map(parent);
	print_map(parent);
	Data data = data_from_id(parent._id);
	_apply_scramble(data, scramble);
	_apply_data(data);
}

inline State::State(int scramble_count) : State() {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(Movement_Start, Movement_End - 1);
	std::uniform_int_distribution<int> turns(1, 3);

	std::cout << "GENERATING CUBE WITH PATTERN:" << std::endl;

	Data data = data_from_id(ID());
	int previous = None;
	for (int i = 0; i < scramble_count; i++) {
		int m = uni(rng);
		while (m == previous)
			m = uni(rng);
		previous = m;
		int t = turns(rng);

		_apply_movement(data, (Movement)m, t);

		if (t == 2)
			m |= Halfturn;
		else if (t == 3)
			m |= Reversed;
		std::cout << (Movement)m;
	}
	std::cout << std::endl;
	_apply_data(data);
}

inline State::State(const State &parent, State::Movement m, const Data &data)
    : State() {
	_movement = m;
	_distance = parent._distance + 1;
	_apply_data(data);
}

inline State::State(const State &parent, State::Movement m) : State() {
	if ((m & Mask) == None)
		throw std::logic_error(
		    "None is not an allowed move, use copy constructor");

	_movement = m;
	_distance = parent._distance + 1;

	Data data = data_from_id(parent._id);
	_apply_movement(data, m, _get_turns(m));
	_apply_data(data);
}

constexpr int State::_get_turns(Movement m) {
	switch (m & ~Mask) {
	case Reversed:
		return 3;
	case Halfturn:
		return 2;
	default:
		return 1;
	}
}

inline State::State(const State &clone) { *this = clone; }

inline State &State::operator=(const State &ra) {
	_id = ra._id;
	_weight = ra._weight;
	_movement = ra._movement;
	_distance = ra._distance;
	return *this;
}

constexpr bool is_move_duplicate(State::Movement a, State::Movement b) {
	if (a == State::Down && b == State::Up)
		return true;
	if (a == State::Back && b == State::Front)
		return true;
	if (a == State::Left && b == State::Right)
		return true;
	return false;
}

constexpr void State::get_candidates(std::vector<State> &candidates) const {
	// get cube of current state
	Data data = data_from_id(_id);
	// get movement of current state
	Movement m = (Movement)(_movement & Mask);

	// foreach possible movement family
	for (int n = Movement_Start; n < Movement_End; n++) {
		// if the movement is in same family as current, skip
		if (m == n)
			continue;
		// if the previous movement was opposite to the current, and had
		// priority, ignore this one
		if (is_move_duplicate(m, (Movement)n))
			continue;
		// rotate 90d, build, then repeat
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)n, data));
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Halfturn), data));
		_apply_movement(data, (Movement)n);
		candidates.push_back(State(*this, (Movement)(n | Reversed), data));
		// reset data, can be changed for stored data copy, or another
		// data_from_id
		_apply_movement(data, (Movement)n);
	}
}

constexpr bool State::is_final() const { return _weight == 0; }

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

constexpr uint State::get_weight(void) const { return this->_weight; }

constexpr State::Movement State::get_movement(void) const {
	return (Movement)_movement;
}

constexpr uint State::get_distance() const { return _distance; }

inline std::ostream &operator<<(std::ostream &s, const State::Movement c) {
	bool reversed = c & State::Reversed;
	bool halfturn = c & State::Halfturn;

	switch (c & State::Mask) {
	case State::Left:
		return s << (reversed ? "L'" : (halfturn ? "L2" : "L"));
	case State::Right:
		return s << (reversed ? "R'" : (halfturn ? "R2" : "R"));
	case State::Up:
		return s << (reversed ? "U'" : (halfturn ? "U2" : "U"));
	case State::Down:
		return s << (reversed ? "D'" : (halfturn ? "D2" : "D"));
	case State::Front:
		return s << (reversed ? "F'" : (halfturn ? "F2" : "F"));
	case State::Back:
		return s << (reversed ? "B'" : (halfturn ? "B2" : "B"));
	default:
		return s << " ERROR:" << (int)c << ' ';
	}
}

constexpr bool State::operator==(const State &ra) const {
	return (_id.corners == ra._id.corners) &&
	       (_id.borders_pos == ra._id.borders_pos) &&
	       (_id.borders_rot == ra._id.borders_rot);
}

constexpr Score State::indexer_astar(const State &state) {
	return state.get_weight() + state.get_distance() * score_multiplier;
}

constexpr Score State::indexer_greedy(const State &state) {
	return state.get_weight();
}

constexpr Score State::indexer_uniform(const State &state) {
	return state.get_distance();
}
