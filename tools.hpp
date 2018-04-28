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

#include "State.hpp"
#include <string>

void print_map(const State &state);
void print_map(const Cube &state);
void print_diff(const Cube &state, const Cube &old);
void print_dist(Coord pos);
void print_animation(State state, vector<Move> &movements);
void clear_screen();

void print_timediff(const char *prefix, const struct timespec &start, const struct timespec &end);