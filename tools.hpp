/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 19:05:00 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/07 19:32:16 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include "State.hpp"

void 	split(const std::string& str,
		std::vector<std::string>& tokens,
		const std::string& delimiters = " ");
bool 	is_number(const std::string& s);
void	print_map(const State& state);
void	print_map(const Cube& state);
void	print_dist(Coord pos);
