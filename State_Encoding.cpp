
#include "State.hpp"
#include <sstream>

constexpr Cube		State::_calculate_solution() {
	Cube			cube = Cube();

    int uid = 0;
	for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
				uid++;

	//Adding corners ID
	cube[Index_U][0][0].cube_id = cube[Index_L][0][0].cube_id = cube[Index_B][0][2].cube_id = 0;
	cube[Index_U][0][2].cube_id = cube[Index_B][0][0].cube_id = cube[Index_R][0][2].cube_id = 1;
	cube[Index_U][2][0].cube_id = cube[Index_F][0][0].cube_id = cube[Index_L][0][2].cube_id = 2;
	cube[Index_U][2][2].cube_id = cube[Index_R][0][0].cube_id = cube[Index_F][0][2].cube_id = 3;
	cube[Index_D][0][0].cube_id = cube[Index_L][2][2].cube_id = cube[Index_F][2][0].cube_id = 4;
	cube[Index_D][0][2].cube_id = cube[Index_F][2][2].cube_id = cube[Index_R][2][0].cube_id = 5;
	cube[Index_D][2][0].cube_id = cube[Index_B][2][2].cube_id = cube[Index_L][2][0].cube_id = 6;
	cube[Index_D][2][2].cube_id = cube[Index_R][2][2].cube_id = cube[Index_B][2][0].cube_id = 7;

	cube[Index_U][0][0].rot_id = 0; cube[Index_L][0][0].rot_id = 1; cube[Index_B][0][2].rot_id = 2;
	cube[Index_U][0][2].rot_id = 0; cube[Index_B][0][0].rot_id = 1; cube[Index_R][0][2].rot_id = 2;
	cube[Index_U][2][0].rot_id = 0; cube[Index_F][0][0].rot_id = 1; cube[Index_L][0][2].rot_id = 2;
	cube[Index_U][2][2].rot_id = 0; cube[Index_R][0][0].rot_id = 1; cube[Index_F][0][2].rot_id = 2;
	cube[Index_D][0][0].rot_id = 0; cube[Index_L][2][2].rot_id = 1; cube[Index_F][2][0].rot_id = 2;
	cube[Index_D][0][2].rot_id = 0; cube[Index_F][2][2].rot_id = 1; cube[Index_R][2][0].rot_id = 2;
	cube[Index_D][2][0].rot_id = 0; cube[Index_B][2][2].rot_id = 1; cube[Index_L][2][0].rot_id = 2;
	cube[Index_D][2][2].rot_id = 0; cube[Index_R][2][2].rot_id = 1; cube[Index_B][2][0].rot_id = 2;

	//Adding borders ID
	cube[Index_U][0][1].cube_id = cube[Index_B][0][1].cube_id = 0;
	cube[Index_U][1][0].cube_id = cube[Index_L][0][1].cube_id = 1;
	cube[Index_U][1][2].cube_id = cube[Index_R][0][1].cube_id = 2;
	cube[Index_U][2][1].cube_id = cube[Index_F][0][1].cube_id = 3;

	cube[Index_R][1][0].cube_id = cube[Index_F][1][2].cube_id = 4;
	cube[Index_B][1][0].cube_id = cube[Index_R][1][2].cube_id = 5;
	cube[Index_L][1][0].cube_id = cube[Index_B][1][2].cube_id = 6;
	cube[Index_F][1][0].cube_id = cube[Index_L][1][2].cube_id = 7;

	cube[Index_D][0][1].cube_id = cube[Index_F][2][1].cube_id = 8;
	cube[Index_D][1][0].cube_id = cube[Index_L][2][1].cube_id = 9;
	cube[Index_D][1][2].cube_id = cube[Index_R][2][1].cube_id = 10;
	cube[Index_D][2][1].cube_id = cube[Index_B][2][1].cube_id = 11;

	cube[Index_U][0][1].rot_id = 0; cube[Index_B][0][1].rot_id = 1;
	cube[Index_U][1][0].rot_id = 0; cube[Index_L][0][1].rot_id = 1;
	cube[Index_U][1][2].rot_id = 0; cube[Index_R][0][1].rot_id = 1;
	cube[Index_U][2][1].rot_id = 0; cube[Index_F][0][1].rot_id = 1;

	cube[Index_D][0][1].rot_id = 0; cube[Index_F][2][1].rot_id = 1;
	cube[Index_D][1][0].rot_id = 0; cube[Index_L][2][1].rot_id = 1;
	cube[Index_D][1][2].rot_id = 0; cube[Index_R][2][1].rot_id = 1;
	cube[Index_D][2][1].rot_id = 0; cube[Index_B][2][1].rot_id = 1;

	cube[Index_R][1][0].rot_id = 0; cube[Index_F][1][2].rot_id = 1;
	cube[Index_B][1][0].rot_id = 0; cube[Index_R][1][2].rot_id = 1;
	cube[Index_L][1][0].rot_id = 0; cube[Index_B][1][2].rot_id = 1;
	cube[Index_F][1][0].rot_id = 0; cube[Index_L][1][2].rot_id = 1;

	//Adding center ID
	cube[Index_U][1][1].cube_id = 0;
	cube[Index_F][1][1].cube_id = 1;
	cube[Index_R][1][1].cube_id = 2;
	cube[Index_B][1][1].cube_id = 3;
	cube[Index_L][1][1].cube_id = 4;
	cube[Index_D][1][1].cube_id = 5;

	cube[Index_U][1][1].rot_id = 0;
	cube[Index_F][1][1].rot_id = 0;
	cube[Index_R][1][1].rot_id = 0;
	cube[Index_B][1][1].rot_id = 0;
	cube[Index_L][1][1].rot_id = 0;
	cube[Index_D][1][1].rot_id = 0;

	return cube;
}

constexpr Finder	State::_calculate_finder(const Cube &cube) {
	Finder			finder = Finder();

	for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++) {
				Coord co = (Coord){s, l, c};
				int id = cube[s][l][c].get_uid(l, c);//TODO replace with smart
				finder[id] = co;
			}
	return finder;
}

void State::_apply_scramble(Data& data, const string& scramble) {

	std::stringstream ss = std::stringstream(scramble);

	while (ss) {
		Movement m;
		int turns = 1;
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
			case '\'': turns = 3; break;
			case '2': turns  = 2; break;
		}
		_apply_movement(data, m, turns);
	}
}

/*

void State::_apply_scramble(Cube& cube, const string& scramble) {

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
		_apply_movement(cube, m);
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

void State::_apply_movement(Cube& cube, Movement m) {

	std::cerr << "applying movements " << m << std::endl;
	bool reversed = m & Reversed;
	bool halfturn = m & Halfturn;
	int turns = reversed ? 3 : (halfturn ? 2 : 1);
	switch (m & Mask) {
		case None : return;
		case Up :
			rotate_face(cube[Index_U], turns);
			 //Crown is front, left, back, right
			swap_s(cube[Index_F][0][0], cube[Index_L][0][0], cube[Index_B][0][0], cube[Index_R][0][0], turns);
			swap_s(cube[Index_F][0][1], cube[Index_L][0][1], cube[Index_B][0][1], cube[Index_R][0][1], turns);
			swap_s(cube[Index_F][0][2], cube[Index_L][0][2], cube[Index_B][0][2], cube[Index_R][0][2], turns);
			break;
		case Front :
			rotate_face(cube[Index_F], turns);
			 //Crown is up, right, down, left
			swap_s(cube[Index_U][2][0], cube[Index_R][0][0], cube[Index_D][0][2], cube[Index_L][2][2], turns);
			swap_s(cube[Index_U][2][1], cube[Index_R][1][0], cube[Index_D][0][1], cube[Index_L][1][2], turns);
			swap_s(cube[Index_U][2][2], cube[Index_R][2][0], cube[Index_D][0][0], cube[Index_L][0][2], turns);
			break;
		case Right :
			rotate_face(cube[Index_R], turns);
			 //Crown is up, back, down, front
			swap_s(cube[Index_U][2][2], cube[Index_B][0][0], cube[Index_D][2][2], cube[Index_F][2][2], turns);
			swap_s(cube[Index_U][1][2], cube[Index_B][1][0], cube[Index_D][1][2], cube[Index_F][1][2], turns);
			swap_s(cube[Index_U][0][2], cube[Index_B][2][0], cube[Index_D][0][2], cube[Index_F][0][2], turns);
			break;
		case Back :
			rotate_face(cube[Index_B], turns);
			 //Crown is up, left, down, right
			swap_s(cube[Index_U][0][0], cube[Index_L][2][0], cube[Index_D][2][2], cube[Index_R][0][2], turns);
			swap_s(cube[Index_U][0][1], cube[Index_L][1][0], cube[Index_D][2][1], cube[Index_R][1][2], turns);
			swap_s(cube[Index_U][0][2], cube[Index_L][0][0], cube[Index_D][2][0], cube[Index_R][2][2], turns);
			break;
		case Left :
			rotate_face(cube[Index_L], turns);
			 //Crown is up, front, down, back
			swap_s(cube[Index_U][0][0], cube[Index_F][0][0], cube[Index_D][0][0], cube[Index_B][2][2], turns);
			swap_s(cube[Index_U][1][0], cube[Index_F][1][0], cube[Index_D][1][0], cube[Index_B][1][2], turns);
			swap_s(cube[Index_U][2][0], cube[Index_F][2][0], cube[Index_D][2][0], cube[Index_B][0][2], turns);
			break;
		case Down :
			rotate_face(cube[Index_D], turns);
 			 //Crown is front, right, back, left
			swap_s(cube[Index_F][2][0], cube[Index_R][2][0], cube[Index_B][2][0], cube[Index_L][2][0], turns);
			swap_s(cube[Index_F][2][1], cube[Index_R][2][1], cube[Index_B][2][1], cube[Index_L][2][1], turns);
			swap_s(cube[Index_F][2][2], cube[Index_R][2][2], cube[Index_B][2][2], cube[Index_L][2][2], turns);
			break;
		default: break;
	}
}


bool State::check_continuity() const{
	int id;

	id = _cube[Index_U][0][0].cube_id;
	if (id != _cube[Index_L][0][0].cube_id || id != _cube[Index_B][0][2].cube_id)
		return false;
	id = _cube[Index_U][2][0].cube_id;
	if (id != _cube[Index_L][0][2].cube_id || id != _cube[Index_F][0][0].cube_id)
		return false;
	id = _cube[Index_U][0][2].cube_id;
	if (id != _cube[Index_R][0][2].cube_id|| id != _cube[Index_B][0][0].cube_id)
		return false;
	id = _cube[Index_U][2][2].cube_id;
	if (id != _cube[Index_R][0][0].cube_id|| id != _cube[Index_F][0][2].cube_id)
		return false;

	id = _cube[Index_D][0][0].cube_id;
	if (id != _cube[Index_L][2][2].cube_id || id != _cube[Index_F][2][0].cube_id)
		return false;
	id = _cube[Index_D][2][0].cube_id;
	if (id != _cube[Index_L][2][0].cube_id || id != _cube[Index_B][2][2].cube_id)
		return false;
	id = _cube[Index_D][0][2].cube_id;
	if (id != _cube[Index_R][2][0].cube_id|| id != _cube[Index_F][2][2].cube_id)
		return false;
	id = _cube[Index_D][2][2].cube_id;
	if (id != _cube[Index_R][2][2].cube_id|| id != _cube[Index_B][2][0].cube_id)
		return false;

	//Adding borders ID
	if (_cube[Index_U][0][1].cube_id != _cube[Index_B][0][1].cube_id)
		return false;
	if (_cube[Index_U][1][0].cube_id != _cube[Index_L][0][1].cube_id)
		return false;
	if (_cube[Index_U][2][1].cube_id != _cube[Index_F][0][1].cube_id)
		return false;
	if (_cube[Index_U][1][2].cube_id != _cube[Index_R][0][1].cube_id)
		return false;

	if (_cube[Index_D][0][1].cube_id != _cube[Index_F][2][1].cube_id)
		return false;
	if (_cube[Index_D][1][0].cube_id != _cube[Index_L][2][1].cube_id)
		return false;
	if (_cube[Index_D][2][1].cube_id != _cube[Index_B][2][1].cube_id)
		return false;
	if (_cube[Index_D][1][2].cube_id != _cube[Index_R][2][1].cube_id)
		return false;

	if (_cube[Index_F][1][2].cube_id != _cube[Index_R][1][0].cube_id)
		return false;
	if (_cube[Index_R][1][2].cube_id != _cube[Index_B][1][0].cube_id)
		return false;
	if (_cube[Index_B][1][2].cube_id != _cube[Index_L][1][0].cube_id)
		return false;
	if (_cube[Index_L][1][2].cube_id != _cube[Index_F][1][0].cube_id)
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
/*
constexpr uint get_id_corners_pos(const Cube& cube) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint s = 0;

    s = get_fact_value(cube[Index_U][0][0].cube_id, values, 8);
    s = get_fact_value(cube[Index_U][0][2].cube_id, values, 8) + s * 7;
    s = get_fact_value(cube[Index_U][2][0].cube_id, values, 8) + s * 6;
    s = get_fact_value(cube[Index_U][2][2].cube_id, values, 8) + s * 5;
    s = get_fact_value(cube[Index_D][0][0].cube_id, values, 8) + s * 4;
    s = get_fact_value(cube[Index_D][0][2].cube_id, values, 8) + s * 3;
    s = get_fact_value(cube[Index_D][2][0].cube_id, values, 8) + s * 2;
  //s = get_fact_value(cube[Index_D][2][2].cube_id, values, 8) + s * 1;
	return s;
}

constexpr uint get_id_borders_pos(const Cube& cube) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	uint s = 0;

	s = get_fact_value(cube[Index_U][0][1].cube_id, values, 12);
	s = get_fact_value(cube[Index_U][1][0].cube_id, values, 12) + s * 11;
	s = get_fact_value(cube[Index_U][1][2].cube_id, values, 12) + s * 10;
	s = get_fact_value(cube[Index_U][2][1].cube_id, values, 12) + s * 9;

	s = get_fact_value(cube[Index_F][1][2].cube_id, values, 12) + s * 8;
	s = get_fact_value(cube[Index_R][1][2].cube_id, values, 12) + s * 7;
	s = get_fact_value(cube[Index_B][1][2].cube_id, values, 12) + s * 6;
	s = get_fact_value(cube[Index_L][1][2].cube_id, values, 12) + s * 5;

	s = get_fact_value(cube[Index_D][0][1].cube_id, values, 12) + s * 4;
	s = get_fact_value(cube[Index_D][1][0].cube_id, values, 12) + s * 3;
	s = get_fact_value(cube[Index_D][1][2].cube_id, values, 12) + s * 2;
  //s = get_fact_value(cube[Index_D][2][1].cube_id, values, 12) + s * 1;
	return s;
}

constexpr uint get_id_corners_rot(const Cube& cube) {
	uint s = 0;

    s = cube[Index_U][0][0].rot_id;
	s = s * 3 + cube[Index_U][0][2].rot_id;
	s = s * 3 + cube[Index_U][2][0].rot_id;
	s = s * 3 + cube[Index_U][2][2].rot_id;
	s = s * 3 + cube[Index_D][0][0].rot_id;
	s = s * 3 + cube[Index_D][0][2].rot_id;
	s = s * 3 + cube[Index_D][2][0].rot_id;
	s = s * 3 + cube[Index_D][2][2].rot_id;
	return s;
}

constexpr uint get_id_borders_rot(const Cube& cube) {
	uint s = 0;

	s = cube[Index_U][0][1].rot_id;
	s = s * 2 + cube[Index_U][1][0].rot_id;
	s = s * 2 + cube[Index_U][2][1].rot_id;
	s = s * 2 + cube[Index_U][1][2].rot_id;

	s = s * 2 + cube[Index_F][1][0].rot_id;
	s = s * 2 + cube[Index_R][1][0].rot_id;
	s = s * 2 + cube[Index_B][1][0].rot_id;
	s = s * 2 + cube[Index_L][1][0].rot_id;

	s = s * 2 + cube[Index_D][0][1].rot_id;
	s = s * 2 + cube[Index_D][1][0].rot_id;
	s = s * 2 + cube[Index_D][2][1].rot_id;
	s = s * 2 + cube[Index_D][1][2].rot_id;

	return s;
}

constexpr ID State::id_from_cube(const Cube cube){
    ID id = ID();
	id.corners = get_id_corners_pos(cube) * pow(3, 8) + get_id_corners_rot(cube);
	id.borders_pos = get_id_borders_pos(cube);
	id.borders_rot = get_id_borders_rot(cube);
    return id;
}*/

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

constexpr void set_cube_corners_rot(Cube& cube, DataCorners& data) {
	set_corner_rot(cube[Index_U][0][0].rot_id, cube[Index_L][0][0].rot_id, cube[Index_B][0][2].rot_id, data[0]);
	set_corner_rot(cube[Index_U][0][2].rot_id, cube[Index_B][0][0].rot_id, cube[Index_R][0][2].rot_id, data[1]);
	set_corner_rot(cube[Index_U][2][0].rot_id, cube[Index_F][0][0].rot_id, cube[Index_L][0][2].rot_id, data[2]);
	set_corner_rot(cube[Index_U][2][2].rot_id, cube[Index_R][0][0].rot_id, cube[Index_F][0][2].rot_id, data[3]);
	set_corner_rot(cube[Index_D][0][0].rot_id, cube[Index_L][2][2].rot_id, cube[Index_F][2][0].rot_id, data[4]);
	set_corner_rot(cube[Index_D][0][2].rot_id, cube[Index_F][2][2].rot_id, cube[Index_R][2][0].rot_id, data[5]);
	set_corner_rot(cube[Index_D][2][0].rot_id, cube[Index_B][2][2].rot_id, cube[Index_L][2][0].rot_id, data[6]);
    set_corner_rot(cube[Index_D][2][2].rot_id, cube[Index_R][2][2].rot_id, cube[Index_B][2][0].rot_id, data[7]);
}

constexpr void set_cube_borders_rot(Cube& cube, DataBorders& data) {
    cube[Index_B][0][1].rot_id = !((cube[Index_U][0][1].rot_id = data[0]));
    cube[Index_L][0][1].rot_id = !((cube[Index_U][1][0].rot_id = data[1]));
    cube[Index_F][0][1].rot_id = !((cube[Index_U][2][1].rot_id = data[2]));
    cube[Index_R][0][1].rot_id = !((cube[Index_U][1][2].rot_id = data[3]));
    cube[Index_L][1][2].rot_id = !((cube[Index_F][1][0].rot_id = data[4]));
    cube[Index_F][1][2].rot_id = !((cube[Index_R][1][0].rot_id = data[5]));
    cube[Index_R][1][2].rot_id = !((cube[Index_B][1][0].rot_id = data[6]));
    cube[Index_B][1][2].rot_id = !((cube[Index_L][1][0].rot_id = data[7]));
    cube[Index_F][2][1].rot_id = !((cube[Index_D][0][1].rot_id = data[8]));
    cube[Index_L][2][1].rot_id = !((cube[Index_D][1][0].rot_id = data[9]));
    cube[Index_B][2][1].rot_id = !((cube[Index_D][2][1].rot_id = data[10]));
	cube[Index_R][2][1].rot_id = !((cube[Index_D][1][2].rot_id = data[11]));
}

constexpr void set_cube_corners_pos(Cube& cube, uint id) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    cube[Index_U][0][0].cube_id = cube[Index_L][0][0].cube_id = cube[Index_B][0][2].cube_id = get_value_fact(id, values, 8, 8);
    cube[Index_U][0][2].cube_id = cube[Index_R][0][2].cube_id = cube[Index_B][0][0].cube_id = get_value_fact(id, values, 7, 8);
    cube[Index_U][2][0].cube_id = cube[Index_L][0][2].cube_id = cube[Index_F][0][0].cube_id = get_value_fact(id, values, 6, 8);
    cube[Index_U][2][2].cube_id = cube[Index_R][0][0].cube_id = cube[Index_F][0][2].cube_id = get_value_fact(id, values, 5, 8);
    cube[Index_D][0][0].cube_id = cube[Index_L][2][2].cube_id = cube[Index_F][2][0].cube_id = get_value_fact(id, values, 4, 8);
    cube[Index_D][0][2].cube_id = cube[Index_R][2][0].cube_id = cube[Index_F][2][2].cube_id = get_value_fact(id, values, 3, 8);
    cube[Index_D][2][0].cube_id = cube[Index_L][2][0].cube_id = cube[Index_B][2][2].cube_id = get_value_fact(id, values, 2, 8);
    cube[Index_D][2][2].cube_id = cube[Index_R][2][2].cube_id = cube[Index_B][2][0].cube_id = get_value_fact(id, values, 1, 8);
}

constexpr void set_cube_center(Cube& cube) {
	cube[Index_U][1][1].cube_id = 0;
	cube[Index_F][1][1].cube_id = 1;
	cube[Index_R][1][1].cube_id = 2;
	cube[Index_B][1][1].cube_id = 3;
	cube[Index_L][1][1].cube_id = 4;
	cube[Index_D][1][1].cube_id = 5;
	
	cube[Index_U][1][1].rot_id = 0;
	cube[Index_F][1][1].rot_id = 0;
	cube[Index_R][1][1].rot_id = 0;
	cube[Index_B][1][1].rot_id = 0;
	cube[Index_L][1][1].rot_id = 0;
	cube[Index_D][1][1].rot_id = 0;
}

constexpr void set_cube_borders_pos(Cube& cube, uint id) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    cube[Index_U][0][1].cube_id = cube[Index_B][0][1].cube_id = get_value_fact(id, values, 12, 12);
    cube[Index_U][1][0].cube_id = cube[Index_L][0][1].cube_id = get_value_fact(id, values, 11, 12);
    cube[Index_U][1][2].cube_id = cube[Index_R][0][1].cube_id = get_value_fact(id, values, 10, 12);
    cube[Index_U][2][1].cube_id = cube[Index_F][0][1].cube_id = get_value_fact(id, values, 9, 12);

    cube[Index_F][1][2].cube_id = cube[Index_R][1][0].cube_id = get_value_fact(id, values, 8, 12);
    cube[Index_R][1][2].cube_id = cube[Index_B][1][0].cube_id = get_value_fact(id, values, 7, 12);
    cube[Index_B][1][2].cube_id = cube[Index_L][1][0].cube_id = get_value_fact(id, values, 6, 12);
    cube[Index_L][1][2].cube_id = cube[Index_F][1][0].cube_id = get_value_fact(id, values, 5, 12);

    cube[Index_D][0][1].cube_id = cube[Index_F][2][1].cube_id = get_value_fact(id, values, 4, 12);
    cube[Index_D][1][0].cube_id = cube[Index_L][2][1].cube_id = get_value_fact(id, values, 3, 12);
    cube[Index_D][1][2].cube_id = cube[Index_R][2][1].cube_id = get_value_fact(id, values, 2, 12);
    cube[Index_D][2][1].cube_id = cube[Index_B][2][1].cube_id = get_value_fact(id, values, 1, 12);
}

constexpr Cube State::cube_from_id(const ID id) {
    Cube cube = Cube();
    set_cube_corners_pos(cube, id.corners / pow(3, 8));
    set_cube_corners_rot(cube, id.corners % pow(3, 8));
    set_cube_borders_pos(cube, id.borders_pos);
    set_cube_borders_rot(cube, id.borders_rot);
    set_cube_center(cube);
    return cube;
}

Cube State::to_cube() const {
	return cube_from_id(_id);
}

inline void swap_data(DataBorders& pos, DataBorders& rot, int a, int b, int c, int d) {
	uchar t= pos[d];
	pos[d] = pos[c];
	pos[c] = pos[b];
	pos[b] = pos[a];
	pos[a] = t;

	t      = rot[d];
	rot[d] = rot[c];
	rot[c] = rot[b];
	rot[b] = rot[a];
	rot[a] = t;
}

inline uchar rotate_cw(uchar c) {
	if (c == 0)
		return 1;
	else if (c == 1)
		return 2;
	else
		return 0;
}

inline uchar rotate_cc(uchar c) {
	if (c == 0)
		return 2;
	else if (c == 1)
		return 0;
	else
		return 1;
}


inline void swap_data(DataCorners& pos, DataCorners& rot, int a, int b, int c, int d) {
	uchar t= pos[d];
	pos[d] = pos[c];
	pos[c] = pos[b];
	pos[b] = pos[a];
	pos[a] = t;

	t      = rotate_cw(rot[d]);
	rot[d] = rotate_cc(rot[c]);
	rot[c] = rotate_cw(rot[b]);
	rot[b] = rotate_cc(rot[a]);
	rot[a] = t;
}


inline void swap_data_poles(DataCorners& pos, DataCorners& rot, int a, int b, int c, int d) {
	uchar t= pos[d];
	pos[d] = pos[c];
	pos[c] = pos[b];
	pos[b] = pos[a];
	pos[a] = t;

	t      = rot[d];
	rot[d] = rot[c];
	rot[c] = rot[b];
	rot[b] = rot[a];
	rot[a] = t;
}

void State::_apply_movement(Data& data, Movement m) {
	switch (m) {
		case None : return;
		case Up ://TODO combine pos and rot for easy swapping
			swap_data_poles(data.corners_pos, data.corners_rot, 1, 3, 2, 0);
			swap_data(data.borders_pos, data.borders_rot, 3, 1, 0, 2);
			break;
		case Front :
			swap_data(data.corners_pos, data.corners_rot, 5, 4, 2, 3);
			swap_data(data.borders_pos, data.borders_rot, 3, 4, 8, 7);
			data.borders_rot[4] = !data.borders_rot[4];
			data.borders_rot[2] = !data.borders_rot[2];
			break;
		case Right :
			swap_data(data.corners_pos, data.corners_rot, 3, 1, 7, 5);
			swap_data(data.borders_pos, data.borders_rot, 4, 2, 5, 10);
			data.borders_rot[5] = !data.borders_rot[5];
			data.borders_rot[3] = !data.borders_rot[3];
			break;
		case Back :
			swap_data(data.corners_pos, data.corners_rot, 1, 0, 6, 7);
			swap_data(data.borders_pos, data.borders_rot, 0, 6, 11, 5);
			data.borders_rot[6] = !data.borders_rot[6];
			data.borders_rot[0] = !data.borders_rot[0];
			break;
		case Left :
			swap_data(data.corners_pos, data.corners_rot, 0, 2, 4, 6);
			swap_data(data.borders_pos, data.borders_rot, 1, 7, 9, 6);
			data.borders_rot[7] = !data.borders_rot[7];
			data.borders_rot[1] = !data.borders_rot[1];
			break;
		case Down :
			swap_data_poles(data.corners_pos, data.corners_rot, 4, 5, 7, 6);
			swap_data(data.borders_pos, data.borders_rot, 11, 9, 8, 10);
			break;
		default: break;
	}
}
/*
void State::_apply_movement(Data& data, Movement m, int turns) {
	for (int i = 0; i < turns; i++)
		_apply_movement(data, m);
} */


constexpr uint get_id_corners_pos(const DataCorners& data) {
    uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint s = 0;

    s = get_fact_value(data[0], values, 8);
    s = get_fact_value(data[1], values, 8) + s * 7;
    s = get_fact_value(data[2], values, 8) + s * 6;
    s = get_fact_value(data[3], values, 8) + s * 5;
    s = get_fact_value(data[4], values, 8) + s * 4;
    s = get_fact_value(data[5], values, 8) + s * 3;
    s = get_fact_value(data[6], values, 8) + s * 2;
  //s = get_fact_value(data[7], values, 8) + s * 1;
    return s;
}

constexpr uint get_id_borders_pos(const DataBorders& data) {
    uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    uint s = 0;

    s = get_fact_value(data[0], values, 12);
    s = get_fact_value(data[1], values, 12) + s * 11;
    s = get_fact_value(data[2], values, 12) + s * 10;
    s = get_fact_value(data[3], values, 12) + s * 9;

    s = get_fact_value(data[4], values, 12) + s * 8;
    s = get_fact_value(data[5], values, 12) + s * 7;
    s = get_fact_value(data[6], values, 12) + s * 6;
    s = get_fact_value(data[7], values, 12) + s * 5;

    s = get_fact_value(data[8], values, 12) + s * 4;
    s = get_fact_value(data[9], values, 12) + s * 3;
    s = get_fact_value(data[10], values, 12) + s * 2;
  //s = get_fact_value(data[11], values, 12) + s * 1;
    return s;
}

constexpr uint get_id_corners_rot(const DataCorners& data) {
    uint s = 0;

    s = data[0];
    s = s * 3 + data[1];
    s = s * 3 + data[2];
    s = s * 3 + data[3];
    s = s * 3 + data[4];
    s = s * 3 + data[5];
    s = s * 3 + data[6];
    s = s * 3 + data[7];
    return s;
}

constexpr uint get_id_borders_rot(const DataBorders& data) {
    uint s = 0;

    s = data[0];
    s = s * 2 + data[1];
    s = s * 2 + data[2];
    s = s * 2 + data[3];

    s = s * 2 + data[4];
    s = s * 2 + data[5];
    s = s * 2 + data[6];
    s = s * 2 + data[7];

    s = s * 2 + data[8];
    s = s * 2 + data[9];
    s = s * 2 + data[10];
    s = s * 2 + data[11];

    return s;
}

constexpr ID State::id_from_data(const Data data){
    ID id = ID();
    id.corners = get_id_corners_pos(data.corners_pos) * pow(3, 8) + get_id_corners_rot(data.corners_rot);
    id.borders_pos = get_id_borders_pos(data.borders_pos);
    id.borders_rot = get_id_borders_rot(data.borders_rot);
    return id;
}

constexpr void set_data_corners_rot(DataCorners& data, uint id) {
    data[7] = id % 3; id /= 3;
	data[6] = id % 3; id /= 3;
	data[5] = id % 3; id /= 3;
	data[4] = id % 3; id /= 3;
	data[3] = id % 3; id /= 3;
	data[2] = id % 3; id /= 3;
	data[1] = id % 3; id /= 3;
	data[0] = id; //s % 3)); s /= 3;
}

constexpr void set_data_borders_rot(DataBorders& data, uint id) {
	uint s = id;

	data[11]= s % 2; s /= 2;
    data[10]= s % 2; s /= 2;
    data[9] = s % 2; s /= 2;
    data[8] = s % 2; s /= 2;

    data[7] = s % 2; s /= 2;
    data[6] = s % 2; s /= 2;
    data[5] = s % 2; s /= 2;
    data[4] = s % 2; s /= 2;

    data[3] = s % 2; s /= 2;
    data[2] = s % 2; s /= 2;
    data[1] = s % 2; s /= 2;
    data[0] = s; //s % 2)); s /= 2;
}

constexpr void set_data_corners_pos(DataCorners& data, uint id) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    data[0] = get_value_fact(id, values, 8, 8);
    data[1] = get_value_fact(id, values, 7, 8);
    data[2] = get_value_fact(id, values, 6, 8);
    data[3] = get_value_fact(id, values, 5, 8);
    data[4] = get_value_fact(id, values, 4, 8);
    data[5] = get_value_fact(id, values, 3, 8);
    data[6] = get_value_fact(id, values, 2, 8);
    data[7] = get_value_fact(id, values, 1, 8);
}

constexpr void set_data_borders_pos(DataBorders& data, uint id) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    data[0] = get_value_fact(id, values, 12, 12);
    data[1] = get_value_fact(id, values, 11, 12);
    data[2] = get_value_fact(id, values, 10, 12);
    data[3] = get_value_fact(id, values, 9, 12);

    data[4] = get_value_fact(id, values, 8, 12);
    data[5] = get_value_fact(id, values, 7, 12);
    data[6] = get_value_fact(id, values, 6, 12);
    data[7] = get_value_fact(id, values, 5, 12);

    data[8] = get_value_fact(id, values, 4, 12);
    data[9] = get_value_fact(id, values, 3, 12);
    data[10]= get_value_fact(id, values, 2, 12);
    data[11]= get_value_fact(id, values, 1, 12);
}

constexpr Data State::data_from_id(const ID id) {
    Data data = Data();
    set_data_corners_pos(data.corners_pos, id.corners / pow(3, 8));
    set_data_corners_rot(data.corners_rot, id.corners % pow(3, 8));
    set_data_borders_pos(data.borders_pos, id.borders_pos);
    set_data_borders_rot(data.borders_rot, id.borders_rot);
    return data;
}

Data State::to_data() const {
	return data_from_id(_id);
}
