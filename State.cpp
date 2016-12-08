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
const UIDFinder		State::uid_finder = _calculate_uid_finder(solution);
const Finder		State::solution_finder = _calculate_finder(solution);

const Color State::solution_colors[] = {White, Green, Red, Blue, Orange, Yellow};


State::MovementNode::MovementNode(Movement _m, MovementRef& _p):value(_m),parent(_p) {}
State::MovementNode::MovementNode(Movement _m):value(_m) {}

State::State() {
	_data = new Data(solution);
	_weight = 0;
	_movement = MovementRef(new MovementNode(None));
	_distance = 0;
	stateCount++;
}

State::State(const State& clone) {
	*this = clone;
	_data = new Data(*_data);
}

State::State(const std::string& scramble):State(){
	apply_scramble(scramble);
	update();
}

void State::update() {
	_weight = Heuristics::HeuristicFunction(*_data);
	deflate();//TODO remove
}

string disp(int a, int b) {
	std::stringstream out;
	if (a == b)
		out << a--;
	else {
		out << "\e[45m" << a << "-" << b << "\e[0m";
	}
	return out.str();
}

State::State(State* parent, State::Movement m) {
	if (m == None)
		throw std::logic_error("None is not an allowed move, use copy constructor");
	if (parent->_weight < 0)
		throw std::logic_error("Attempting to create children from dead parent");

	_data = new Data(*parent->_data);
	_distance = parent->_distance + 1;
	_movement = MovementRef(new MovementNode(m, parent->_movement));

	apply_movement(m);
	update();

	if (parent->_id == _id)
		std::cerr << m;//TODO remove

	Data d1 = get_data();

	inflate();

	Data d2 = get_data();

	//if (d1 != d2) {
		std::cerr << "WHAT" << std::endl;
		for (int s = Index_S; s < Index_L; s++) {
			for (int l = 0; l < size; l++) {
				for (int c = 0; c < size; c++)
	            {
					Square as = d1[s][l][c];
					Square bs = d2[s][l][c];
					std::cout << "  {" << /*disp(as.cube_id, bs.cube_id) << " " <<*/ disp(as.rot_id, bs.rot_id) /*<< " " << disp(as.face_id, bs.face_id)*/ << "}  ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	//}

	stateCount++;
}

State::State(int scramble_count):State(){
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0,6);
	std::uniform_int_distribution<int> bo(0,2);

	Movement previous = None;
	for (int i = 0; i < scramble_count; i++) {
		Movement m = (Movement)uni(rng);
		while (m == previous)
			m = (Movement)uni(rng);
		int c = bo(rng);
		if (c == 1)
			m = (Movement)(m | Reversed);
		else if (c == 2)
			m = (Movement)(m | Halfturn);

		apply_movement(m);
	}
	update();
}

State::~State()
{
	delete _data;
	stateCount--;
}

void	State::get_candidates(std::vector<StateRef>& candidates)
{
	Movement m = (Movement)(_movement->value & Mask);

	for (int n = Movement_Start; n < Movement_End; n++) {

		if (n == m)
			continue;

		Movement nr = (Movement)(n | Reversed);
		Movement nh = (Movement)(n | Halfturn);

		candidates.push_back(StateRef(new State(this, (Movement)n)));
		candidates.push_back(StateRef(new State(this, nr)));
		candidates.push_back(StateRef(new State(this, nh)));
	}
}

constexpr Data		State::_calculate_solution() {
	Data			data = Data();

	/*for (int l = 0; l < size; l++)
		for (int c = 0; c < size; c++) {
			data[Index_U][l][c].color = White;
			data[Index_F][l][c].color = Green;
			data[Index_R][l][c].color = Red;
			data[Index_B][l][c].color = Blue;
			data[Index_L][l][c].color = Orange;
			data[Index_D][l][c].color = Yellow;
		}
	*/

    int uid = 0;
	for (int s = Index_S; s < Index_L; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
				data[s][l][c].face_id = uid++;

	//Adding corners ID
	//ULB
	//UFL
	//UBR
	//URF
	//DBL
	//DLF
	//DRB
	//DFR


	data[Index_U][0][0].cube_id = data[Index_L][0][0].cube_id = data[Index_B][0][2].cube_id = 0;
	data[Index_U][2][0].cube_id = data[Index_L][0][2].cube_id = data[Index_F][0][0].cube_id = 1;
	data[Index_U][0][2].cube_id = data[Index_R][0][2].cube_id = data[Index_B][0][0].cube_id = 2;
	data[Index_U][2][2].cube_id = data[Index_R][0][0].cube_id = data[Index_F][0][2].cube_id = 3;
	data[Index_D][0][0].cube_id = data[Index_L][2][2].cube_id = data[Index_F][2][0].cube_id = 4;
	data[Index_D][2][0].cube_id = data[Index_L][2][0].cube_id = data[Index_B][2][2].cube_id = 5;
	data[Index_D][0][2].cube_id = data[Index_R][2][0].cube_id = data[Index_F][2][2].cube_id = 6;
	data[Index_D][2][2].cube_id = data[Index_R][2][2].cube_id = data[Index_B][2][0].cube_id = 7;

	data[Index_U][0][0].rot_id = 0; data[Index_L][0][0].rot_id = 1; data[Index_B][0][2].rot_id = 2;
	data[Index_U][2][0].rot_id = 0; data[Index_L][0][2].rot_id = 1; data[Index_F][0][0].rot_id = 2;
	data[Index_U][0][2].rot_id = 0; data[Index_R][0][2].rot_id = 1; data[Index_B][0][0].rot_id = 2;
	data[Index_U][2][2].rot_id = 0; data[Index_R][0][0].rot_id = 1; data[Index_F][0][2].rot_id = 2;
	data[Index_D][0][0].rot_id = 0; data[Index_L][2][2].rot_id = 1; data[Index_F][2][0].rot_id = 2;
	data[Index_D][2][0].rot_id = 0; data[Index_L][2][0].rot_id = 1; data[Index_B][2][2].rot_id = 2;
	data[Index_D][0][2].rot_id = 0; data[Index_R][2][0].rot_id = 1; data[Index_F][2][2].rot_id = 2;
	data[Index_D][2][2].rot_id = 0; data[Index_R][2][2].rot_id = 1; data[Index_B][2][0].rot_id = 2;

	//Adding borders ID
	data[Index_U][0][1].cube_id = data[Index_B][0][1].cube_id = 0;
	data[Index_U][1][0].cube_id = data[Index_L][0][1].cube_id = 1;
	data[Index_U][1][2].cube_id = data[Index_R][0][1].cube_id = 2;
	data[Index_U][2][1].cube_id = data[Index_F][0][1].cube_id = 3;

	data[Index_D][0][1].cube_id = data[Index_F][2][1].cube_id = 4;
	data[Index_D][1][0].cube_id = data[Index_L][2][1].cube_id = 5;
	data[Index_D][1][2].cube_id = data[Index_R][2][1].cube_id = 6;
	data[Index_D][2][1].cube_id = data[Index_B][2][1].cube_id = 7;

	data[Index_F][1][2].cube_id = data[Index_R][1][0].cube_id = 8;
	data[Index_R][1][2].cube_id = data[Index_B][1][0].cube_id = 9;
	data[Index_B][1][2].cube_id = data[Index_L][1][0].cube_id = 10;
	data[Index_L][1][2].cube_id = data[Index_F][1][0].cube_id = 11;

	data[Index_U][0][1].rot_id = 0; data[Index_B][0][1].rot_id = 1;
	data[Index_U][1][0].rot_id = 0; data[Index_L][0][1].rot_id = 1;
	data[Index_U][1][2].rot_id = 0; data[Index_R][0][1].rot_id = 1;
	data[Index_U][2][1].rot_id = 0; data[Index_F][0][1].rot_id = 1;

	data[Index_D][0][1].rot_id = 0; data[Index_F][2][1].rot_id = 1;
	data[Index_D][1][0].rot_id = 0; data[Index_L][2][1].rot_id = 1;
	data[Index_D][1][2].rot_id = 0; data[Index_R][2][1].rot_id = 1;
	data[Index_D][2][1].rot_id = 0; data[Index_B][2][1].rot_id = 1;

	data[Index_F][1][2].rot_id = 0; data[Index_R][1][0].rot_id = 1;
	data[Index_R][1][2].rot_id = 0; data[Index_B][1][0].rot_id = 1;
	data[Index_B][1][2].rot_id = 0; data[Index_L][1][0].rot_id = 1;
	data[Index_L][1][2].rot_id = 0; data[Index_F][1][0].rot_id = 1;

	//Adding center ID
	data[Index_U][1][1].cube_id = 0;
	data[Index_D][1][1].cube_id = 1;
	data[Index_B][1][1].cube_id = 2;
	data[Index_L][1][1].cube_id = 3;
	data[Index_F][1][1].cube_id = 4;
	data[Index_R][1][1].cube_id = 5;
	data[Index_U][1][1].rot_id = 0;
	data[Index_D][1][1].rot_id = 0;
	data[Index_B][1][1].rot_id = 0;
	data[Index_L][1][1].rot_id = 0;
	data[Index_F][1][1].rot_id = 0;
	data[Index_R][1][1].rot_id = 0;

	return data;
}

constexpr UIDFinder		State::_calculate_uid_finder(const Data& data) {

    UIDFinder finder = UIDFinder();

    for (int s = Index_S; s < Index_L; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++) {
                Square sq = data[s][l][c];
                if (l == 1 && c == 1)
                    finder.centers.at(sq.cube_id) = sq.face_id;
                else if (l == 1 || c == 1)
                    finder.borders.at(sq.cube_id).at(sq.rot_id) = sq.face_id;
                else
                    finder.corners.at(sq.cube_id).at(sq.rot_id) = sq.face_id;
			}
    return finder;
}

constexpr Finder	State::_calculate_finder(const Data &data) {
	Finder			finder = Finder();

	for (int s = Index_S; s < Index_L; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++) {
				Coord co = (Coord){s, l, c};
				int id = data[s][l][c].face_id;
				finder[id] = co;
			}
	return finder;
}

void State::apply_scramble(const string& scramble) {

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
		apply_movement(m);
	}
}



void swap_s(Square& a, Square& b, Square& c, Square& d, int turns) {
	Square t;
	switch (turns) {
		case 1:

			t = d;
			d = c;
			c = b;
			b = a;
			a = t;
			break;
		case 2:
			std::swap(a, c);
			std::swap(b, d);
			break;
		case 3:
			t = a;
			a = b;
			b = c;
			c = d;
			d = t;
			break;
	}
}

void rotate_face(Face& face, int turns) {

	swap_s(face[0][2], face[2][2], face[2][0], face[0][0], turns);
	swap_s(face[1][2], face[2][1], face[1][0], face[0][1], turns);
}

void State::apply_movement(Movement m) {

	bool reversed = m & Reversed;
	bool halfturn = m & Halfturn;
	int turns = reversed ? 3 : (halfturn ? 2 : 1);
	Data& data = *_data;
	switch (m & Mask) {
		case None : return;
		case Up :
			rotate_face(data[Index_U], turns);
			 //Crown is front, left, back, right
			swap_s(data[Index_F][0][0], data[Index_L][0][0], data[Index_B][0][0], data[Index_R][0][0], turns);
			swap_s(data[Index_F][0][1], data[Index_L][0][1], data[Index_B][0][1], data[Index_R][0][1], turns);
			swap_s(data[Index_F][0][2], data[Index_L][0][2], data[Index_B][0][2], data[Index_R][0][2], turns);
			break;
		case Front :
			rotate_face(data[Index_F], turns);
			 //Crown is up, right, down, left
			swap_s(data[Index_U][2][0], data[Index_R][0][0], data[Index_D][0][2], data[Index_L][2][2], turns);
			swap_s(data[Index_U][2][1], data[Index_R][1][0], data[Index_D][0][1], data[Index_L][1][2], turns);
			swap_s(data[Index_U][2][2], data[Index_R][2][0], data[Index_D][0][0], data[Index_L][0][2], turns);
			break;
		case Right :
			rotate_face(data[Index_R], turns);
			 //Crown is up, back, down, front
			swap_s(data[Index_U][2][2], data[Index_B][0][0], data[Index_D][2][2], data[Index_F][2][2], turns);
			swap_s(data[Index_U][1][2], data[Index_B][1][0], data[Index_D][1][2], data[Index_F][1][2], turns);
			swap_s(data[Index_U][0][2], data[Index_B][2][0], data[Index_D][0][2], data[Index_F][0][2], turns);
			break;
		case Back :
			rotate_face(data[Index_B], turns);
			 //Crown is up, left, down, right
			swap_s(data[Index_U][0][0], data[Index_L][2][0], data[Index_D][2][2], data[Index_R][0][2], turns);
			swap_s(data[Index_U][0][1], data[Index_L][1][0], data[Index_D][2][1], data[Index_R][1][2], turns);
			swap_s(data[Index_U][0][2], data[Index_L][0][0], data[Index_D][2][0], data[Index_R][2][2], turns);
			break;
		case Left :
			rotate_face(data[Index_L], turns);
			 //Crown is up, front, down, back
			swap_s(data[Index_U][0][0], data[Index_F][0][0], data[Index_D][0][0], data[Index_B][2][2], turns);
			swap_s(data[Index_U][1][0], data[Index_F][1][0], data[Index_D][1][0], data[Index_B][1][2], turns);
			swap_s(data[Index_U][2][0], data[Index_F][2][0], data[Index_D][2][0], data[Index_B][0][2], turns);
			break;
		case Down :
			rotate_face(data[Index_D], turns);
 			 //Crown is front, right, back, left
			swap_s(data[Index_F][2][0], data[Index_R][2][0], data[Index_B][2][0], data[Index_L][2][0], turns);
			swap_s(data[Index_F][2][1], data[Index_R][2][1], data[Index_B][2][1], data[Index_L][2][1], turns);
			swap_s(data[Index_F][2][2], data[Index_R][2][2], data[Index_B][2][2], data[Index_L][2][2], turns);
			break;
		default: break;
	}
}

/*
bool State::check_continuity() const{
	int id;

	id = _data[Index_U][0][0].cube_id;
	if (id != _data[Index_L][0][0].cube_id || id != _data[Index_B][0][2].cube_id)
		return false;
	id = _data[Index_U][2][0].cube_id;
	if (id != _data[Index_L][0][2].cube_id || id != _data[Index_F][0][0].cube_id)
		return false;
	id = _data[Index_U][0][2].cube_id;
	if (id != _data[Index_R][0][2].cube_id|| id != _data[Index_B][0][0].cube_id)
		return false;
	id = _data[Index_U][2][2].cube_id;
	if (id != _data[Index_R][0][0].cube_id|| id != _data[Index_F][0][2].cube_id)
		return false;

	id = _data[Index_D][0][0].cube_id;
	if (id != _data[Index_L][2][2].cube_id || id != _data[Index_F][2][0].cube_id)
		return false;
	id = _data[Index_D][2][0].cube_id;
	if (id != _data[Index_L][2][0].cube_id || id != _data[Index_B][2][2].cube_id)
		return false;
	id = _data[Index_D][0][2].cube_id;
	if (id != _data[Index_R][2][0].cube_id|| id != _data[Index_F][2][2].cube_id)
		return false;
	id = _data[Index_D][2][2].cube_id;
	if (id != _data[Index_R][2][2].cube_id|| id != _data[Index_B][2][0].cube_id)
		return false;

	//Adding borders ID
	if (_data[Index_U][0][1].cube_id != _data[Index_B][0][1].cube_id)
		return false;
	if (_data[Index_U][1][0].cube_id != _data[Index_L][0][1].cube_id)
		return false;
	if (_data[Index_U][2][1].cube_id != _data[Index_F][0][1].cube_id)
		return false;
	if (_data[Index_U][1][2].cube_id != _data[Index_R][0][1].cube_id)
		return false;

	if (_data[Index_D][0][1].cube_id != _data[Index_F][2][1].cube_id)
		return false;
	if (_data[Index_D][1][0].cube_id != _data[Index_L][2][1].cube_id)
		return false;
	if (_data[Index_D][2][1].cube_id != _data[Index_B][2][1].cube_id)
		return false;
	if (_data[Index_D][1][2].cube_id != _data[Index_R][2][1].cube_id)
		return false;

	if (_data[Index_F][1][2].cube_id != _data[Index_R][1][0].cube_id)
		return false;
	if (_data[Index_R][1][2].cube_id != _data[Index_B][1][0].cube_id)
		return false;
	if (_data[Index_B][1][2].cube_id != _data[Index_L][1][0].cube_id)
		return false;
	if (_data[Index_L][1][2].cube_id != _data[Index_F][1][0].cube_id)
		return false;
	return true;
}*/

std::vector<State::Movement> State::get_movements() const {

	std::vector<Movement> movements;

	MovementNode* m = _movement.get();

	while (m->value != None) {
		movements.insert(movements.begin(), m->value);
		m = m->parent.get();
	}
	return (movements);
}

Score State::get_distance() const
{
	return (_distance);
}

bool State::is_final() const {
	return _weight == 0;
}

bool State::is_alive() const {
	return _weight >= 0;
}

void State::kill() {
	if (_weight < 0)
		throw std::logic_error("Node already dead");
	_weight = -1;
}

const Data&	State::get_data(void) const
{
	return (*this->_data);
}

Data&	State::get_data(void)
{
	return (*this->_data);
}

const ID&	State::get_id(void) const
{
	return (this->_id);
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

Score 			State::get_weight(void) const
{
	return (this->_weight);
}

size_t custom_hash::operator()(const StateRef& l) const noexcept {
	//const Data& data = l->get_data();
	//size_t h = 13;
	const ID& id = l->get_id();

	/*//TODO skip useless faces
	//std::cerr << "h";
    for (int s = 0; s < 6; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
				//if (l != 1 || c != 1)
					h = h * 31 + data[s][l][c].face_id;
	//std::clog << "h" << h << std::endl;
	*/
	size_t h = (id.borders_rot ^ id.borders_pos) | ((size_t)id.corners << 32);
	return (h);
}

bool custom_equal_to::operator()(const StateRef& a, const StateRef& b) const noexcept
{
	if (a->get_weight() != b->get_weight() )
		return (false);
	return (a->get_id() == b->get_id());
}

Score State::indexer_astar(const State& state)
{
	return state.get_weight() + state.get_distance() * score_multiplier;
}

Score State::indexer_greedy(const State& state)
{
	return state.get_weight();
}

Score State::indexer_uniform(const State& state)
{
	return state.get_distance();
}
