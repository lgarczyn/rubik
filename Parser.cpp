/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:45:02 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/09 21:45:03 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <fstream>
#include <regex>
#include "tools.hpp"

Parser::ParseResult::ParseResult()
    : is_random(true), data(), iteration(100), forget(), search_step(0){};
