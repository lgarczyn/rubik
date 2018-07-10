/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:40:16 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:28 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include "State.hpp"
#include <string>
#include <unistd.h>

namespace Display {
	void print_map(const IState &state);
	void print_map(const Cube &state);
	void print_diff(const Cube &cube, Move m);
	void print_dist(Coord pos);
	void clear_screen();

	void print_timediff(const char *prefix, const struct timespec &start, const struct timespec &end);


	template <class ID>
	void print_animation(State<ID> state, vector<Move> &movements) {
		Cube cube = state.get_cube();

		for (Move l : movements) {
			Cube new_cube;
			state = state.get_child(l);
			new_cube = state.get_cube();

			clear_screen();
			print_map(cube);
			usleep(700000);

			clear_screen();
			print_diff(cube, l);
			usleep(700000);

			clear_screen();
			print_diff(new_cube, l);
			usleep(700000);

			cube = new_cube;
		}
		clear_screen();
		print_map(cube);
		usleep(1000000);
	}
} // Namespace Display