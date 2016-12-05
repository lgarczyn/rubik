
#include "Types.hpp"
#include <iostream>

bool operator==(const Square& a, const Square& b) {
	return (a.face_id == b.face_id) && (a.cube_id == b.cube_id) && (a.color == b.color);
	//return a.face_id == b.face_id;
}
bool operator==(const Coord& ca, const Coord& cb) {
	return (ca.f == cb.f) && (ca.l == cb.l) && (ca.c == cb.c);
}
bool operator==(const ID& a, const ID& b) {
	return (a.corners == b.corners) && (a.borders_pos == b.borders_pos) && (a.borders_rot == b.borders_rot);
}

bool operator!=(const Square& a, const Square& b) {
	return (a.face_id != b.face_id) || (a.cube_id != b.cube_id) || (a.color != b.color);
	//return a.face_id != b.face_id;//TODO check if correct
}
bool operator!=(const Coord& ca, const Coord& cb) {
	return (ca.f != cb.f) || (ca.l != cb.l) || (ca.c != cb.c);
}
bool operator!=(const ID& a, const ID& b) {
	return (a.corners != b.corners) || (a.borders_pos != b.borders_pos) || (a.borders_rot != b.borders_rot);
}


std::ostream& operator<<(std::ostream& s, const Coord& c) {
	s << '{' << c.f << ',' << c.l << ',' << c.c << '}';
	return s;
}
