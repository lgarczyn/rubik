#include <iostream>
#include "Types.hpp"

template <int length>
class Database{

public:
    Database():data() {

    }

    std::array<uchar, length> data;
};

template <int length>
std::ostream &operator<<(std::ostream& s, Database<length>& db) {
    s.write((char*)&(db.data[0]), length);
    return s;
}
template <int length>
std::istream &operator>>(std::istream& s, Database<length>& db) {
    s.read((char*)&(db.data[0]), length);
    return s;
}
