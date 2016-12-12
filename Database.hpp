#include <iostream>
#include <vector>
#include "Types.hpp"

class Database{

public:

    Database(){}
    Database(int length):data(length) {}

    std::vector<uchar> data;
};
std::ostream &operator<<(std::ostream& s, Database& db);
std::istream &operator>>(std::istream& s, Database& db);

namespace Databases{
    static Database upper_corners;
    static Database lower_corners;
}
