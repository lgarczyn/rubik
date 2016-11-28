#pragma once

using score = long int;
using weighter = score (*)(const std::string& data);

enum Color {
	White,
	Red,
	Blue,
	Orange,
	Green,
	Yellow
};

typedef std::array<std::array<std::array<Color, size>, size>, 6> Data;
