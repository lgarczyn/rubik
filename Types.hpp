#pragma once

enum Color {
	White = 0,
	Red = 1,
	Blue = 2,
	Orange = 4,
	Green = 5,
	Yellow = 6
};

 //SI CHANGE
 // CHANGER DE IMPAIRE A PAIRE CASSE TOUT
 // RECODER TOUT LE SYSTEME DE TRANSFORMATION AUSSI
static const int size = 3;
static const int center = (size / 2);

static const int colorsCount = 6;

using Column = std::array<Color, size>;
using Face = std::array<Column, size>;
using Data = std::array<Face, 6>;

using score = long int;
using weighter = score (*)(const Data& data);
using uchar = unsigned char;

#include <string>
using string = std::string;
