/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State_Const.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:39:17 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/25 03:39:10 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "State.hpp"

const Data State::solution_data = State::data_from_id(ID());
const Cube State::solution_cube = State::cube_from_id(ID());
const Finder State::solution_finder = State::_calculate_finder(solution_cube);
const Color State::solution_colors[] = {White, Green, Red,
    Blue, Orange, Yellow};
