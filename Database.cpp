#include "Database.hpp"

std::ostream &operator<<(std::ostream& s, Database& db) {
    s.write((char*)&(db.data[0]), db.data.size());
    return s;
}
std::istream &operator>>(std::istream& s, Database& db) {
    s.read((char*)&(db.data[0]), db.data.size());
    return s;
}
