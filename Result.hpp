#pragma once
#include "State.hpp"

class Result {
  public:
	int timeComplexity;
	int sizeComplexity;
	Cube cube;
	Score weight;
	vector<Move> movements;
	bool finished;

	Result()
	    : timeComplexity(0),
	      sizeComplexity(0),
	      cube(),
	      weight(),
	      movements(),
	      finished(false) {}

	Result(int tc, int sc)
	    : Result() {
		timeComplexity = tc;
		sizeComplexity = sc;
	}

	Result(const Result &a, const Result &b)
	    : Result() {
		timeComplexity = a.timeComplexity + b.timeComplexity;
		sizeComplexity = std::max(a.sizeComplexity, b.sizeComplexity);
		cube = b.cube;
		weight = 0;
		finished = b.finished;

		movements.reserve(a.movements.size() + b.movements.size()); // preallocate memory
		movements.insert(movements.end(), a.movements.begin(), a.movements.end());
		movements.insert(movements.end(), b.movements.begin(), b.movements.end());
	}
};