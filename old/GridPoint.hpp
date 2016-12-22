//
// Created by Louis GARCZYNSKI on 3/4/16.
//

#ifndef NPUZZLE_GRIDPOINT_H
#define NPUZZLE_GRIDPOINT_H

#include <stdlib.h>

class GridPoint {
private:
	int l, c;
public:
	GridPoint();

	GridPoint(int l, int c);

	static GridPoint GetPointFromIndex(int index, int width);

	static int GetIndexFromPoint(const GridPoint &point, int width);

	int ManDistance(const GridPoint &value);
	static int ManDistance(const GridPoint &a, const GridPoint &b);
	static int ManDistance(int a, int b, int width);
};


#endif //NPUZZLE_GRIDPOINT_H
