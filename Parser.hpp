/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:44:52 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/16 01:23:59 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Move.hpp"
#include "Types.hpp"
#include <cstdlib>
#include <fstream>

class Parser {
  private:
  public:
	class ParseResult {
	  public:
		bool is_random;
		string data;
		int iteration;

		int clean_steps;

		size_t search_step;
		ParseResult();

		vector<Move> get_data() const;
	};

	static ParseResult parse_args(unsigned int ac, char **av);

	Parser() {}
};
