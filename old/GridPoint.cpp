//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#include "GridPoint.hpp"

GridPoint::GridPoint() : l(0), c(0) {}

GridPoint::GridPoint(int l, int c) : l(l), c(c) {}

GridPoint GridPoint::GetPointFromIndex(int index, int width) {
	return GridPoint(index % width, index / width);
}

int GridPoint::GetIndexFromPoint(const GridPoint &point, int width) {
	return point.c * width + point.l;
}

int GridPoint::ManDistance(const GridPoint &value) {
	return abs(l - value.l) + abs(c - value.c);
}

int GridPoint::ManDistance(const GridPoint &a, const GridPoint &b) {
	return abs(a.l - b.l) + abs(a.c - b.c);
}

int GridPoint::ManDistance(int a, int b, int width) {
	return abs(a % width - b % width) + abs(a / width - b / width);
}
