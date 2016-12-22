

/*constexpr Cube		State::_calculate_solution() {
	Cube			cube = Cube();

	int uid = 0;
	for (int s = Index_Start; s < Index_End; s++)
		for (int l = 0; l < size; l++)
			for (int c = 0; c < size; c++)
				uid++;

	//Adding corners ID
	cube[Index_U][0][0].cube_id = cube[Index_L][0][0].cube_id = cube[Index_B][0][2].cube_id = 0;
	cube[Index_U][0][2].cube_id = cube[Index_B][0][0].cube_id = cube[Index_R][0][2].cube_id = 1;
	cube[Index_U][2][0].cube_id = cube[Index_F][0][0].cube_id = cube[Index_L][0][2].cube_id = 2;
	cube[Index_U][2][2].cube_id = cube[Index_R][0][0].cube_id = cube[Index_F][0][2].cube_id = 3;
	cube[Index_D][0][0].cube_id = cube[Index_L][2][2].cube_id = cube[Index_F][2][0].cube_id = 4;
	cube[Index_D][0][2].cube_id = cube[Index_F][2][2].cube_id = cube[Index_R][2][0].cube_id = 5;
	cube[Index_D][2][0].cube_id = cube[Index_B][2][2].cube_id = cube[Index_L][2][0].cube_id = 6;
	cube[Index_D][2][2].cube_id = cube[Index_R][2][2].cube_id = cube[Index_B][2][0].cube_id = 7;

	cube[Index_U][0][0].rot_id = 0; cube[Index_L][0][0].rot_id = 1; cube[Index_B][0][2].rot_id = 2;
	cube[Index_U][0][2].rot_id = 0; cube[Index_B][0][0].rot_id = 1; cube[Index_R][0][2].rot_id = 2;
	cube[Index_U][2][0].rot_id = 0; cube[Index_F][0][0].rot_id = 1; cube[Index_L][0][2].rot_id = 2;
	cube[Index_U][2][2].rot_id = 0; cube[Index_R][0][0].rot_id = 1; cube[Index_F][0][2].rot_id = 2;
	cube[Index_D][0][0].rot_id = 0; cube[Index_L][2][2].rot_id = 1; cube[Index_F][2][0].rot_id = 2;
	cube[Index_D][0][2].rot_id = 0; cube[Index_F][2][2].rot_id = 1; cube[Index_R][2][0].rot_id = 2;
	cube[Index_D][2][0].rot_id = 0; cube[Index_B][2][2].rot_id = 1; cube[Index_L][2][0].rot_id = 2;
	cube[Index_D][2][2].rot_id = 0; cube[Index_R][2][2].rot_id = 1; cube[Index_B][2][0].rot_id = 2;

	//Adding borders ID
	cube[Index_U][0][1].cube_id = cube[Index_B][0][1].cube_id = 0;
	cube[Index_U][1][0].cube_id = cube[Index_L][0][1].cube_id = 1;
	cube[Index_U][1][2].cube_id = cube[Index_R][0][1].cube_id = 2;
	cube[Index_U][2][1].cube_id = cube[Index_F][0][1].cube_id = 3;

	cube[Index_R][1][0].cube_id = cube[Index_F][1][2].cube_id = 4;
	cube[Index_B][1][0].cube_id = cube[Index_R][1][2].cube_id = 5;
	cube[Index_L][1][0].cube_id = cube[Index_B][1][2].cube_id = 6;
	cube[Index_F][1][0].cube_id = cube[Index_L][1][2].cube_id = 7;

	cube[Index_D][0][1].cube_id = cube[Index_F][2][1].cube_id = 8;
	cube[Index_D][1][0].cube_id = cube[Index_L][2][1].cube_id = 9;
	cube[Index_D][1][2].cube_id = cube[Index_R][2][1].cube_id = 10;
	cube[Index_D][2][1].cube_id = cube[Index_B][2][1].cube_id = 11;

	cube[Index_U][0][1].rot_id = 0; cube[Index_B][0][1].rot_id = 1;
	cube[Index_U][1][0].rot_id = 0; cube[Index_L][0][1].rot_id = 1;
	cube[Index_U][1][2].rot_id = 0; cube[Index_R][0][1].rot_id = 1;
	cube[Index_U][2][1].rot_id = 0; cube[Index_F][0][1].rot_id = 1;

	cube[Index_D][0][1].rot_id = 0; cube[Index_F][2][1].rot_id = 1;
	cube[Index_D][1][0].rot_id = 0; cube[Index_L][2][1].rot_id = 1;
	cube[Index_D][1][2].rot_id = 0; cube[Index_R][2][1].rot_id = 1;
	cube[Index_D][2][1].rot_id = 0; cube[Index_B][2][1].rot_id = 1;

	cube[Index_R][1][0].rot_id = 0; cube[Index_F][1][2].rot_id = 1;
	cube[Index_B][1][0].rot_id = 0; cube[Index_R][1][2].rot_id = 1;
	cube[Index_L][1][0].rot_id = 0; cube[Index_B][1][2].rot_id = 1;
	cube[Index_F][1][0].rot_id = 0; cube[Index_L][1][2].rot_id = 1;

	//Adding center ID
	cube[Index_U][1][1].cube_id = 0;
	cube[Index_F][1][1].cube_id = 1;
	cube[Index_R][1][1].cube_id = 2;
	cube[Index_B][1][1].cube_id = 3;
	cube[Index_L][1][1].cube_id = 4;
	cube[Index_D][1][1].cube_id = 5;

	cube[Index_U][1][1].rot_id = 0;
	cube[Index_F][1][1].rot_id = 0;
	cube[Index_R][1][1].rot_id = 0;
	cube[Index_B][1][1].rot_id = 0;
	cube[Index_L][1][1].rot_id = 0;
	cube[Index_D][1][1].rot_id = 0;

	return cube;
}*/

/*

void State::_apply_scramble(Cube& cube, const string& scramble) {

	std::stringstream ss = std::stringstream(scramble);

	while (ss) {
		Movement m;
		int c = ss.get();

		switch (c) {
			case 'F': m = Front; break;
			case 'R': m = Right; break;
			case 'U': m = Up; break;
			case 'B': m = Back; break;
			case 'L': m = Left; break;
			case 'D': m = Down; break;
			default: continue;
		}
		c = ss.peek();
		switch (c) {
			case '\'': m = (Movement)(m | Reversed); break;
			case '2': m = (Movement)(m | Halfturn); break;
		}
		_apply_movement(cube, m);
	}
}

void swap_s(Square& a, Square& b, Square& c, Square& d, int turns) {
	Square t;
	switch (turns) {
		case 1:

			t = d;
			d = c;
			c = b;
			b = a;
			a = t;
			break;
		case 2:
			std::swap(a, c);
			std::swap(b, d);
			break;
		case 3:
			t = a;
			a = b;
			b = c;
			c = d;
			d = t;
			break;
	}
}

void rotate_face(Face& face, int turns) {

	swap_s(face[0][2], face[2][2], face[2][0], face[0][0], turns);
	swap_s(face[1][2], face[2][1], face[1][0], face[0][1], turns);
}

void State::_apply_movement(Cube& cube, Movement m) {

	std::cerr << "applying movements " << m << std::endl;
	bool reversed = m & Reversed;
	bool halfturn = m & Halfturn;
	int turns = reversed ? 3 : (halfturn ? 2 : 1);
	switch (m & Mask) {
		case None : return;
		case Up :
			rotate_face(cube[Index_U], turns);
			 //Crown is front, left, back, right
			swap_s(cube[Index_F][0][0], cube[Index_L][0][0], cube[Index_B][0][0], cube[Index_R][0][0], turns);
			swap_s(cube[Index_F][0][1], cube[Index_L][0][1], cube[Index_B][0][1], cube[Index_R][0][1], turns);
			swap_s(cube[Index_F][0][2], cube[Index_L][0][2], cube[Index_B][0][2], cube[Index_R][0][2], turns);
			break;
		case Front :
			rotate_face(cube[Index_F], turns);
			 //Crown is up, right, down, left
			swap_s(cube[Index_U][2][0], cube[Index_R][0][0], cube[Index_D][0][2], cube[Index_L][2][2], turns);
			swap_s(cube[Index_U][2][1], cube[Index_R][1][0], cube[Index_D][0][1], cube[Index_L][1][2], turns);
			swap_s(cube[Index_U][2][2], cube[Index_R][2][0], cube[Index_D][0][0], cube[Index_L][0][2], turns);
			break;
		case Right :
			rotate_face(cube[Index_R], turns);
			 //Crown is up, back, down, front
			swap_s(cube[Index_U][2][2], cube[Index_B][0][0], cube[Index_D][2][2], cube[Index_F][2][2], turns);
			swap_s(cube[Index_U][1][2], cube[Index_B][1][0], cube[Index_D][1][2], cube[Index_F][1][2], turns);
			swap_s(cube[Index_U][0][2], cube[Index_B][2][0], cube[Index_D][0][2], cube[Index_F][0][2], turns);
			break;
		case Back :
			rotate_face(cube[Index_B], turns);
			 //Crown is up, left, down, right
			swap_s(cube[Index_U][0][0], cube[Index_L][2][0], cube[Index_D][2][2], cube[Index_R][0][2], turns);
			swap_s(cube[Index_U][0][1], cube[Index_L][1][0], cube[Index_D][2][1], cube[Index_R][1][2], turns);
			swap_s(cube[Index_U][0][2], cube[Index_L][0][0], cube[Index_D][2][0], cube[Index_R][2][2], turns);
			break;
		case Left :
			rotate_face(cube[Index_L], turns);
			 //Crown is up, front, down, back
			swap_s(cube[Index_U][0][0], cube[Index_F][0][0], cube[Index_D][0][0], cube[Index_B][2][2], turns);
			swap_s(cube[Index_U][1][0], cube[Index_F][1][0], cube[Index_D][1][0], cube[Index_B][1][2], turns);
			swap_s(cube[Index_U][2][0], cube[Index_F][2][0], cube[Index_D][2][0], cube[Index_B][0][2], turns);
			break;
		case Down :
			rotate_face(cube[Index_D], turns);
 			 //Crown is front, right, back, left
			swap_s(cube[Index_F][2][0], cube[Index_R][2][0], cube[Index_B][2][0], cube[Index_L][2][0], turns);
			swap_s(cube[Index_F][2][1], cube[Index_R][2][1], cube[Index_B][2][1], cube[Index_L][2][1], turns);
			swap_s(cube[Index_F][2][2], cube[Index_R][2][2], cube[Index_B][2][2], cube[Index_L][2][2], turns);
			break;
		default: break;
	}
}


bool State::check_continuity() const{
	int id;

	id = _cube[Index_U][0][0].cube_id;
	if (id != _cube[Index_L][0][0].cube_id || id != _cube[Index_B][0][2].cube_id)
		return false;
	id = _cube[Index_U][2][0].cube_id;
	if (id != _cube[Index_L][0][2].cube_id || id != _cube[Index_F][0][0].cube_id)
		return false;
	id = _cube[Index_U][0][2].cube_id;
	if (id != _cube[Index_R][0][2].cube_id|| id != _cube[Index_B][0][0].cube_id)
		return false;
	id = _cube[Index_U][2][2].cube_id;
	if (id != _cube[Index_R][0][0].cube_id|| id != _cube[Index_F][0][2].cube_id)
		return false;

	id = _cube[Index_D][0][0].cube_id;
	if (id != _cube[Index_L][2][2].cube_id || id != _cube[Index_F][2][0].cube_id)
		return false;
	id = _cube[Index_D][2][0].cube_id;
	if (id != _cube[Index_L][2][0].cube_id || id != _cube[Index_B][2][2].cube_id)
		return false;
	id = _cube[Index_D][0][2].cube_id;
	if (id != _cube[Index_R][2][0].cube_id|| id != _cube[Index_F][2][2].cube_id)
		return false;
	id = _cube[Index_D][2][2].cube_id;
	if (id != _cube[Index_R][2][2].cube_id|| id != _cube[Index_B][2][0].cube_id)
		return false;

	//Adding borders ID
	if (_cube[Index_U][0][1].cube_id != _cube[Index_B][0][1].cube_id)
		return false;
	if (_cube[Index_U][1][0].cube_id != _cube[Index_L][0][1].cube_id)
		return false;
	if (_cube[Index_U][2][1].cube_id != _cube[Index_F][0][1].cube_id)
		return false;
	if (_cube[Index_U][1][2].cube_id != _cube[Index_R][0][1].cube_id)
		return false;

	if (_cube[Index_D][0][1].cube_id != _cube[Index_F][2][1].cube_id)
		return false;
	if (_cube[Index_D][1][0].cube_id != _cube[Index_L][2][1].cube_id)
		return false;
	if (_cube[Index_D][2][1].cube_id != _cube[Index_B][2][1].cube_id)
		return false;
	if (_cube[Index_D][1][2].cube_id != _cube[Index_R][2][1].cube_id)
		return false;

	if (_cube[Index_F][1][2].cube_id != _cube[Index_R][1][0].cube_id)
		return false;
	if (_cube[Index_R][1][2].cube_id != _cube[Index_B][1][0].cube_id)
		return false;
	if (_cube[Index_B][1][2].cube_id != _cube[Index_L][1][0].cube_id)
		return false;
	if (_cube[Index_L][1][2].cube_id != _cube[Index_F][1][0].cube_id)
		return false;
	return true;
}*/
/**/
