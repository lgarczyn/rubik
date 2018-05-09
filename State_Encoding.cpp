/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State_Encoding.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:39:25 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/21 01:50:05 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "State.hpp"
#include "Move.hpp"

namespace Encoding {

	constexpr Finder _calculate_finder(const Cube &cube) {
		Finder finder = Finder();

		for (int s = Index_Start; s < Index_End; s++)
			for (int l = 0; l < size; l++)
				for (int c = 0; c < size; c++) {
					Coord co = (Coord){s, l, c};
					int id =
					    cube[s][l][c].get_uid(l, c); // TODO replace with smart
					assert(id < 3 * 3 * 6);
					finder.at(id) = co;
				}
		return finder;
	}

	static constexpr int fact(int i) {
		int r = 1;
		for (int it = 1; it <= i; it++) {
			r *= it;
		}
		return r;
	}

	static constexpr int pow(int a, int b) {
		int r = 1;
		for (int it = 0; it < b; it++) {
			r *= a;
		}
		return r;
	}

	const int corners_max = pow(3, 8) * fact(8);
	const int corners_max_rot = pow(3, 8);
	const int corners_upper_max_rot = pow(3, 4);
	const int corners_upper_max_pos = fact(4);

	uint constexpr floor_index_upper_corners(uint u) {
		uint pos = u / Encoding::corners_max_rot;
		uint rot = u % Encoding::corners_max_rot;

		uint pos_upper = pos - (pos % Encoding::corners_upper_max_pos);
		uint rot_upper = rot - (rot % Encoding::corners_upper_max_rot);
		return pos_upper * Encoding::corners_max_rot + rot_upper;
	}

	static constexpr inline void move_values_down(uchar *values, uchar pos,
	    int len) {
		for (uchar i = pos + 1; i < len; i++) {
			values[i]--;
		}
	}

	static constexpr inline uint get_fact_value(uchar cube_id, uchar *values,
	    int len) {
		uchar r = values[cube_id];
		move_values_down(values, cube_id, len);
		return r;
	}

	static constexpr inline void set_corner_rot(uchar &up, uchar &fb, uchar &rl,
	    uchar i) {
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

	static constexpr void set_cube_corners_rot(Cube &cube,
	    const DataCorners &data) {
		set_corner_rot(cube[Index_U][0][0].rot_id, cube[Index_L][0][0].rot_id,
		    cube[Index_B][0][2].rot_id, data[0].rot_id);
		set_corner_rot(cube[Index_U][0][2].rot_id, cube[Index_B][0][0].rot_id,
		    cube[Index_R][0][2].rot_id, data[1].rot_id);
		set_corner_rot(cube[Index_U][2][0].rot_id, cube[Index_F][0][0].rot_id,
		    cube[Index_L][0][2].rot_id, data[2].rot_id);
		set_corner_rot(cube[Index_U][2][2].rot_id, cube[Index_R][0][0].rot_id,
		    cube[Index_F][0][2].rot_id, data[3].rot_id);
		set_corner_rot(cube[Index_D][0][0].rot_id, cube[Index_L][2][2].rot_id,
		    cube[Index_F][2][0].rot_id, data[4].rot_id);
		set_corner_rot(cube[Index_D][0][2].rot_id, cube[Index_F][2][2].rot_id,
		    cube[Index_R][2][0].rot_id, data[5].rot_id);
		set_corner_rot(cube[Index_D][2][0].rot_id, cube[Index_B][2][2].rot_id,
		    cube[Index_L][2][0].rot_id, data[6].rot_id);
		set_corner_rot(cube[Index_D][2][2].rot_id, cube[Index_R][2][2].rot_id,
		    cube[Index_B][2][0].rot_id, data[7].rot_id);
	}

	static constexpr void set_cube_corners_pos(Cube &cube,
	    const DataCorners &data) {
		cube[Index_U][0][0].cube_id = cube[Index_L][0][0].cube_id =
		    cube[Index_B][0][2].cube_id = data[0].cube_id;
		cube[Index_U][0][2].cube_id = cube[Index_R][0][2].cube_id =
		    cube[Index_B][0][0].cube_id = data[1].cube_id;
		cube[Index_U][2][0].cube_id = cube[Index_L][0][2].cube_id =
		    cube[Index_F][0][0].cube_id = data[2].cube_id;
		cube[Index_U][2][2].cube_id = cube[Index_R][0][0].cube_id =
		    cube[Index_F][0][2].cube_id = data[3].cube_id;
		cube[Index_D][0][0].cube_id = cube[Index_L][2][2].cube_id =
		    cube[Index_F][2][0].cube_id = data[4].cube_id;
		cube[Index_D][0][2].cube_id = cube[Index_R][2][0].cube_id =
		    cube[Index_F][2][2].cube_id = data[5].cube_id;
		cube[Index_D][2][0].cube_id = cube[Index_L][2][0].cube_id =
		    cube[Index_B][2][2].cube_id = data[6].cube_id;
		cube[Index_D][2][2].cube_id = cube[Index_R][2][2].cube_id =
		    cube[Index_B][2][0].cube_id = data[7].cube_id;
	}

	static constexpr void set_cube_center(Cube &cube) {
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

	static constexpr void set_cube_borders_rot(Cube &cube,
	    const DataBorders &data) {
		cube[Index_U][0][1].rot_id = data[0].rot_id;
		cube[Index_B][0][1].rot_id = !data[0].rot_id;
		cube[Index_U][1][0].rot_id = data[1].rot_id;
		cube[Index_L][0][1].rot_id = !data[1].rot_id;
		cube[Index_U][1][2].rot_id = data[2].rot_id;
		cube[Index_R][0][1].rot_id = !data[2].rot_id;
		cube[Index_U][2][1].rot_id = data[3].rot_id;
		cube[Index_F][0][1].rot_id = !data[3].rot_id;

		cube[Index_F][1][0].rot_id = data[4].rot_id;
		cube[Index_L][1][2].rot_id = !data[4].rot_id;
		cube[Index_R][1][0].rot_id = data[5].rot_id;
		cube[Index_F][1][2].rot_id = !data[5].rot_id;
		cube[Index_B][1][0].rot_id = data[6].rot_id;
		cube[Index_R][1][2].rot_id = !data[6].rot_id;
		cube[Index_L][1][0].rot_id = data[7].rot_id;
		cube[Index_B][1][2].rot_id = !data[7].rot_id;

		cube[Index_D][0][1].rot_id = data[8].rot_id;
		cube[Index_F][2][1].rot_id = !data[8].rot_id;
		cube[Index_D][1][0].rot_id = data[9].rot_id;
		cube[Index_L][2][1].rot_id = !data[9].rot_id;
		cube[Index_D][1][2].rot_id = data[10].rot_id;
		cube[Index_R][2][1].rot_id = !data[10].rot_id;
		cube[Index_D][2][1].rot_id = data[11].rot_id;
		cube[Index_B][2][1].rot_id = !data[11].rot_id;
	}

	static constexpr void set_cube_borders_pos(Cube &c,
	    const DataBorders &data) {
		c[Index_U][0][1].cube_id = c[Index_B][0][1].cube_id = data[0].cube_id;
		c[Index_U][1][0].cube_id = c[Index_L][0][1].cube_id = data[1].cube_id;
		c[Index_U][1][2].cube_id = c[Index_R][0][1].cube_id = data[2].cube_id;
		c[Index_U][2][1].cube_id = c[Index_F][0][1].cube_id = data[3].cube_id;

		c[Index_F][1][0].cube_id = c[Index_L][1][2].cube_id = data[4].cube_id;
		c[Index_R][1][0].cube_id = c[Index_F][1][2].cube_id = data[5].cube_id;
		c[Index_B][1][0].cube_id = c[Index_R][1][2].cube_id = data[6].cube_id;
		c[Index_L][1][0].cube_id = c[Index_B][1][2].cube_id = data[7].cube_id;

		c[Index_D][0][1].cube_id = c[Index_F][2][1].cube_id = data[8].cube_id;
		c[Index_D][1][0].cube_id = c[Index_L][2][1].cube_id = data[9].cube_id;
		c[Index_D][1][2].cube_id = c[Index_R][2][1].cube_id = data[10].cube_id;
		c[Index_D][2][1].cube_id = c[Index_B][2][1].cube_id = data[11].cube_id;
	}

	static constexpr Coord coord_from_uid(
	    int uid) { // TODO is this just solution finder?
		auto pair = Square::get_square(uid);
		Square sq = pair.first;
		SquareType type = pair.second;

		switch (type) {
		case st_center:
			return {sq.cube_id, 1, 1};
		case st_border:
			switch (sq.rot_id) {
			case 0:
				switch (sq.cube_id) {
				case 0:
					return {Index_U, 0, 1};
				case 1:
					return {Index_U, 1, 0};
				case 2:
					return {Index_U, 1, 2};
				case 3:
					return {Index_U, 2, 1};
				case 4:
					return {Index_F, 1, 0};
				case 5:
					return {Index_R, 1, 0};
				case 6:
					return {Index_B, 1, 0};
				case 7:
					return {Index_L, 1, 0};
				case 8:
					return {Index_D, 0, 1};
				case 9:
					return {Index_D, 1, 0};
				case 10:
					return {Index_D, 1, 2};
				case 11:
					return {Index_D, 2, 1};
				default:
					throw std::logic_error("impossible");
				}
			case 1:
				switch (sq.cube_id) {
				case 0:
					return {Index_B, 0, 1};
				case 1:
					return {Index_L, 0, 1};
				case 2:
					return {Index_R, 0, 1};
				case 3:
					return {Index_F, 0, 1};
				case 4:
					return {Index_L, 1, 2};
				case 5:
					return {Index_F, 1, 2};
				case 6:
					return {Index_R, 1, 2};
				case 7:
					return {Index_B, 1, 2};
				case 8:
					return {Index_F, 2, 1};
				case 9:
					return {Index_L, 2, 1};
				case 10:
					return {Index_R, 2, 1};
				case 11:
					return {Index_B, 2, 1};
				default:
					throw std::logic_error("impossible");
				}
			default:
				throw std::logic_error("impossible");
			}
		case st_corner:
			switch (sq.rot_id) {
			case 0:
				switch (sq.cube_id) {
				case 0:
					return {Index_U, 0, 0};
				case 1:
					return {Index_U, 0, 2};
				case 2:
					return {Index_U, 2, 0};
				case 3:
					return {Index_U, 2, 2};
				case 4:
					return {Index_D, 0, 0};
				case 5:
					return {Index_D, 0, 2};
				case 6:
					return {Index_D, 2, 0};
				case 7:
					return {Index_D, 2, 2};
				default:
					throw std::logic_error("impossible");
				}
			case 1:
				switch (sq.cube_id) {
				case 0:
					return {Index_L, 0, 0};
				case 1:
					return {Index_B, 0, 0};
				case 2:
					return {Index_F, 0, 0};
				case 3:
					return {Index_R, 0, 0};
				case 4:
					return {Index_L, 2, 2};
				case 5:
					return {Index_F, 2, 2};
				case 6:
					return {Index_B, 2, 2};
				case 7:
					return {Index_R, 2, 2};
				default:
					throw std::logic_error("impossible");
				}
			case 2:
				switch (sq.cube_id) {
				case 0:
					return {Index_B, 0, 2};
				case 1:
					return {Index_R, 0, 2};
				case 2:
					return {Index_L, 0, 2};
				case 3:
					return {Index_F, 0, 2};
				case 4:
					return {Index_F, 2, 0};
				case 5:
					return {Index_R, 2, 0};
				case 6:
					return {Index_L, 2, 0};
				case 7:
					return {Index_B, 2, 0};
				default:
					throw std::logic_error("impossible");
				}
			default:
				throw std::logic_error("impossible");
			}
		default:
			throw std::logic_error("impossible");
		}
	}

	constexpr Cube cube_from_data(const Data &data) {
		Cube cube = Cube();
		set_cube_corners_pos(cube, data.corners);
		set_cube_corners_rot(cube, data.corners);
		set_cube_borders_pos(cube, data.borders);
		set_cube_borders_rot(cube, data.borders);
		set_cube_center(cube);
		return cube;
	}

	static constexpr inline uchar rotate_cw(uchar c) {
		if (c == 0)
			return 1;
		else if (c == 1)
			return 2;
		else
			return 0;
	}

	static constexpr inline uchar rotate_cc(uchar c) {
		if (c == 0)
			return 2;
		else if (c == 1)
			return 0;
		else
			return 1;
	}

	static constexpr inline void swap_data(DataBorders &data, int a, int b,
	    int c, int d) {
		Square t = data[a];
		data[a] = data[d];
		data[d] = data[c];
		data[c] = data[b];
		data[b] = t;

		data[a].rot_id = !data[a].rot_id;
		data[b].rot_id = !data[b].rot_id;
	}

	static constexpr inline void swap_data(DataCorners &data, int a, int b,
	    int c, int d) {
		data[a].rot_id = rotate_cc(data[a].rot_id);
		data[b].rot_id = rotate_cw(data[b].rot_id);
		data[c].rot_id = rotate_cc(data[c].rot_id);
		data[d].rot_id = rotate_cw(data[d].rot_id);

		Square t = data[a];
		data[a] = data[d];
		data[d] = data[c];
		data[c] = data[b];
		data[b] = t;
	}

	template <size_t T>
	static constexpr inline void swap_data_poles(std::array<Square, T> &data,
	    int a, int b, int c, int d) {
		Square t = data[a];
		data[a] = data[d];
		data[d] = data[c];
		data[c] = data[b];
		data[b] = t;
	}

	void constexpr _apply_movement(Data &data, Move::Direction m) {
		switch (m) {
		case Move::Direction::None:
			return;
		case Move::Direction::Up:
			swap_data_poles(data.corners, 1, 3, 2, 0);
			swap_data_poles(data.borders, 3, 1, 0, 2);
			break;
		case Move::Direction::Front:
			swap_data(data.corners, 5, 4, 2, 3);
			swap_data(data.borders, 4, 3, 5, 8);
			break;
		case Move::Direction::Right:
			swap_data(data.corners, 3, 1, 7, 5);
			swap_data(data.borders, 5, 2, 6, 10);
			break;
		case Move::Direction::Back:
			swap_data(data.corners, 1, 0, 6, 7);
			swap_data(data.borders, 6, 0, 7, 11);
			break;
		case Move::Direction::Left:
			swap_data(data.corners, 0, 2, 4, 6);
			swap_data(data.borders, 7, 1, 4, 9);
			break;
		case Move::Direction::Down:
			swap_data_poles(data.corners, 4, 5, 7, 6);
			swap_data_poles(data.borders, 11, 9, 8, 10);
			break;
		default:
			break;
		}
	}

	void constexpr _apply_movement(Data &data, Move m) {
		for (int i = 0; i < m.get_turns(); i++)
			_apply_movement(data, m.direction);
	}

	static constexpr inline void move_values_left(uchar *values, uchar pos,
	    int len) {
		for (uchar i = pos; i < len - 1; i++) {
			values[i] = values[i + 1];
		}
	}

	static constexpr inline uchar get_value_fact(uint s, uchar *values,
	    uchar pos, uchar len) {
		if (pos == 1)
			return values[0];
		uchar c = (s / fact(pos - 1)) % pos;
		uchar r = values[c];
		move_values_left(values, c, len);
		return r;
	}

	static constexpr uint get_id_corners_pos(const DataCorners &data) {
		uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		uint s = 0;

		s = get_fact_value(data[0].cube_id, values, 8);
		s = get_fact_value(data[1].cube_id, values, 8) + s * 7;
		s = get_fact_value(data[2].cube_id, values, 8) + s * 6;
		s = get_fact_value(data[3].cube_id, values, 8) + s * 5;
		s = get_fact_value(data[4].cube_id, values, 8) + s * 4;
		s = get_fact_value(data[5].cube_id, values, 8) + s * 3;
		s = get_fact_value(data[6].cube_id, values, 8) + s * 2;
		// implied but useless line, since it always returns 0;
		// s = get_fact_value(data[7], values, 8) + s * 1;
		return s;
	}

	static constexpr uint get_id_borders_pos(const DataBorders &data) {
		uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
		uint s = 0;

		s = get_fact_value(data[0].cube_id, values, 12);
		s = get_fact_value(data[1].cube_id, values, 12) + s * 11;
		s = get_fact_value(data[2].cube_id, values, 12) + s * 10;
		s = get_fact_value(data[3].cube_id, values, 12) + s * 9;

		s = get_fact_value(data[4].cube_id, values, 12) + s * 8;
		s = get_fact_value(data[5].cube_id, values, 12) + s * 7;
		s = get_fact_value(data[6].cube_id, values, 12) + s * 6;
		s = get_fact_value(data[7].cube_id, values, 12) + s * 5;

		s = get_fact_value(data[8].cube_id, values, 12) + s * 4;
		s = get_fact_value(data[9].cube_id, values, 12) + s * 3;
		s = get_fact_value(data[10].cube_id, values, 12) + s * 2;
		// implied but useless line, since it always returns 0;
		// s = get_fact_value(data[11], values, 12) + s * 1;
		return s;
	}

	static constexpr uint get_id_borders_rot(const DataBorders &data) {
		uint s = 0;

		s = s * 2 + data[0].rot_id;
		s = s * 2 + data[1].rot_id;
		s = s * 2 + data[2].rot_id;
		s = s * 2 + data[3].rot_id;

		s = s * 2 + data[4].rot_id;
		s = s * 2 + data[5].rot_id;
		s = s * 2 + data[6].rot_id;
		s = s * 2 + data[7].rot_id;

		s = s * 2 + data[8].rot_id;
		s = s * 2 + data[9].rot_id;
		s = s * 2 + data[10].rot_id;
		s = s * 2 + data[11].rot_id;

		return s;
	}

	static constexpr uint get_id_corners_rot(const DataCorners &data) {
		uint s = 0;

		s = s * 3 + data[0].rot_id;
		s = s * 3 + data[1].rot_id;
		s = s * 3 + data[2].rot_id;
		s = s * 3 + data[3].rot_id;
		s = s * 3 + data[4].rot_id;
		s = s * 3 + data[5].rot_id;
		s = s * 3 + data[6].rot_id;
		s = s * 3 + data[7].rot_id;
		return s;
	}

	constexpr ID id_from_data(const Data &data) {
		ID id = ID();
		id.corners_pos = get_id_corners_pos(data.corners);
		id.corners_rot = get_id_corners_rot(data.corners);
		id.borders_pos = get_id_borders_pos(data.borders);
		id.borders_rot = get_id_borders_rot(data.borders);
		return id;
	}

	static constexpr void set_data_corners_rot(DataCorners &data, uint id) {
		data[7].rot_id = id % 3;
		id /= 3;
		data[6].rot_id = id % 3;
		id /= 3;
		data[5].rot_id = id % 3;
		id /= 3;
		data[4].rot_id = id % 3;
		id /= 3;
		data[3].rot_id = id % 3;
		id /= 3;
		data[2].rot_id = id % 3;
		id /= 3;
		data[1].rot_id = id % 3;
		id /= 3;
		data[0].rot_id = id; // s % 3)); s /= 3;
	}

	static constexpr void set_data_borders_rot(DataBorders &data,
	    uint id) { // TODO optimize
		uint s = id;

		data[11].rot_id = s % 2;
		s /= 2;
		data[10].rot_id = s % 2;
		s /= 2;
		data[9].rot_id = s % 2;
		s /= 2;
		data[8].rot_id = s % 2;
		s /= 2;

		data[7].rot_id = s % 2;
		s /= 2;
		data[6].rot_id = s % 2;
		s /= 2;
		data[5].rot_id = s % 2;
		s /= 2;
		data[4].rot_id = s % 2;
		s /= 2;

		data[3].rot_id = s % 2;
		s /= 2;
		data[2].rot_id = s % 2;
		s /= 2;
		data[1].rot_id = s % 2;
		s /= 2;
		data[0].rot_id = s; // s % 2)); s /= 2;
	}

	static constexpr void set_data_corners_pos(DataCorners &data, uint id) {
		uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};

		data[0].cube_id = get_value_fact(id, values, 8, 8);
		data[1].cube_id = get_value_fact(id, values, 7, 8);
		data[2].cube_id = get_value_fact(id, values, 6, 8);
		data[3].cube_id = get_value_fact(id, values, 5, 8);
		data[4].cube_id = get_value_fact(id, values, 4, 8);
		data[5].cube_id = get_value_fact(id, values, 3, 8);
		data[6].cube_id = get_value_fact(id, values, 2, 8);
		data[7].cube_id = get_value_fact(id, values, 1, 8);
	}

	static constexpr void set_data_borders_pos(DataBorders &data, uint id) {
		uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

		data[0].cube_id = get_value_fact(id, values, 12, 12);
		data[1].cube_id = get_value_fact(id, values, 11, 12);
		data[2].cube_id = get_value_fact(id, values, 10, 12);
		data[3].cube_id = get_value_fact(id, values, 9, 12);

		data[4].cube_id = get_value_fact(id, values, 8, 12);
		data[5].cube_id = get_value_fact(id, values, 7, 12);
		data[6].cube_id = get_value_fact(id, values, 6, 12);
		data[7].cube_id = get_value_fact(id, values, 5, 12);

		data[8].cube_id = get_value_fact(id, values, 4, 12);
		data[9].cube_id = get_value_fact(id, values, 3, 12);
		data[10].cube_id = get_value_fact(id, values, 2, 12);
		data[11].cube_id = get_value_fact(id, values, 1, 12);
	}

	constexpr Data data_from_id(const ID id) {
		Data data = Data();
		set_data_corners_pos(data.corners, id.corners_pos);
		set_data_corners_rot(data.corners, id.corners_rot);
		set_data_borders_pos(data.borders, id.borders_pos);
		set_data_borders_rot(data.borders, id.borders_rot);
		return data;
	}

	constexpr Cube cube_from_id(const ID id) {
		Data d = data_from_id(id);
		return cube_from_data(d);
	}

	static constexpr uint get_id_corners_pos(const Cube &cube) {
		uchar values[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		uint s = 0;

		s = get_fact_value(cube[Index_U][0][0].cube_id, values, 8);
		s = get_fact_value(cube[Index_U][0][2].cube_id, values, 8) + s * 7;
		s = get_fact_value(cube[Index_U][2][0].cube_id, values, 8) + s * 6;
		s = get_fact_value(cube[Index_U][2][2].cube_id, values, 8) + s * 5;
		s = get_fact_value(cube[Index_D][0][0].cube_id, values, 8) + s * 4;
		s = get_fact_value(cube[Index_D][0][2].cube_id, values, 8) + s * 3;
		s = get_fact_value(cube[Index_D][2][0].cube_id, values, 8) + s * 2;
		// s = get_fact_value(cube[Index_D][2][2].cube_id, values, 8) + s * 1;
		return s;
	}

	static constexpr uint get_id_borders_pos(const Cube &cube) {
		uchar values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
		uint s = 0;

		s = get_fact_value(cube[Index_U][0][1].cube_id, values, 12);
		s = get_fact_value(cube[Index_U][1][0].cube_id, values, 12) + s * 11;
		s = get_fact_value(cube[Index_U][1][2].cube_id, values, 12) + s * 10;
		s = get_fact_value(cube[Index_U][2][1].cube_id, values, 12) + s * 9;

		s = get_fact_value(cube[Index_F][1][0].cube_id, values, 12) + s * 8;
		s = get_fact_value(cube[Index_R][1][0].cube_id, values, 12) + s * 7;
		s = get_fact_value(cube[Index_B][1][0].cube_id, values, 12) + s * 6;
		s = get_fact_value(cube[Index_L][1][0].cube_id, values, 12) + s * 5;

		s = get_fact_value(cube[Index_D][0][1].cube_id, values, 12) + s * 4;
		s = get_fact_value(cube[Index_D][1][0].cube_id, values, 12) + s * 3;
		s = get_fact_value(cube[Index_D][1][2].cube_id, values, 12) + s * 2;
		// s = get_fact_value(cube[Index_D][2][1].cube_id, values, 12) + s * 1;
		return s;
	}

	static constexpr uint get_id_corners_rot(const Cube &cube) {
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

	static constexpr uint get_id_borders_rot(const Cube &cube) {
		uint s = 0;

		s = cube[Index_U][0][1].rot_id;
		s = s * 2 + cube[Index_U][1][0].rot_id;
		s = s * 2 + cube[Index_U][1][2].rot_id;
		s = s * 2 + cube[Index_U][2][1].rot_id;

		s = s * 2 + cube[Index_F][1][0].rot_id;
		s = s * 2 + cube[Index_R][1][0].rot_id;
		s = s * 2 + cube[Index_B][1][0].rot_id;
		s = s * 2 + cube[Index_L][1][0].rot_id;

		s = s * 2 + cube[Index_D][0][1].rot_id;
		s = s * 2 + cube[Index_D][1][0].rot_id;
		s = s * 2 + cube[Index_D][1][2].rot_id;
		s = s * 2 + cube[Index_D][2][1].rot_id;

		return s;
	}

	constexpr ID id_from_cube(const Cube &cube) {
		ID id = ID();
		id.corners_pos = get_id_corners_pos(cube);
		id.corners_rot = get_id_corners_rot(cube);
		id.borders_pos = get_id_borders_pos(cube);
		id.borders_rot = get_id_borders_rot(cube);
		return id;
	}
}

constexpr Finder State::_calculate_finder(const Cube &cube) {
	return Encoding::_calculate_finder(cube);
}
constexpr Cube State::cube_from_data(const Data &data) {
	return Encoding::cube_from_data(data);
}
constexpr ID State::id_from_data(const Data &data) {
	return Encoding::id_from_data(data);
}
constexpr Data State::data_from_id(const ID id) {
	return Encoding::data_from_id(id);
}
constexpr Cube State::cube_from_id(const ID id) {
	return Encoding::cube_from_id(id);
}
constexpr ID State::id_from_cube(const Cube &cube) {
	return Encoding::id_from_cube(cube);
}
constexpr void State::_apply_movement(Data &data, Move::Direction m) {
	Encoding::_apply_movement(data, m);
}
constexpr void State::_apply_movement(Data &data, Move m) {
	Encoding::_apply_movement(data, m);
}