/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 14:18:52 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/18 19:04:49 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "State.hpp"
#include "Heuristics.hpp"
#include <sstream>
#include <random>

int					State::stateCount = 0;
Score				State::initial_score = 0;
indexer				State::get_index = indexer_astar;

const Data			State::solution = _calculate_solution();
const Finder		State::solution_finder = _calculate_finder(solution);

State::State() {
	_data = solution;
	_finder = solution_finder;
	_weight = 0;
	_distance = 0;
	_movement = None;
	_parent = nullptr;
	stateCount++;
}

State::State(const std::string& scramble):State(){
	applyScramble(scramble);
	update();
}

State::State(State* parent, State::Movement m):State() {

	_data = parent->_data;
	_parent = parent;
	_distance = parent->_distance + 1;
	_movement = m;

	applyMovement(m);
	update();
}

State::State(int scramble_count):State(){
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0,6);
	std::uniform_int_distribution<int> bo(0,2);

	for (int i = 0; i < scramble_count; i++) {
		Movement m = (Movement)uni(rng);
		int c = bo(rng);
		if (c == 1)
			m = (Movement)(m | Reversed);
		else if (c == 2)
			m = (Movement)(m | Halfturn);

		applyMovement(m);
	}
	update();
}

void State::update() {
	_finder = _calculate_finder(_data);
	_weight = Heuristics::HeuristicFunction(_data);
}

State::~State()
{
	stateCount--;
}

int State::compare(const Data& a, const Data& b) {
	for (int s = 0; s < 6; s++)
		for (int x = 0; x < 6; x++)
			for (int y = 0; y < 6; y++)
				if (x != 1 || y != 1) {
					if (a[s][x][y].color < b[s][x][y].color)
						return -1;
					else if (a[s][x][y].color > b[s][x][y].color)
						return 1;
				}
	return 0;
}

void	State::get_candidates(State** candidates)
{
	int i = 0;

	Movement m = _movement;

	for (Movement n = Up; n <= Down; n = (Movement)((int)n + 1)) {
		Movement nr = (Movement)(n | Reversed);
		Movement nh = (Movement)(n | Halfturn);

		if (m != n) candidates[i++] = new State(this, n);
		if (m != nr) candidates[i++] = new State(this, nr);
		if (m != nh) candidates[i++] = new State(this, nh);
	}

	candidates[i] = nullptr;
}

Data				State::_calculate_solution() {
	Data			data;

	int uid = 0;
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++) {
			data[Index_Up][x][y].color = White;
			data[Index_Front][x][y].color = Green;
			data[Index_Right][x][y].color = Red;
			data[Index_Back][x][y].color = Blue;
			data[Index_Left][x][y].color = Orange;
			data[Index_Down][x][y].color = Yellow;
		}

	for (int s = Index_Start; s < Index_Len; s++)
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++)
				data[s][x][y].face_id = uid++;

	//Adding corners ID
	data[Index_Up][0][0].cube_id = data[Index_Left][0][0].cube_id = data[Index_Back][0][2].cube_id = 1;
	data[Index_Up][2][0].cube_id = data[Index_Left][0][2].cube_id = data[Index_Front][0][0].cube_id = 2;
	data[Index_Up][0][2].cube_id = data[Index_Right][0][2].cube_id = data[Index_Back][0][0].cube_id = 3;
	data[Index_Up][2][2].cube_id = data[Index_Right][0][0].cube_id = data[Index_Front][0][2].cube_id = 4;

	data[Index_Down][0][0].cube_id = data[Index_Left][2][2].cube_id = data[Index_Front][2][0].cube_id = 5;
	data[Index_Down][2][0].cube_id = data[Index_Left][2][0].cube_id = data[Index_Back][2][2].cube_id = 6;
	data[Index_Down][0][2].cube_id = data[Index_Right][2][0].cube_id = data[Index_Front][2][2].cube_id = 7;
	data[Index_Down][2][2].cube_id = data[Index_Right][2][2].cube_id = data[Index_Back][2][0].cube_id = 8;

	//Adding borders ID
	data[Index_Up][0][1].cube_id = data[Index_Back][0][1].cube_id = 9;
	data[Index_Up][1][0].cube_id = data[Index_Left][0][1].cube_id = 10;
	data[Index_Up][2][1].cube_id = data[Index_Front][0][1].cube_id = 11;
	data[Index_Up][1][2].cube_id = data[Index_Right][0][1].cube_id = 12;

	data[Index_Down][0][1].cube_id = data[Index_Front][2][1].cube_id = 13;
	data[Index_Down][1][0].cube_id = data[Index_Left][2][1].cube_id = 14;
	data[Index_Down][2][1].cube_id = data[Index_Back][2][1].cube_id = 15;
	data[Index_Down][1][2].cube_id = data[Index_Right][2][1].cube_id = 16;

	data[Index_Front][1][2].cube_id = data[Index_Right][1][0].cube_id = 17;
	data[Index_Right][1][2].cube_id = data[Index_Back][1][0].cube_id = 18;
	data[Index_Back][1][2].cube_id = data[Index_Left][1][0].cube_id = 19;
	data[Index_Left][1][2].cube_id = data[Index_Front][1][0].cube_id = 20;

	//Adding center ID
	data[Index_Up][1][1].cube_id = 0;
	data[Index_Down][1][1].cube_id = 0;
	data[Index_Back][1][1].cube_id = 0;
	data[Index_Left][1][1].cube_id = 0;
	data[Index_Front][1][1].cube_id = 0;
	data[Index_Right][1][1].cube_id = 0;

	return data;
}

Finder				State::_calculate_finder(const Data &data) {
	Finder			finder;

	for (int s = Index_Start; s < Index_Len; s++)
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++) {
				Coord co = (Coord){s, x, y};
				int id = data[s][x][y].face_id;
				finder[id] = co;
			}
	return finder;
}

void State::applyScramble(const string& scramble) {

	std::stringstream ss = std::stringstream(scramble);

	while (ss) {
		Movement m;
		int c = ss.get();

		switch (c) {
			case 'F': m = Front; break;
			case 'R': m = Right; break;
			case 'U': m = Up; break;
			case 'B': m = Back; break;
			case 'L': m = Left; break;
			case 'D': m = Down; break;
			default: continue;
		}
		c = ss.peek();
		switch (c) {
			case '\'': m = (Movement)(m | Reversed); break;
			case '2': m = (Movement)(m | Halfturn); break;
		}
		applyMovement(m);
	}
}

void rotate_face(Face& face, int turns) {
	for (int i = 0; i < turns; i++) {
		Square tl =  face[0][0];
		face[0][0] = face[2][0];
		face[2][0] = face[2][2];
		face[2][2] = face[0][2];
		face[0][2] = tl;

		tl =         face[0][1];
		face[0][1] = face[1][0];
		face[1][0] = face[2][1];
		face[2][1] = face[1][2];
		face[1][2] = tl;
	}
}

void swap_s(Square& a, Square& b, Square& c, Square& d, int turns) {
	for (int i = 0; i < turns; i++) {
		Square t;
		t = d;
		d = c;
		c = b;
		b = a;
		a = t;
	}
}

void State::applyMovement(State::Movement m) {

	bool reversed = m & Reversed;
	bool halfturn = m & Halfturn;
	int turns = reversed ? 3 : (halfturn ? 2 : 1);

	switch (m & Mask) {
		case None : return;
		case Up :
			rotate_face(_data[Index_Up], turns);
			 //Crown is front, left, back, right
			swap_s(_data[Index_Front][0][0], _data[Index_Left][0][0], _data[Index_Back][0][0], _data[Index_Right][0][0], turns);
			swap_s(_data[Index_Front][0][1], _data[Index_Left][0][1], _data[Index_Back][0][1], _data[Index_Right][0][1], turns);
			swap_s(_data[Index_Front][0][2], _data[Index_Left][0][2], _data[Index_Back][0][2], _data[Index_Right][0][2], turns);
			break;
		case Front :
			rotate_face(_data[Index_Front], turns);
			 //Crown is up, right, down, left
			swap_s(_data[Index_Up][2][0], _data[Index_Right][0][0], _data[Index_Down][0][2], _data[Index_Left][2][2], turns);
			swap_s(_data[Index_Up][2][1], _data[Index_Right][1][0], _data[Index_Down][0][1], _data[Index_Left][1][2], turns);
			swap_s(_data[Index_Up][2][2], _data[Index_Right][2][0], _data[Index_Down][0][0], _data[Index_Left][0][2], turns);
			break;
		case Right :
			rotate_face(_data[Index_Right], turns);
			 //Crown is up, back, down, front
			swap_s(_data[Index_Up][2][2], _data[Index_Back][0][0], _data[Index_Down][2][2], _data[Index_Front][2][2], turns);
			swap_s(_data[Index_Up][1][2], _data[Index_Back][1][0], _data[Index_Down][1][2], _data[Index_Front][1][2], turns);
			swap_s(_data[Index_Up][0][2], _data[Index_Back][2][0], _data[Index_Down][0][2], _data[Index_Front][0][2], turns);
			break;
		case Back :
			rotate_face(_data[Index_Back], turns);
			 //Crown is up, left, down, right
			swap_s(_data[Index_Up][0][0], _data[Index_Left][2][0], _data[Index_Down][2][2], _data[Index_Right][0][2], turns);
			swap_s(_data[Index_Up][0][1], _data[Index_Left][1][0], _data[Index_Down][2][1], _data[Index_Right][1][2], turns);
			swap_s(_data[Index_Up][0][2], _data[Index_Left][0][0], _data[Index_Down][2][0], _data[Index_Right][2][2], turns);
			break;
		case Left :
			rotate_face(_data[Index_Left], turns);
			 //Crown is up, front, down, back
			swap_s(_data[Index_Up][0][0], _data[Index_Front][0][0], _data[Index_Down][0][0], _data[Index_Back][2][2], turns);
			swap_s(_data[Index_Up][1][0], _data[Index_Front][1][0], _data[Index_Down][1][0], _data[Index_Back][1][2], turns);
			swap_s(_data[Index_Up][2][0], _data[Index_Front][2][0], _data[Index_Down][2][0], _data[Index_Back][0][2], turns);
			break;
		case Down :
			rotate_face(_data[Index_Down], turns);
 			 //Crown is front, right, back, left
			swap_s(_data[Index_Front][2][0], _data[Index_Right][2][0], _data[Index_Back][2][0], _data[Index_Left][2][0], turns);
			swap_s(_data[Index_Front][2][1], _data[Index_Right][2][1], _data[Index_Back][2][1], _data[Index_Left][2][1], turns);
			swap_s(_data[Index_Front][2][2], _data[Index_Right][2][2], _data[Index_Back][2][2], _data[Index_Left][2][2], turns);
			break;
		default: break;
	}
}

bool State::check_continuity() const{
	int id;

	id = _data[Index_Up][0][0].cube_id;
	if (id != _data[Index_Left][0][0].cube_id || id != _data[Index_Back][0][2].cube_id)
		return false;
	id = _data[Index_Up][2][0].cube_id;
	if (id != _data[Index_Left][0][2].cube_id || id != _data[Index_Front][0][0].cube_id)
		return false;
	id = _data[Index_Up][0][2].cube_id;
	if (id != _data[Index_Right][0][2].cube_id|| id != _data[Index_Back][0][0].cube_id)
		return false;
	id = _data[Index_Up][2][2].cube_id;
	if (id != _data[Index_Right][0][0].cube_id|| id != _data[Index_Front][0][2].cube_id)
		return false;

	id = _data[Index_Down][0][0].cube_id;
	if (id != _data[Index_Left][2][2].cube_id || id != _data[Index_Front][2][0].cube_id)
		return false;
	id = _data[Index_Down][2][0].cube_id;
	if (id != _data[Index_Left][2][0].cube_id || id != _data[Index_Back][2][2].cube_id)
		return false;
	id = _data[Index_Down][0][2].cube_id;
	if (id != _data[Index_Right][2][0].cube_id|| id != _data[Index_Front][2][2].cube_id)
		return false;
	id = _data[Index_Down][2][2].cube_id;
	if (id != _data[Index_Right][2][2].cube_id|| id != _data[Index_Back][2][0].cube_id)
		return false;

	//Adding borders ID
	if (_data[Index_Up][0][1].cube_id != _data[Index_Back][0][1].cube_id)
		return false;
	if (_data[Index_Up][1][0].cube_id != _data[Index_Left][0][1].cube_id)
		return false;
	if (_data[Index_Up][2][1].cube_id != _data[Index_Front][0][1].cube_id)
		return false;
	if (_data[Index_Up][1][2].cube_id != _data[Index_Right][0][1].cube_id)
		return false;

	if (_data[Index_Down][0][1].cube_id != _data[Index_Front][2][1].cube_id)
		return false;
	if (_data[Index_Down][1][0].cube_id != _data[Index_Left][2][1].cube_id)
		return false;
	if (_data[Index_Down][2][1].cube_id != _data[Index_Back][2][1].cube_id)
		return false;
	if (_data[Index_Down][1][2].cube_id != _data[Index_Right][2][1].cube_id)
		return false;

	if (_data[Index_Front][1][2].cube_id != _data[Index_Right][1][0].cube_id)
		return false;
	if (_data[Index_Right][1][2].cube_id != _data[Index_Back][1][0].cube_id)
		return false;
	if (_data[Index_Back][1][2].cube_id != _data[Index_Left][1][0].cube_id)
		return false;
	if (_data[Index_Left][1][2].cube_id != _data[Index_Front][1][0].cube_id)
		return false;
	return true;
}

std::vector<State::Movement>* State::get_movements() const {
	std::vector<State::Movement>* movements = new std::vector<State::Movement>(_distance);

	const State* node = this;
	int counter = _distance;

	while (--counter >= 0 && node)
	{
		(*movements)[counter] = node->_movement;
		node = node->_parent;
	}
	return (movements);
}

int State::get_distance() const
{
	return (_distance);
}

bool State::is_final() const {
	return _weight == 0;
}

Data&	State::get_data(void)
{
	return (this->_data);
}

const Finder&	State::get_finder(void) const
{
	return (this->_finder);
}

const Data&	State::get_data(void) const
{
	return (this->_data);
}

std::ostream& operator<< (std::ostream& s, const State::Movement c)
{
	bool reversed = c & State::Reversed;
	bool halfturn = c & State::Halfturn;

	switch (c & State::Mask) {
		case State::Left:  return s << (reversed ? "L'" : (halfturn ? "L2" : "L"));
		case State::Right: return s << (reversed ? "R'" : (halfturn ? "R2" : "R"));
		case State::Up:    return s << (reversed ? "U'" : (halfturn ? "U2" : "U"));
		case State::Down:  return s << (reversed ? "D'" : (halfturn ? "D2" : "D"));
		case State::Front: return s << (reversed ? "F'" : (halfturn ? "F2" : "F"));
		case State::Back:  return s << (reversed ? "B'" : (halfturn ? "B2" : "B"));
		default: return s << " ERROR:" << (int)c << ' ';
	}
}

void			State::set_parent(State* p)
{
	this->_parent = p;
}

State*			State::get_parent(void) const
{
	return (this->_parent);
}

void 			State::set_distance(int d)
{
	this->_distance = d;
}

Score 			State::get_weight(void) const
{
	return (this->_weight);
}

void 			State::set_weight(Score s)
{
	this->_weight = s;
}

State::Movement		State::get_movement() const {
	return (this->_movement);
}

size_t custom_hash::operator()(const State* x) const noexcept {
	const Data& data = x->get_data();
	size_t h = 13;

    for (int s = 0; s < 6; s++)
		for (int x = 0; x < size; x++)
			for (int y = 0; y < size; y++)
				if (x != 2 && y != 2)
					h = h * 31 + data[s][x][y].face_id;
	return (h);
}

bool custom_equal_to::operator()(const State* a, const State* b) const noexcept
{
	return (a->get_data() == b->get_data());
}

Score State::indexer_astar(const State& state)
{
	return state.get_weight() + state.get_distance();
}

Score State::indexer_greedy(const State& state)
{
	return state.get_weight();
}

Score State::indexer_uniform(const State& state)
{
	return state.get_distance();
}
