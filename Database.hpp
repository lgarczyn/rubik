#include <iostream>
#include <vector>
#include "Types.hpp"

class Database:public std::vector<uchar>{

public:

    Database(){}
    Database(int length):std::vector<uchar>(length) {}
};
std::ostream &operator<<(std::ostream& s, Database& db);
std::istream &operator>>(std::istream& s, Database& db);

namespace Databases{
    extern Database upper_corners;
    extern Database lower_corners;
    extern Database corners;
    extern uint current_index;
}
