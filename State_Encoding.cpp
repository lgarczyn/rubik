
#include "State.hpp"
#include <sstream>

constexpr Data		State::_calculate_solution() {
	Data			data = Data();

    int uid = 0;
	for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
				data[s][l][c].face_id = uid++;

	//Adding corners ID
	data[Index_U][0][0].cube_id = data[Index_L][0][0].cube_id = data[Index_B][0][2].cube_id = 0;
	data[Index_U][0][2].cube_id = data[Index_B][0][0].cube_id = data[Index_R][0][2].cube_id = 1;
	data[Index_U][2][0].cube_id = data[Index_F][0][0].cube_id = data[Index_L][0][2].cube_id = 2;
	data[Index_U][2][2].cube_id = data[Index_R][0][0].cube_id = data[Index_F][0][2].cube_id = 3;
	data[Index_D][0][0].cube_id = data[Index_L][2][2].cube_id = data[Index_F][2][0].cube_id = 4;
	data[Index_D][0][2].cube_id = data[Index_F][2][2].cube_id = data[Index_R][2][0].cube_id = 5;
	data[Index_D][2][0].cube_id = data[Index_B][2][2].cube_id = data[Index_L][2][0].cube_id = 6;
	data[Index_D][2][2].cube_id = data[Index_R][2][2].cube_id = data[Index_B][2][0].cube_id = 7;

	data[Index_U][0][0].rot_id = 0; data[Index_L][0][0].rot_id = 1; data[Index_B][0][2].rot_id = 2;
	data[Index_U][0][2].rot_id = 0; data[Index_B][0][0].rot_id = 1; data[Index_R][0][2].rot_id = 2;
	data[Index_U][2][0].rot_id = 0; data[Index_F][0][0].rot_id = 1; data[Index_L][0][2].rot_id = 2;
	data[Index_U][2][2].rot_id = 0; data[Index_R][0][0].rot_id = 1; data[Index_F][0][2].rot_id = 2;
	data[Index_D][0][0].rot_id = 0; data[Index_L][2][2].rot_id = 1; data[Index_F][2][0].rot_id = 2;
	data[Index_D][0][2].rot_id = 0; data[Index_F][2][2].rot_id = 1; data[Index_R][2][0].rot_id = 2;
	data[Index_D][2][0].rot_id = 0; data[Index_B][2][2].rot_id = 1; data[Index_L][2][0].rot_id = 2;
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

	data[Index_R][1][0].cube_id = data[Index_F][1][2].cube_id = 8;
	data[Index_B][1][0].cube_id = data[Index_R][1][2].cube_id = 9;
	data[Index_L][1][0].cube_id = data[Index_B][1][2].cube_id = 10;
	data[Index_F][1][0].cube_id = data[Index_L][1][2].cube_id = 11;

	data[Index_U][0][1].rot_id = 0; data[Index_B][0][1].rot_id = 1;
	data[Index_U][1][0].rot_id = 0; data[Index_L][0][1].rot_id = 1;
	data[Index_U][1][2].rot_id = 0; data[Index_R][0][1].rot_id = 1;
	data[Index_U][2][1].rot_id = 0; data[Index_F][0][1].rot_id = 1;

	data[Index_D][0][1].rot_id = 0; data[Index_F][2][1].rot_id = 1;
	data[Index_D][1][0].rot_id = 0; data[Index_L][2][1].rot_id = 1;
	data[Index_D][1][2].rot_id = 0; data[Index_R][2][1].rot_id = 1;
	data[Index_D][2][1].rot_id = 0; data[Index_B][2][1].rot_id = 1;

	data[Index_R][1][0].rot_id = 0; data[Index_F][1][2].rot_id = 1;
	data[Index_B][1][0].rot_id = 0; data[Index_R][1][2].rot_id = 1;
	data[Index_L][1][0].rot_id = 0; data[Index_B][1][2].rot_id = 1;
	data[Index_F][1][0].rot_id = 0; data[Index_L][1][2].rot_id = 1;

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

    for (int s = Index_Start; s < Index_End; s++)
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

	for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++) {
				Coord co = (Coord){s, l, c};
				int id = data[s][l][c].face_id;
				finder[id] = co;
			}
	return finder;
}

void State::_apply_scramble(Data& data, const string& scramble) {

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
		_apply_movement(data, m);
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

void State::_apply_movement(Data& data, Movement m) {

	bool reversed = m & Reversed;
	bool halfturn = m & Halfturn;
	int turns = reversed ? 3 : (halfturn ? 2 : 1);
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

constexpr int fact(int i) {
    int r = 1;
    for (int it = 1; it <= i; it++) {
        r *= it;
    }
    return r;
}

constexpr int pow(int a, int b) {
    int r = 1;
    for (int it = 0; it < b; it++) {
        r *= a;
    }
    return r;
}

constexpr inline void move_values_down(uchar* values, uchar pos, int len) {
	for (uchar i = pos + 1; i < len; i++) {
		values[i]--;
	}
}

constexpr inline uint get_fact_value(uchar cube_id, uchar *values, int len) {
	uchar r = values[cube_id];
	move_values_down(values, cube_id, len);
    return r;
}

constexpr uint get_id_corners_pos(const Data& data) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint s = 0;

    s = get_fact_value(data[Index_U][0][0].cube_id, values, 8);
    s = get_fact_value(data[Index_U][0][2].cube_id, values, 8) + s * 7;
    s = get_fact_value(data[Index_U][2][0].cube_id, values, 8) + s * 6;
    s = get_fact_value(data[Index_U][2][2].cube_id, values, 8) + s * 5;
    s = get_fact_value(data[Index_D][0][0].cube_id, values, 8) + s * 4;
    s = get_fact_value(data[Index_D][0][2].cube_id, values, 8) + s * 3;
    s = get_fact_value(data[Index_D][2][0].cube_id, values, 8) + s * 2;
  //s = get_fact_value(data[Index_D][2][2].cube_id, values, 8) + s * 1;
	return s;
}

constexpr uint get_id_borders_pos(const Data& data) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	uint s = 0;

	s = get_fact_value(data[Index_U][0][1].cube_id, values, 12);
	s = get_fact_value(data[Index_U][1][0].cube_id, values, 12) + s * 11;
	s = get_fact_value(data[Index_U][1][2].cube_id, values, 12) + s * 10;
	s = get_fact_value(data[Index_U][2][1].cube_id, values, 12) + s * 9;

	s = get_fact_value(data[Index_D][0][1].cube_id, values, 12) + s * 8;
	s = get_fact_value(data[Index_D][1][0].cube_id, values, 12) + s * 7;
	s = get_fact_value(data[Index_D][1][2].cube_id, values, 12) + s * 6;
	s = get_fact_value(data[Index_D][2][1].cube_id, values, 12) + s * 5;

	s = get_fact_value(data[Index_F][1][2].cube_id, values, 12) + s * 4;
	s = get_fact_value(data[Index_R][1][2].cube_id, values, 12) + s * 3;
	s = get_fact_value(data[Index_B][1][2].cube_id, values, 12) + s * 2;
	//s = get_fact_value(data[Index_L][1][2].cube_id, values, 12) + s * 1;
	return s;
}

constexpr uint get_id_corners_rot(const Data& data) {
	uint s = 0;

    s = data[Index_U][0][0].rot_id;
	s = s * 3 + data[Index_U][0][2].rot_id;
	s = s * 3 + data[Index_U][2][0].rot_id;
	s = s * 3 + data[Index_U][2][2].rot_id;
	s = s * 3 + data[Index_D][0][0].rot_id;
	s = s * 3 + data[Index_D][0][2].rot_id;
	s = s * 3 + data[Index_D][2][0].rot_id;
	s = s * 3 + data[Index_D][2][2].rot_id;
	return s;
}

constexpr uint get_id_borders_rot(const Data& data) {
	uint s = 0;

	s = data[Index_U][0][1].rot_id;
	s = s * 2 + data[Index_U][1][0].rot_id;
	s = s * 2 + data[Index_U][2][1].rot_id;
	s = s * 2 + data[Index_U][1][2].rot_id;

	s = s * 2 + data[Index_F][1][0].rot_id;
	s = s * 2 + data[Index_R][1][0].rot_id;
	s = s * 2 + data[Index_B][1][0].rot_id;
	s = s * 2 + data[Index_L][1][0].rot_id;

	s = s * 2 + data[Index_D][0][1].rot_id;
	s = s * 2 + data[Index_D][1][0].rot_id;
	s = s * 2 + data[Index_D][2][1].rot_id;
	s = s * 2 + data[Index_D][1][2].rot_id;

	return s;
}

constexpr ID State::id_from_data(const Data data){
    ID id = ID();
	id.corners = get_id_corners_pos(data) * pow(3, 8) + get_id_corners_rot(data);
	id.borders_pos = get_id_borders_pos(data);
	id.borders_rot = get_id_borders_rot(data);
    return id;
}

constexpr inline void move_values_left(uchar* values, uchar pos, int len) {

	for (uchar i = pos; i < len - 1; i++) {
		values[i] = values[i + 1];
	}
}

constexpr inline uchar get_value_fact(uint s, uchar* values, uchar pos, uchar len) {
    if (pos == 1)
        return (values[0]);
    uchar c = (s / fact(pos - 1)) % pos;
	uchar r = values[c];
	move_values_left(values, c, len);
    return r;
}

constexpr inline void set_corner_rot(uchar& up, uchar& fb, uchar& rl, uchar i) {
    if (i == 0) {
        up = 0;
        fb = 1;
        rl = 2;
    } else if (i == 1) {
        up = 1;
        fb = 2;
        rl = 0;
    } else {
        up = 2;
        fb = 0;
        rl = 1;
    }
}

constexpr void set_data_corners_rot(Data& data, uint id) {
    set_corner_rot(data[Index_D][2][2].rot_id, data[Index_R][2][2].rot_id, data[Index_B][2][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_D][2][0].rot_id, data[Index_B][2][2].rot_id, data[Index_L][2][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_D][0][2].rot_id, data[Index_F][2][2].rot_id, data[Index_R][2][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_D][0][0].rot_id, data[Index_L][2][2].rot_id, data[Index_F][2][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_U][2][2].rot_id, data[Index_R][0][0].rot_id, data[Index_F][0][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_U][2][0].rot_id, data[Index_F][0][0].rot_id, data[Index_L][0][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_U][0][2].rot_id, data[Index_B][0][0].rot_id, data[Index_R][0][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_U][0][0].rot_id, data[Index_L][0][0].rot_id, data[Index_B][0][2].rot_id, id % 3); id /= 3;//TODO remove /% 3
}

constexpr void set_data_borders_rot(Data& data, uint id) {
	uint s = id;

	data[Index_R][2][1].rot_id = !((data[Index_D][1][2].rot_id = s % 2)); s /= 2;
    data[Index_B][2][1].rot_id = !((data[Index_D][2][1].rot_id = s % 2)); s /= 2;
    data[Index_L][2][1].rot_id = !((data[Index_D][1][0].rot_id = s % 2)); s /= 2;
    data[Index_F][2][1].rot_id = !((data[Index_D][0][1].rot_id = s % 2)); s /= 2;

    data[Index_B][1][2].rot_id = !((data[Index_L][1][0].rot_id = s % 2)); s /= 2;
    data[Index_R][1][2].rot_id = !((data[Index_B][1][0].rot_id = s % 2)); s /= 2;
    data[Index_F][1][2].rot_id = !((data[Index_R][1][0].rot_id = s % 2)); s /= 2;
    data[Index_L][1][2].rot_id = !((data[Index_F][1][0].rot_id = s % 2)); s /= 2;

    data[Index_R][0][1].rot_id = !((data[Index_U][1][2].rot_id = s % 2)); s /= 2;
    data[Index_F][0][1].rot_id = !((data[Index_U][2][1].rot_id = s % 2)); s /= 2;
    data[Index_L][0][1].rot_id = !((data[Index_U][1][0].rot_id = s % 2)); s /= 2;
    data[Index_B][0][1].rot_id = !((data[Index_U][0][1].rot_id = s)); //s % 2)); s /= 2;
}

constexpr void set_data_corners_pos(Data& data, uint id) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    data[Index_U][0][0].cube_id = data[Index_L][0][0].cube_id = data[Index_B][0][2].cube_id = get_value_fact(id, values, 8, 8);
    data[Index_U][0][2].cube_id = data[Index_R][0][2].cube_id = data[Index_B][0][0].cube_id = get_value_fact(id, values, 7, 8);
    data[Index_U][2][0].cube_id = data[Index_L][0][2].cube_id = data[Index_F][0][0].cube_id = get_value_fact(id, values, 6, 8);
    data[Index_U][2][2].cube_id = data[Index_R][0][0].cube_id = data[Index_F][0][2].cube_id = get_value_fact(id, values, 5, 8);
    data[Index_D][0][0].cube_id = data[Index_L][2][2].cube_id = data[Index_F][2][0].cube_id = get_value_fact(id, values, 4, 8);
    data[Index_D][0][2].cube_id = data[Index_R][2][0].cube_id = data[Index_F][2][2].cube_id = get_value_fact(id, values, 3, 8);
    data[Index_D][2][0].cube_id = data[Index_L][2][0].cube_id = data[Index_B][2][2].cube_id = get_value_fact(id, values, 2, 8);
    data[Index_D][2][2].cube_id = data[Index_R][2][2].cube_id = data[Index_B][2][0].cube_id = get_value_fact(id, values, 1, 8);
}

constexpr void set_data_center(Data& data) {
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
}

constexpr void set_data_borders_pos(Data& data, uint id) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    data[Index_U][0][1].cube_id = data[Index_B][0][1].cube_id = get_value_fact(id, values, 12, 12);
    data[Index_U][1][0].cube_id = data[Index_L][0][1].cube_id = get_value_fact(id, values, 11, 12);
    data[Index_U][1][2].cube_id = data[Index_R][0][1].cube_id = get_value_fact(id, values, 10, 12);
    data[Index_U][2][1].cube_id = data[Index_F][0][1].cube_id = get_value_fact(id, values, 9, 12);

    data[Index_D][0][1].cube_id = data[Index_F][2][1].cube_id = get_value_fact(id, values, 8, 12);
    data[Index_D][1][0].cube_id = data[Index_L][2][1].cube_id = get_value_fact(id, values, 7, 12);
    data[Index_D][1][2].cube_id = data[Index_R][2][1].cube_id = get_value_fact(id, values, 6, 12);
    data[Index_D][2][1].cube_id = data[Index_B][2][1].cube_id = get_value_fact(id, values, 5, 12);

    data[Index_F][1][2].cube_id = data[Index_R][1][0].cube_id = get_value_fact(id, values, 4, 12);
    data[Index_R][1][2].cube_id = data[Index_B][1][0].cube_id = get_value_fact(id, values, 3, 12);
    data[Index_B][1][2].cube_id = data[Index_L][1][0].cube_id = get_value_fact(id, values, 2, 12);
    data[Index_L][1][2].cube_id = data[Index_F][1][0].cube_id = get_value_fact(id, values, 1, 12);
}

constexpr void get_uids(Data& data) {
    for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++){
                Square &sq = data[s][l][c];
                if (l == 1 && c == 1)
                    sq.face_id = State::uid_finder.centers[sq.cube_id];
                else if (l == 1 || c == 1)
                    sq.face_id = State::uid_finder.borders[sq.cube_id][sq.rot_id];
                else
                    sq.face_id = State::uid_finder.corners[sq.cube_id][sq.rot_id];
            }
}

constexpr Data State::data_from_id(const ID id) {
    Data data = Data();
    set_data_corners_pos(data, id.corners / pow(3, 8));
    set_data_corners_rot(data, id.corners % pow(3, 8));
    set_data_borders_pos(data, id.borders_pos);
    set_data_borders_rot(data, id.borders_rot);
    set_data_center(data);
    get_uids(data);
    return data;
}

Data State::to_data() const {
	return data_from_id(_id);
}
