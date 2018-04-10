/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:44:52 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/09 23:06:22 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <fstream>
#include "State.hpp"

class Parser {
   private:
   public:
    class ParseResult {
       public:
	bool is_random;
	std::string data;
	int iteration;
	bool forget;
	size_t search_step;

	ParseResult();
    };

    Parser() {}
};
