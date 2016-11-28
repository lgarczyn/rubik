/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edelangh <edelangh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/04 19:05:57 by edelangh          #+#    #+#             */
/*   Updated: 2016/03/07 19:39:17 by edelangh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.h"
#include "State.hpp"

/*void	print_map(const std::string& data, const std::string& solution)
{
	int		width = State::width;
	int		height = State::height;

	int		min = get_int_len(width * height);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			size_t index = x + y * width;
			uint16_t c = data.at(index);
			std::string val = std::to_string(c);

			if (c == 0)
				val = "\x1B[44m" + val + " ";
			else
				val = "\x1B[40m" + val + " ";
			if (solution.find(c) == index)
				val = "\x1B[32m" + val;
			else
				val = "\x1B[37m" + val;

			std::cout << val << std::string(min - get_int_len(c), ' ') << "\x1B[0m ";
		}
		std::cout << std::endl << std::flush;
	}
}*/
