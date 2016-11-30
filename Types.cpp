
#include "Types.hpp"

bool operator==(const Square& sa, const Square& sb) {
	return sa.face_id == sb.face_id;
}
