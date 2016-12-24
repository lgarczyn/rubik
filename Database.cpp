#include "Database.hpp"

std::ostream &operator<<(std::ostream& s, Database& db) {
	s.write((char*)&(db[0]), db.size());
	return s;
}
std::istream &operator>>(std::istream& s, Database& db) {
	s.read((char*)&(db[0]), db.size());
	return s;
}

Database Databases::upper_corners;
Database Databases::lower_corners;
Database Databases::corners;
