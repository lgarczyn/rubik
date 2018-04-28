#include "Move.hpp"

std::ostream &operator<<(std::ostream &s, Move c) {
	bool reversed = c.reversed;
	bool halfturn = c.halfturn;

	switch (c.direction) {
	case Move::Left:
		return s << (reversed ? "L'" : (halfturn ? "L2" : "L"));
	case Move::Right:
		return s << (reversed ? "R'" : (halfturn ? "R2" : "R"));
	case Move::Up:
		return s << (reversed ? "U'" : (halfturn ? "U2" : "U"));
	case Move::Down:
		return s << (reversed ? "D'" : (halfturn ? "D2" : "D"));
	case Move::Front:
		return s << (reversed ? "F'" : (halfturn ? "F2" : "F"));
	case Move::Back:
		return s << (reversed ? "B'" : (halfturn ? "B2" : "B"));
	default:
		return s << " ERROR:" << (int)c.direction << ' ';
	}
}