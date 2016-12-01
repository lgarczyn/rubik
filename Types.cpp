
#include "Types.hpp"
#include <iostream>

bool operator==(const Square& sa, const Square& sb) {
	return sa.face_id == sb.face_id;
}
bool operator==(const Coord& ca, const Coord& cb) {
	return (ca.f == cb.f) && (ca.l == cb.l) && (ca.c == cb.c);
}

std::ostream& operator<<(std::ostream& s, const Coord& c) {
	s << '{' << c.f << ',' << c.l << ',' << c.c << '}';
	return s;
}
