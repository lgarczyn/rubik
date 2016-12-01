
#include "Types.hpp"

bool operator==(const Square& sa, const Square& sb) {
	return sa.face_id == sb.face_id;
}
bool operator==(const Coord& ca, const Coord& cb) {
	return (ca.f == cb.f) && (ca.l == cb.l) && (ca.c == cb.c);
}
