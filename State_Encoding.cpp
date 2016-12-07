
#include "State.hpp"

constexpr int fact(int i) {
    int r = 1;
    for (int it = 1; it <= i; it++) {
        r *= it;
    }
    return r;
}

constexpr int pow(int a, int b) {
    int r = a;
    for (int it = 1; it < b; it++) {
        r *= a;
    }
    return r;
}

inline void move_values_down(uchar* values, uchar pos, int len) {

	//values[pos] = 0;
	for (uchar i = pos + 1; i < len; i++) {
		if (values[i])//TODO REMOVE
			values[i]--;
	}
}

uint get_id_corners_rot(const Data& data) {
	uint s;

    s = data[Index_Up][0][0].rot_id;
	s = s * 3 + data[Index_Up][0][2].rot_id;
	s = s * 3 + data[Index_Up][2][0].rot_id;
	s = s * 3 + data[Index_Up][2][2].rot_id;
	s = s * 3 + data[Index_Down][0][0].rot_id;
	s = s * 3 + data[Index_Down][0][2].rot_id;
	s = s * 3 + data[Index_Down][2][0].rot_id;
	s = s * 3 + data[Index_Down][2][2].rot_id;
	return s;
}

inline uint get_fact_value(uchar cube_id, uchar *values, int len) {
	uchar r = values[cube_id];
	move_values_down(values, cube_id, len);
    return r;
}

uint get_id_corners_pos(const Data& data) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint s;

    s = get_fact_value(data[Index_Up][0][0].cube_id, values, 8);
    s = s * 7 + get_fact_value(data[Index_Up][0][2].cube_id, values, 8);
    s = s * 6 + get_fact_value(data[Index_Up][2][0].cube_id, values, 8);
    s = s * 5 + get_fact_value(data[Index_Up][2][2].cube_id, values, 8);
    s = s * 4 + get_fact_value(data[Index_Down][0][0].cube_id, values, 8);
    s = s * 3 + get_fact_value(data[Index_Down][0][2].cube_id, values, 8);
    s = s * 2 + get_fact_value(data[Index_Down][2][0].cube_id, values, 8);
    //s = s * 1 + get_fact_value(data[Index_Down][2][2].cube_id, values, 8);
	return s;
}

uint get_id_borders_pos(const Data& data) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	uint s;

	s = get_fact_value(data[Index_Up][0][1].cube_id, values, 12);
	s = s * 11 + get_fact_value(data[Index_Up][1][0].cube_id, values, 12);
	s = s * 10 + get_fact_value(data[Index_Up][2][1].cube_id, values, 12);
	s = s * 9 + get_fact_value(data[Index_Up][1][2].cube_id, values, 12);

	s = s * 8 + get_fact_value(data[Index_Front][1][0].cube_id, values, 12);
	s = s * 7 + get_fact_value(data[Index_Right][1][0].cube_id, values, 12);
	s = s * 6 + get_fact_value(data[Index_Back][1][0].cube_id, values, 12);
	s = s * 5 + get_fact_value(data[Index_Left][1][0].cube_id, values, 12);

	s = s * 4 + get_fact_value(data[Index_Down][0][1].cube_id, values, 12);
	s = s * 3 + get_fact_value(data[Index_Down][1][0].cube_id, values, 12);
	s = s * 2 + get_fact_value(data[Index_Down][2][1].cube_id, values, 12);
	//s = s * 1 + get_fact_value(data[Index_Down][1][2].cube_id, values, 12);
	return s;
}

uint get_id_borders_rot(const Data& data) {
	uint s;

	s = data[Index_Up][0][1].rot_id;
	s = s * 2 + data[Index_Up][1][0].rot_id;
	s = s * 2 + data[Index_Up][2][1].rot_id;
	s = s * 2 + data[Index_Up][1][2].rot_id;

	s = s * 2 + data[Index_Front][1][0].rot_id;
	s = s * 2 + data[Index_Right][1][0].rot_id;
	s = s * 2 + data[Index_Back][1][0].rot_id;
	s = s * 2 + data[Index_Left][1][0].rot_id;

	s = s * 2 + data[Index_Down][0][1].rot_id;
	s = s * 2 + data[Index_Down][1][0].rot_id;
	s = s * 2 + data[Index_Down][2][1].rot_id;
	s = s * 2 + data[Index_Down][1][2].rot_id;

	return s;
}

void State::deflate() {
	_id.corners = get_id_corners_pos(*_data) * pow(2, 12) + get_id_corners_rot(*_data);
	_id.borders_pos = get_id_borders_pos(*_data);
	_id.borders_rot = get_id_borders_rot(*_data);
}

inline void move_values_left(uchar* values, uchar pos, int len) {

	//values[pos] = 0;
	for (uchar i = pos; i < len - 1; i++) {//TODO pos + 1
		values[i] = values[i + 1];
	}
}

inline uchar get_value_fact(uint s, uchar* values, uchar pos, uchar len) {
    if (pos == 1)
        return (values[0]);
    uchar c = (s / fact(pos - 1)) % pos;
	uchar r = values[c];
	move_values_left(values, c, len);
    return r;
}

inline void set_corner_rot(uchar& up, uchar& fb, uchar& rl, uchar i) {
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

void set_data_corners_rot(Data& data, uint id) {
	set_corner_rot(data[Index_Up][0][0].rot_id, data[Index_Left][0][0].rot_id, data[Index_Back][0][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Up][0][2].rot_id, data[Index_Left][0][2].rot_id, data[Index_Front][0][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Up][2][0].rot_id, data[Index_Right][0][2].rot_id, data[Index_Back][0][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Up][2][2].rot_id, data[Index_Right][0][0].rot_id, data[Index_Front][0][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Down][0][0].rot_id, data[Index_Left][2][2].rot_id, data[Index_Front][2][0].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Down][0][2].rot_id, data[Index_Left][2][0].rot_id, data[Index_Back][2][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Down][2][0].rot_id, data[Index_Right][2][0].rot_id, data[Index_Front][2][2].rot_id, id % 3); id /= 3;
	set_corner_rot(data[Index_Down][2][2].rot_id, data[Index_Right][2][2].rot_id, data[Index_Back][2][0].rot_id, id % 3); id /= 3;
}

void set_data_borders_rot(Data& data, uint id) {
	uint s = id;
	data[Index_Right][2][1].rot_id = !((data[Index_Down][1][2].rot_id = s % 2)); s /= 2;
	data[Index_Back][2][1].rot_id = !((data[Index_Down][2][1].rot_id = s % 2)); s /= 2;
	data[Index_Left][2][1].rot_id = !((data[Index_Down][1][0].rot_id = s % 2)); s /= 2;
    data[Index_Front][2][1].rot_id = !((data[Index_Down][0][1].rot_id = s % 2)); s /= 2;

	data[Index_Back][1][2].rot_id = !((data[Index_Left][1][0].rot_id = s % 2)); s /= 2;
	data[Index_Right][1][2].rot_id = !((data[Index_Back][1][0].rot_id = s % 2)); s /= 2;
	data[Index_Front][1][2].rot_id = !((data[Index_Right][1][0].rot_id = s % 2)); s /= 2;
	data[Index_Left][1][2].rot_id = !((data[Index_Front][1][0].rot_id = s % 2)); s /= 2;

	data[Index_Right][0][1].rot_id = !((data[Index_Up][1][2].rot_id = s % 2)); s /= 2;
	data[Index_Front][0][1].rot_id = !((data[Index_Up][2][1].rot_id = s % 2)); s /= 2;
	data[Index_Left][0][1].rot_id = !((data[Index_Up][1][0].rot_id = s % 2)); s /= 2;
	data[Index_Back][0][1].rot_id = !((data[Index_Up][0][1].rot_id = s % 2)); s /= 2;//TODO remove % 2 && /= 2
}

void set_data_corners_pos(Data& data, uint id) {
	uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    data[Index_Up][0][0].cube_id = data[Index_Left][0][0].cube_id = data[Index_Back][0][2].cube_id = get_value_fact(id, values, 8, 8);
    data[Index_Up][0][2].cube_id = data[Index_Left][0][2].cube_id = data[Index_Front][0][0].cube_id = get_value_fact(id, values, 7, 8);
    data[Index_Up][2][0].cube_id = data[Index_Right][0][2].cube_id = data[Index_Back][0][0].cube_id = get_value_fact(id, values, 6, 8);
    data[Index_Up][2][2].cube_id = data[Index_Right][0][0].cube_id = data[Index_Front][0][2].cube_id = get_value_fact(id, values, 5, 8);
    data[Index_Down][0][0].cube_id = data[Index_Left][2][2].cube_id = data[Index_Front][2][0].cube_id = get_value_fact(id, values, 4, 8);
    data[Index_Down][0][2].cube_id = data[Index_Left][2][0].cube_id = data[Index_Back][2][2].cube_id = get_value_fact(id, values, 3, 8);
    data[Index_Down][2][0].cube_id = data[Index_Right][2][0].cube_id = data[Index_Front][2][2].cube_id = get_value_fact(id, values, 2, 8);
    data[Index_Down][2][2].cube_id = data[Index_Right][2][2].cube_id = data[Index_Back][2][0].cube_id = get_value_fact(id, values, 1, 8);
}

void set_data_borders_pos(Data& data, uint id) {
	uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    data[Index_Up][0][1].cube_id = data[Index_Right][0][1].cube_id = get_value_fact(id, values, 12, 12);
    data[Index_Up][1][0].cube_id = data[Index_Front][0][1].cube_id = get_value_fact(id, values, 11, 12);
    data[Index_Up][2][1].cube_id = data[Index_Left][0][1].cube_id = get_value_fact(id, values, 10, 12);
    data[Index_Up][1][2].cube_id = data[Index_Back][0][1].cube_id = get_value_fact(id, values, 9, 12);

    data[Index_Front][1][0].cube_id = data[Index_Left][1][2].cube_id = get_value_fact(id, values, 8, 12);
    data[Index_Right][1][0].cube_id = data[Index_Back][1][2].cube_id = get_value_fact(id, values, 7, 12);
    data[Index_Back][1][0].cube_id = data[Index_Right][1][2].cube_id = get_value_fact(id, values, 6, 12);
    data[Index_Left][1][0].cube_id = data[Index_Front][1][2].cube_id = get_value_fact(id, values, 5, 12);

    data[Index_Down][0][1].cube_id = data[Index_Right][2][1].cube_id = get_value_fact(id, values, 4, 12);
    data[Index_Down][1][0].cube_id = data[Index_Back][2][1].cube_id = get_value_fact(id, values, 3, 12);
    data[Index_Down][2][1].cube_id = data[Index_Left][2][1].cube_id = get_value_fact(id, values, 2, 12);
    data[Index_Down][1][2].cube_id = data[Index_Front][2][1].cube_id = get_value_fact(id, values, 1, 12);
}

void get_uids(Data& data) {
    for (int s = Index_Start; s < Index_Len; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
            {
                //TODO remove at
                Square &sq = data[s][l][c];
                if (l == 0 && c == 0)
                    sq.face_id = State::uid_finder.centers.at(sq.cube_id);
                if (l == 1 || c == 1)
                    sq.face_id = State::uid_finder.borders.at(sq.cube_id).at(sq.rot_id);
                else
                    sq.face_id = State::uid_finder.corners.at(sq.cube_id).at(sq.rot_id);
            }
}

void State::inflate() {
    set_data_corners_pos(get_data(), _id.corners / pow(2, 12));
    set_data_corners_rot(get_data(), _id.corners % pow(2, 12));
    set_data_borders_pos(get_data(), _id.borders_pos);
    set_data_borders_rot(get_data(), _id.borders_rot);
    get_uids(get_data());
}
