/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:40:28 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/20 22:25:55 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Types.hpp"
#include <cassert>
#include <iostream>
#include <utility>

constexpr Square::Square(uchar cid, uchar rid) : cube_id(cid), rot_id(rid) {}
constexpr Square::Square() : cube_id(0), rot_id(0) {}

constexpr uchar Square::get_uid(int c, int l) const {
	if (c == 1 && l == 1)
		return get_uid(st_center);
	if (c == 1 || l == 1)
		return get_uid(st_border);
	return get_uid(st_corner);
}

constexpr uchar Square::get_uid(SquareType type) const {
	return get_uid(cube_id, rot_id, type);
}

constexpr uchar Square::get_uid(int cube_id, int rot_id, SquareType type) {
	if (type == st_corner)
		return cube_id * 3 + rot_id;
	if (type == st_border)
		return max_uid_corner + cube_id * 2 + rot_id;
	return max_uid_border + cube_id;
}

constexpr pair<Square, SquareType> Square::get_square(uchar uid) {
	assert(uid < max_uid);
	if (uid >= max_uid_border) {
		uid -= max_uid_border;
		return std::make_pair(Square(uid, 0), st_center);
	}
	if (uid >= max_uid_corner) {
		uid -= max_uid_corner;
		return std::make_pair(Square(uid / 2, uid % 2), st_border);
	}
	return std::make_pair(Square(uid / 3, (uchar)uid % 3), st_corner);
}

constexpr bool operator==(const Square &a, const Square &b) {
	return (a.cube_id == b.cube_id) && (a.rot_id == b.rot_id);
	// return a.face_id == b.face_id;
}

constexpr bool operator==(const Coord &ca, const Coord &cb) {
	return (ca.f == cb.f) && (ca.l == cb.l) && (ca.c == cb.c);
}

constexpr bool operator==(const ID &a, const ID &b) {
	return (a.corners_pos == b.corners_pos) && (a.corners_rot == b.corners_rot) &&
	       (a.borders_pos == b.borders_pos) && (a.borders_rot == b.borders_rot);
}

constexpr bool operator==(const IDG1 &a, const IDG1 &b) {
	return (a.corners_rot == b.corners_rot) &&
	       (a.borders_rot == b.borders_rot) &&
	       (a.ud_slice == b.ud_slice);
}

constexpr bool operator==(const IDG2 &a, const IDG2 &b) {
	return (a.corners_pos == b.corners_pos) &&
	       (a.borders_crown_pos == b.borders_crown_pos) &&
	       (a.borders_ud_pos == b.borders_ud_pos);
}

constexpr bool operator!=(const Square &a, const Square &b) {
	return (a.cube_id != b.cube_id) || (a.rot_id != b.rot_id);
}

constexpr bool operator!=(const Coord &ca, const Coord &cb) {
	return (ca.f != cb.f) || (ca.l != cb.l) || (ca.c != cb.c);
}

constexpr bool operator!=(const ID &a, const ID &b) {
	return (a.corners_pos != b.corners_pos) || (a.corners_rot != b.corners_rot) ||
	       (a.borders_pos != b.borders_pos) || (a.borders_rot != b.borders_rot);
}

constexpr bool operator!=(const IDG1 &a, const IDG1 &b) {
	return (a.corners_rot != b.corners_rot) ||
	       (a.borders_rot != b.borders_rot) ||
	       (a.ud_slice != b.ud_slice);
}

constexpr bool operator!=(const IDG2 &a, const IDG2 &b) {
	return (a.corners_pos != b.corners_pos) ||
	       (a.borders_crown_pos != b.borders_crown_pos) ||
	       (a.borders_ud_pos != b.borders_ud_pos);
}

constexpr bool operator<(const ID &la, const ID &ra) {
	if (la.corners_pos != ra.corners_pos)
		return la.corners_pos < ra.corners_pos;
	if (la.corners_rot != ra.corners_rot)
		return la.corners_rot < ra.corners_rot;
	if (la.borders_pos != ra.borders_pos)
		return la.borders_pos < ra.borders_pos;
	return la.borders_rot < ra.borders_rot;
}

constexpr bool operator<(const IDG1 &la, const IDG1 &ra) {
	if (la.corners_rot != ra.corners_rot)
		return la.corners_rot < ra.corners_rot;
	if (la.borders_rot != ra.borders_rot)
		return la.borders_rot < ra.borders_rot;
	return la.ud_slice < ra.ud_slice;
}

constexpr bool operator<(const IDG2 &la, const IDG2 &ra) {
	if (la.corners_pos != ra.corners_pos)
		return la.corners_pos < ra.corners_pos;
	if (la.borders_crown_pos != ra.borders_crown_pos)
		return la.borders_crown_pos < ra.borders_crown_pos;
	return la.borders_ud_pos < ra.borders_ud_pos;
}

constexpr size_t id_hash(const ID &id) noexcept {
	return (id.borders_rot ^ id.borders_pos) |
	       ((uint64_t)id.corners_rot << 32) |
	       ((uint64_t)id.corners_pos << 48);
}

constexpr size_t id_hash(const IDG1 &id) noexcept {
	//TODO: check if it translates to a reinterpret_cast
	return ((uint)id.corners_rot |
	        ((uint)id.borders_rot << 11) |
	        (uint)id.ud_slice << (11 + 12));
};

constexpr size_t id_hash(const IDG2 &id) noexcept {
	return (id.corners_pos |
	        ((uint)id.borders_crown_pos << 16) |
	        ((size_t)id.borders_ud_pos << 32));
};

inline std::ostream &operator<<(std::ostream &s, const Coord &c) {
	s << '{' << c.f << ',' << c.l << ',' << c.c << '}';
	return s;
}
