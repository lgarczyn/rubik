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

#pragma once

#include "Move.hpp"
#include "State.hpp"
#include <cassert>

namespace Encoding {

	constexpr Finder _calculate_finder(const Cube &cube) {
		Finder finder = Finder();

		for (uchar s = Index_Start; s < Index_End; s++)
			for (uchar l = 0; l < size; l++)
				for (uchar c = 0; c < size; c++) {
					Coord co = (Coord){s, l, c};
					int id =
					    cube[s][l][c].get_uid(l, c);
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

	static constexpr void set_cube_corners_rot(Cube &c,
	    const DataCorners &data) {
		set_corner_rot(c[Index_U][0][0].rot_id, c[Index_L][0][0].rot_id, c[Index_B][0][2].rot_id,
		    data[Corner_ULB].rot_id);
		set_corner_rot(c[Index_U][0][2].rot_id, c[Index_B][0][0].rot_id, c[Index_R][0][2].rot_id,
		    data[Corner_URB].rot_id);
		set_corner_rot(c[Index_U][2][0].rot_id, c[Index_F][0][0].rot_id, c[Index_L][0][2].rot_id,
		    data[Corner_ULF].rot_id);
		set_corner_rot(c[Index_U][2][2].rot_id, c[Index_R][0][0].rot_id, c[Index_F][0][2].rot_id,
		    data[Corner_URF].rot_id);
		set_corner_rot(c[Index_D][0][0].rot_id, c[Index_L][2][2].rot_id, c[Index_F][2][0].rot_id,
		    data[Corner_DLF].rot_id);
		set_corner_rot(c[Index_D][0][2].rot_id, c[Index_F][2][2].rot_id, c[Index_R][2][0].rot_id,
		    data[Corner_DRF].rot_id);
		set_corner_rot(c[Index_D][2][0].rot_id, c[Index_B][2][2].rot_id, c[Index_L][2][0].rot_id,
		    data[Corner_DLB].rot_id);
		set_corner_rot(c[Index_D][2][2].rot_id, c[Index_R][2][2].rot_id, c[Index_B][2][0].rot_id,
		    data[Corner_DRB].rot_id);
	}

	static constexpr void set_cube_corners_pos(Cube &c,
	    const DataCorners &data) {
		c[Index_U][0][0].cube_id = c[Index_L][0][0].cube_id = c[Index_B][0][2].cube_id = data[Corner_ULB].cube_id;
		c[Index_U][0][2].cube_id = c[Index_R][0][2].cube_id = c[Index_B][0][0].cube_id = data[Corner_URB].cube_id;
		c[Index_U][2][0].cube_id = c[Index_L][0][2].cube_id = c[Index_F][0][0].cube_id = data[Corner_ULF].cube_id;
		c[Index_U][2][2].cube_id = c[Index_R][0][0].cube_id = c[Index_F][0][2].cube_id = data[Corner_URF].cube_id;
		c[Index_D][0][0].cube_id = c[Index_L][2][2].cube_id = c[Index_F][2][0].cube_id = data[Corner_DLF].cube_id;
		c[Index_D][0][2].cube_id = c[Index_R][2][0].cube_id = c[Index_F][2][2].cube_id = data[Corner_DRF].cube_id;
		c[Index_D][2][0].cube_id = c[Index_L][2][0].cube_id = c[Index_B][2][2].cube_id = data[Corner_DLB].cube_id;
		c[Index_D][2][2].cube_id = c[Index_R][2][2].cube_id = c[Index_B][2][0].cube_id = data[Corner_DRB].cube_id;
	}

	static constexpr void set_cube_center(Cube &cube) {
		cube[Index_U][1][1] = Square(0, 0);
		cube[Index_F][1][1] = Square(1, 0);
		cube[Index_R][1][1] = Square(2, 0);
		cube[Index_B][1][1] = Square(3, 0);
		cube[Index_L][1][1] = Square(4, 0);
		cube[Index_D][1][1] = Square(5, 0);
	}

	static constexpr void set_cube_borders_rot(Cube &cube,
	    const DataBorders &data) {
		cube[Index_U][0][1].rot_id = data[Border_UB].rot_id;
		cube[Index_B][0][1].rot_id = !data[Border_UB].rot_id;
		cube[Index_U][1][0].rot_id = data[Border_UL].rot_id;
		cube[Index_L][0][1].rot_id = !data[Border_UL].rot_id;
		cube[Index_U][1][2].rot_id = data[Border_UR].rot_id;
		cube[Index_R][0][1].rot_id = !data[Border_UR].rot_id;
		cube[Index_U][2][1].rot_id = data[Border_UF].rot_id;
		cube[Index_F][0][1].rot_id = !data[Border_UF].rot_id;

		cube[Index_F][1][0].rot_id = data[Border_FL].rot_id;
		cube[Index_L][1][2].rot_id = !data[Border_FL].rot_id;
		cube[Index_R][1][0].rot_id = data[Border_RF].rot_id;
		cube[Index_F][1][2].rot_id = !data[Border_RF].rot_id;
		cube[Index_B][1][0].rot_id = data[Border_BR].rot_id;
		cube[Index_R][1][2].rot_id = !data[Border_BR].rot_id;
		cube[Index_L][1][0].rot_id = data[Border_LB].rot_id;
		cube[Index_B][1][2].rot_id = !data[Border_LB].rot_id;

		cube[Index_D][0][1].rot_id = data[Border_DF].rot_id;
		cube[Index_F][2][1].rot_id = !data[Border_DF].rot_id;
		cube[Index_D][1][0].rot_id = data[Border_DL].rot_id;
		cube[Index_L][2][1].rot_id = !data[Border_DL].rot_id;
		cube[Index_D][1][2].rot_id = data[Border_DR].rot_id;
		cube[Index_R][2][1].rot_id = !data[Border_DR].rot_id;
		cube[Index_D][2][1].rot_id = data[Border_DB].rot_id;
		cube[Index_B][2][1].rot_id = !data[Border_DB].rot_id;
	}

	static constexpr void set_cube_borders_pos(Cube &c,
	    const DataBorders &data) {
		c[Index_U][0][1].cube_id = c[Index_B][0][1].cube_id = data[Border_UB].cube_id;
		c[Index_U][1][0].cube_id = c[Index_L][0][1].cube_id = data[Border_UL].cube_id;
		c[Index_U][1][2].cube_id = c[Index_R][0][1].cube_id = data[Border_UR].cube_id;
		c[Index_U][2][1].cube_id = c[Index_F][0][1].cube_id = data[Border_UF].cube_id;

		c[Index_F][1][0].cube_id = c[Index_L][1][2].cube_id = data[Border_FL].cube_id;
		c[Index_R][1][0].cube_id = c[Index_F][1][2].cube_id = data[Border_RF].cube_id;
		c[Index_B][1][0].cube_id = c[Index_R][1][2].cube_id = data[Border_BR].cube_id;
		c[Index_L][1][0].cube_id = c[Index_B][1][2].cube_id = data[Border_LB].cube_id;

		c[Index_D][0][1].cube_id = c[Index_F][2][1].cube_id = data[Border_DF].cube_id;
		c[Index_D][1][0].cube_id = c[Index_L][2][1].cube_id = data[Border_DL].cube_id;
		c[Index_D][1][2].cube_id = c[Index_R][2][1].cube_id = data[Border_DR].cube_id;
		c[Index_D][2][1].cube_id = c[Index_B][2][1].cube_id = data[Border_DB].cube_id;
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
		case Move::Direction::Up:
			swap_data_poles(data.corners, Corner_URB, Corner_URF, Corner_ULF, Corner_ULB);
			swap_data_poles(data.borders, Border_UF, Border_UL, Border_UB, Border_UR);
			break;
		case Move::Direction::Front:
			swap_data(data.corners, Corner_DRF, Corner_DLF, Corner_ULF, Corner_URF);
			swap_data(data.borders, Border_FL, Border_UF, Border_RF, Border_DF);
			break;
		case Move::Direction::Right:
			swap_data(data.corners, Corner_URF, Corner_URB, Corner_DRB, Corner_DRF);
			swap_data(data.borders, Border_RF, Border_UR, Border_BR, Border_DR);
			break;
		case Move::Direction::Back:
			swap_data(data.corners, Corner_URB, Corner_ULB, Corner_DLB, Corner_DRB);
			swap_data(data.borders, Border_BR, Border_UB, Border_LB, Border_DB);
			break;
		case Move::Direction::Left:
			swap_data(data.corners, Corner_ULB, Corner_ULF, Corner_DLF, Corner_DLB);
			swap_data(data.borders, Border_LB, Border_UL, Border_FL, Border_DL);
			break;
		case Move::Direction::Down:
			swap_data_poles(data.corners, Corner_DLF, Corner_DRF, Corner_DRB, Corner_DLB);
			swap_data_poles(data.borders, Border_DB, Border_DL, Border_DF, Border_DR);
			break;
		default:
			break;
		}
	}

	void constexpr _apply_movement(Data &data, Move m) {
		for (int i = 0; i < m.get_turns(); i++)
			_apply_movement(data, m.direction);
	}

	static constexpr inline uint get_fact_value(uchar cube_id, uint &values, uint max) {
		uint r = 0;

		r = __builtin_popcount(values >> (max - cube_id));

		values ^= 1 << (max - cube_id - 1);

		return r;
	}

	static constexpr uint get_id_corners_pos(const DataCorners &data) {
		uint values = 0b11111111;
		uint s = 0;

		s = get_fact_value(data[Corner_URF].cube_id, values, 8);
		s = get_fact_value(data[Corner_ULF].cube_id, values, 8) + s * 7;
		s = get_fact_value(data[Corner_ULB].cube_id, values, 8) + s * 6;
		s = get_fact_value(data[Corner_URB].cube_id, values, 8) + s * 5;
		s = get_fact_value(data[Corner_DRF].cube_id, values, 8) + s * 4;
		s = get_fact_value(data[Corner_DLF].cube_id, values, 8) + s * 3;
		s = get_fact_value(data[Corner_DLB].cube_id, values, 8) + s * 2;
		// implied but useless line, since it always returns 0;
		// s = get_fact_value(data[Corner_DRB], values, 8) + s * 1;
		return s;
	}

	static constexpr uint get_id_borders_pos(const DataBorders &data) {
		uint values = 0b111111111111;
		uint s = 0;

		s = get_fact_value(data[Border_UR].cube_id, values, 12);
		s = get_fact_value(data[Border_UF].cube_id, values, 12) + s * 11;
		s = get_fact_value(data[Border_UL].cube_id, values, 12) + s * 10;
		s = get_fact_value(data[Border_UB].cube_id, values, 12) + s * 9;

		s = get_fact_value(data[Border_DR].cube_id, values, 12) + s * 8;
		s = get_fact_value(data[Border_DF].cube_id, values, 12) + s * 7;
		s = get_fact_value(data[Border_DL].cube_id, values, 12) + s * 6;
		s = get_fact_value(data[Border_DB].cube_id, values, 12) + s * 5;

		s = get_fact_value(data[Border_RF].cube_id, values, 12) + s * 4;
		s = get_fact_value(data[Border_FL].cube_id, values, 12) + s * 3;
		s = get_fact_value(data[Border_LB].cube_id, values, 12) + s * 2;
		// implied but useless line, since it always returns 0;
		s = get_fact_value(data[Border_BR].cube_id, values, 12) + s * 1;
		return s;
	}

	static constexpr uint get_id_borders_crown_pos(const DataBorders &data) {
		uint values = 0b11111111;
		uint s = 0;

		s = get_fact_value(data[Border_UR].cube_id, values, 8);
		s = get_fact_value(data[Border_UF].cube_id, values, 8) + s * 7;
		s = get_fact_value(data[Border_UL].cube_id, values, 8) + s * 6;
		s = get_fact_value(data[Border_UB].cube_id, values, 8) + s * 5;

		s = get_fact_value(data[Border_DR].cube_id, values, 8) + s * 4;
		s = get_fact_value(data[Border_DF].cube_id, values, 8) + s * 3;
		s = get_fact_value(data[Border_DL].cube_id, values, 8) + s * 2;
		// implied but useless line, since it always returns 0;
		s = get_fact_value(data[Border_DB].cube_id, values, 8) + s * 1;

		if (s >= 40320) // exp(8)
			throw std::logic_error("data was not in second phase");
		return s;
	}

	static constexpr uint get_id_borders_ud_pos(const DataBorders &data) {
		uint values = 0b1111;
		uint s = 0;

		s = get_fact_value(data[Border_RF].cube_id - Border_UD_Start, values, 4) + s * 4;
		s = get_fact_value(data[Border_FL].cube_id - Border_UD_Start, values, 4) + s * 3;
		s = get_fact_value(data[Border_LB].cube_id - Border_UD_Start, values, 4) + s * 2;
		// implied but useless line, since it always returns 0;
		s = get_fact_value(data[Border_BR].cube_id - Border_UD_Start, values, 4) + s * 1;

		//useful warning, but do not rely on it
		//bad data at this point could wrap back around into a valid range
		if (s >= 24) // exp(4)
			throw std::logic_error("data was not in second phase");
		return s;
	}

	static constexpr uint get_id_borders_rot(const DataBorders &data) {
		uint s = 0;

		//UR is deduced from rotation parity
		//s = s * 2 + data[Border_UR].rot_id;
		s = s * 2 + data[Border_UF].rot_id;
		s = s * 2 + data[Border_UL].rot_id;
		s = s * 2 + data[Border_UB].rot_id;
		s = s * 2 + data[Border_DR].rot_id;
		s = s * 2 + data[Border_DF].rot_id;
		s = s * 2 + data[Border_DL].rot_id;
		s = s * 2 + data[Border_DB].rot_id;
		s = s * 2 + data[Border_RF].rot_id;
		s = s * 2 + data[Border_FL].rot_id;
		s = s * 2 + data[Border_LB].rot_id;
		s = s * 2 + data[Border_BR].rot_id;

		return s;
	}

	static constexpr uint get_id_corners_rot(const DataCorners &data) {
		uint s = 0;

		// last rotation is deduced from the rest
		//s = s * 3 + data[Corner_DRB].rot_id;
		s = s * 3 + data[Corner_DLB].rot_id;
		s = s * 3 + data[Corner_DLF].rot_id;
		s = s * 3 + data[Corner_DRF].rot_id;
		s = s * 3 + data[Corner_URB].rot_id;
		s = s * 3 + data[Corner_ULB].rot_id;
		s = s * 3 + data[Corner_ULF].rot_id;
		s = s * 3 + data[Corner_URF].rot_id;
		return s;
	}

	// Returns value of Binomial Coefficient C(n, k)
	static constexpr uint binomial_coeff(uint n, uint k) {
		uint res = 1;

		// Since C(n, k) = C(n, n-k)
		if (k > n - k)
			k = n - k;

		// Calculate value of [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
		for (uint i = 0; i < k; ++i) {
			res *= (n - i);
			res /= (i + 1);
		}

		return res;
	}

	static constexpr std::array<std::array<uint16_t, 4>, 12> get_cnk_table() {
		std::array<std::array<uint16_t, 4>, 12> table = {{}};

		for (int n = 0; n < 12; n++)
			for (int k = 0; k < 4; k++)
				table[n][k] = binomial_coeff(n, k);

		return table;
	}

	static constexpr uint cnk(uint n, uint k) {
		const std::array<std::array<uint16_t, 4>, 12> table = get_cnk_table();

		return table[n][k];
	}

	static constexpr uint16_t get_id_udslice(const DataBorders &borders) {
		//kociemba magic to generate the ud coordinates ID
		//http://kociemba.org/math/twophase.htm
		uint s = 0;
		int k = 3;
		uint n = 11;
		while (k >= 0) {
			//if the border at this position is FL LB BR or RF
			if (borders[n].cube_id >= Border_UD_Start)
				k--;
			else
				s += cnk(n, (uint)k);
			n--;
		}
		return s;
	}

	static constexpr void set_data_udslice(DataBorders &borders, uint16_t id) {
		bool occupied[12] = {};

		uint n = 11;
		int k = 3;

		while (k >= 0) {
			uint v = cnk(n, (uint)k);
			if (id < v) {
				k--;
				occupied[n] = true;
			} else {
				id = id - v;
			}
			n--;
		}

		int udslice_border = Border_UD_Start;
		int crown_border = Border_Start;

		for (int i = Border_Start; i < Border_End; i++)
			if (occupied[i])
				borders[i].cube_id = udslice_border++;
			else
				borders[i].cube_id = crown_border++;
	}

	template <class ID>
	constexpr ID id_from_data(const Data &data) {
		(void)data;
		return -1;
	}

	template <>
	constexpr ID id_from_data<ID>(const Data &data) {
		ID id = ID();
		id.corners_pos = get_id_corners_pos(data.corners);
		id.corners_rot = get_id_corners_rot(data.corners);
		id.borders_pos = get_id_borders_pos(data.borders);
		id.borders_rot = get_id_borders_rot(data.borders);
		return id;
	}

	template <>
	constexpr IDG1 id_from_data<IDG1>(const Data &data) {
		IDG1 id = IDG1();
		id.corners_rot = get_id_corners_rot(data.corners);
		id.borders_rot = get_id_borders_rot(data.borders);
		id.ud_slice = get_id_udslice(data.borders);
		return id;
	}

	template <>
	constexpr IDG2 id_from_data<IDG2>(const Data &data) {
		IDG2 id = IDG2();
		id.corners_pos = get_id_corners_pos(data.corners);
		id.borders_crown_pos = get_id_borders_crown_pos(data.borders);
		id.borders_ud_pos = get_id_borders_ud_pos(data.borders);
		return id;
	}

	static constexpr void set_data_corners_rot(DataCorners &data, uint id) {
		uint c = 0;
		char rot[] = {0, 2, 1};

		c += data[Corner_URF].rot_id = id % 3;
		id /= 3;
		c += data[Corner_ULF].rot_id = id % 3;
		id /= 3;
		c += data[Corner_ULB].rot_id = id % 3;
		id /= 3;
		c += data[Corner_URB].rot_id = id % 3;
		id /= 3;
		c += data[Corner_DRF].rot_id = id % 3;
		id /= 3;
		c += data[Corner_DLF].rot_id = id % 3;
		id /= 3;
		c += data[Corner_DLB].rot_id = id; // % 3;
		//id /= 3;
		//data[Corner_DRB].rot_id = id; // s % 3));
		data[Corner_DRB].rot_id = rot[c % 3];
	}

	static constexpr void set_data_borders_rot(DataBorders &data, uint id) {
		uint c = 0;

		//deduced from rotation parity
		//c += data[Border_UR].rot_id = (id >> 11);
		c += data[Border_UF].rot_id = (id >> 10) % 2;
		c += data[Border_UL].rot_id = (id >> 9) % 2;
		c += data[Border_UB].rot_id = (id >> 8) % 2;

		c += data[Border_DR].rot_id = (id >> 7) % 2;
		c += data[Border_DF].rot_id = (id >> 6) % 2;
		c += data[Border_DL].rot_id = (id >> 5) % 2;
		c += data[Border_DB].rot_id = (id >> 4) % 2;

		c += data[Border_RF].rot_id = (id >> 3) % 2;
		c += data[Border_FL].rot_id = (id >> 2) % 2;
		c += data[Border_LB].rot_id = (id >> 1) % 2;
		c += data[Border_BR].rot_id = (id >> 0) % 2;

		data[Border_UR].rot_id = c % 2;
	}

	static constexpr inline uchar get_value_fact(uchar c, bool *values) {
		int i = 0;

		while (1) {
			while (values[i])
				i++;
			if (c == 0)
				break;
			c--;
			i++;
		}
		values[i] = true;
		return i;
	}

	static constexpr void set_data_corners_pos(DataCorners &data, uint id) {
		bool values[8] = {};

		data[Corner_URF].cube_id = get_value_fact(id / fact(7) % 8, values);
		data[Corner_ULF].cube_id = get_value_fact(id / fact(6) % 7, values);
		data[Corner_ULB].cube_id = get_value_fact(id / fact(5) % 6, values);
		data[Corner_URB].cube_id = get_value_fact(id / fact(4) % 5, values);
		data[Corner_DRF].cube_id = get_value_fact(id / fact(3) % 4, values);
		data[Corner_DLF].cube_id = get_value_fact(id / fact(2) % 3, values);
		data[Corner_DLB].cube_id = get_value_fact(id / fact(1) % 2, values);
		data[Corner_DRB].cube_id = get_value_fact(id / fact(0) % 1, values);
	}

	static constexpr void set_data_borders_pos(DataBorders &data, uint id) {
		bool values[12] = {};

		data[Border_UR].cube_id = get_value_fact(id / fact(11) % 12, values);
		data[Border_UF].cube_id = get_value_fact(id / fact(10) % 11, values);
		data[Border_UL].cube_id = get_value_fact(id / fact(9) % 10, values);
		data[Border_UB].cube_id = get_value_fact(id / fact(8) % 9, values);

		data[Border_DR].cube_id = get_value_fact(id / fact(7) % 8, values);
		data[Border_DF].cube_id = get_value_fact(id / fact(6) % 7, values);
		data[Border_DL].cube_id = get_value_fact(id / fact(5) % 6, values);
		data[Border_DB].cube_id = get_value_fact(id / fact(4) % 5, values);

		data[Border_RF].cube_id = get_value_fact(id / fact(3) % 4, values);
		data[Border_FL].cube_id = get_value_fact(id / fact(2) % 3, values);
		data[Border_LB].cube_id = get_value_fact(id / fact(1) % 2, values);
		data[Border_BR].cube_id = get_value_fact(id / fact(0) % 1, values);
	}

	static constexpr void set_data_borders_crown_pos(DataBorders &data, uint id) {
		bool values[8] = {};

		data[Border_UR].cube_id = get_value_fact(id / fact(7) % 8, values);
		data[Border_UF].cube_id = get_value_fact(id / fact(6) % 7, values);
		data[Border_UL].cube_id = get_value_fact(id / fact(5) % 6, values);
		data[Border_UB].cube_id = get_value_fact(id / fact(4) % 5, values);

		data[Border_DR].cube_id = get_value_fact(id / fact(3) % 4, values);
		data[Border_DF].cube_id = get_value_fact(id / fact(2) % 3, values);
		data[Border_DL].cube_id = get_value_fact(id / fact(1) % 2, values);
		data[Border_DB].cube_id = get_value_fact(id / fact(0) % 1, values);
	}

	static constexpr void set_data_borders_ud_pos(DataBorders &data, uint id) {
		bool values[4] = {};

		data[Border_RF].cube_id = Border_UD_Start + get_value_fact(id / fact(3) % 4, values);
		data[Border_FL].cube_id = Border_UD_Start + get_value_fact(id / fact(2) % 3, values);
		data[Border_LB].cube_id = Border_UD_Start + get_value_fact(id / fact(1) % 2, values);
		data[Border_BR].cube_id = Border_UD_Start + get_value_fact(id / fact(0) % 1, values);
	}

	constexpr Data data_from_id(const ID id) {
		Data data = Data();
		set_data_corners_pos(data.corners, id.corners_pos);
		set_data_corners_rot(data.corners, id.corners_rot);
		set_data_borders_pos(data.borders, id.borders_pos);
		set_data_borders_rot(data.borders, id.borders_rot);
		return data;
	}

	constexpr Data data_from_id(const IDG1 id) {
		Data data = Data();
		set_data_corners_rot(data.corners, id.corners_rot);
		set_data_borders_rot(data.borders, id.borders_rot);
		set_data_udslice(data.borders, id.ud_slice);
		set_data_corners_pos(data.corners, 0);
		return data;
	}

	constexpr Data data_from_id(const IDG2 id) {
		Data data = Data();
		set_data_corners_pos(data.corners, id.corners_pos);
		set_data_borders_crown_pos(data.borders, id.borders_crown_pos);
		set_data_borders_ud_pos(data.borders, id.borders_ud_pos);
		return data;
	}

	template <class ID>
	constexpr Cube cube_from_id(const ID id) {
		Data d = data_from_id(id);
		return cube_from_data(d);
	}

	const Finder solution_finder = _calculate_finder(cube_from_id(ID()));
	const Data solution_data = data_from_id(ID());
	const Cube solution_cube = cube_from_id(ID());
} // namespace Encoding

/*
previous project to convert to UD_slice coordinates
it worked, but either only one way, or with a range of ((8*9+8)*9+8)*9+8

static constexpr uint16_t get_id_udslice(const DataBorders &borders) {

	char pos[4] = {};
	char choices[4] = {};
	int k = 0;
	int lp = 0;

	for (int n = 0; n < 12; n++)
		if (borders[n].cube_id >= Border_UD_Start) {
			choices[0] = 9 - lp;
			int p = n - k;
			pos[k] = p - lp;
			lp = p;
			k++;
		}

	pos[3] -= pos[2];
	pos[2] -= pos[1];
	pos[1] -= pos[0];

	return (((pos[3] * choices[2] + pos[2]) * choices[1] + pos[1]) * choices[0] + pos[0]);
}

static constexpr void set_data_udslice(DataBorders &borders, uint16_t id) {
	bool occupied[12] = {};

	int ch = 9;
	int lp = 0;

	for (int i = 0; i < 4; i++) {
		int p = ch - 1 - id % ch;
		id /= ch;
		ch -= p;
		p += lp;
		occupied[p + i] = true;
		lp = p;
	}

	int udslice_border = Border_UD_Start;
	int crown_border = Border_Start;

	for (int i = Border_Start; i < Border_End; i++)
		if (occupied[i])
			borders[i].cube_id = udslice_border++;
		else
			borders[i].cube_id = crown_border++;
}
*/