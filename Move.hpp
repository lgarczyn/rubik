#pragma once
#include <functional>
#include <iostream>

class Move {
  public:
	enum Direction {
		None = 0,
		Up = 1,
		Right = 2,
		Down = 3,
		Left = 4,
		Front = 5,
		Back = 6,

		Direction_Start = Up,
		Direction_End = Back,
	};

	Direction direction : 3;
	bool reversed : 1;
	bool halfturn : 1;

	constexpr char get_turns() const {
		if (reversed)
			return 3;
		if (halfturn)
			return 2;
		return 1;
	}
	static constexpr bool is_commutative(Direction a, Direction b) {
		if (a == Down && b == Up)
			return true;
		if (a == Back && b == Front)
			return true;
		if (a == Left && b == Right)
			return true;
		return false;
	}
	static constexpr bool is_commutative(int a, int b) {
		return is_commutative((Direction)a, (Direction)b);
	}
	constexpr Move inverse() const {
		return Move(direction, 4 - get_turns());
	}
	constexpr Move() : direction(None), reversed(false), halfturn(false){};
	constexpr Move(Direction d) : direction(d), reversed(false), halfturn(false){};
	constexpr Move(Direction d, int turns) : Move(d) {
		if (turns == 2)
			halfturn = true;
		else if (turns == 3)
			reversed = true;
	};
	constexpr Move(std::pair<char, char> p) : Move() {
		reversed = p.second == '\'';
		halfturn = p.second == '2';

		switch (p.first) {
		case 'F':
			direction = Move::Front;
			break;
		case 'R':
			direction = Move::Right;
			break;
		case 'U':
			direction = Move::Up;
			break;
		case 'B':
			direction = Move::Back;
			break;
		case 'L':
			direction = Move::Left;
			break;
		case 'D':
			direction = Move::Down;
			break;
		default:
			direction = Move::None;
		}
	}
};

std::ostream &operator<<(std::ostream &s, const Move m);
