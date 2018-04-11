/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:40:28 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:29 by lgarczyn         ###   ########.fr       */
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
	return (a.corners == b.corners) && (a.borders_pos == b.borders_pos) &&
	       (a.borders_rot == b.borders_rot);
}

constexpr bool operator!=(const Square &a, const Square &b) {
	// return a.face_id != b.face_id;//TODO once sure of deflate/inflate,
	// uncomment
	return (a.cube_id != b.cube_id) || (a.rot_id != b.rot_id);
}

constexpr bool operator!=(const Coord &ca, const Coord &cb) {
	return (ca.f != cb.f) || (ca.l != cb.l) || (ca.c != cb.c);
}

constexpr bool operator!=(const ID &a, const ID &b) {
	return (a.corners != b.corners) || (a.borders_pos != b.borders_pos) ||
	       (a.borders_rot != b.borders_rot);
}

inline std::ostream &operator<<(std::ostream &s, const Coord &c) {
	s << '{' << c.f << ',' << c.l << ',' << c.c << '}';
	return s;
}
