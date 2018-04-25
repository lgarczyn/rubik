/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:45:02 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/17 00:20:41 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "tools.hpp"
#include <fstream>
#include <regex>

Parser::ParseResult::ParseResult()
    : is_random(true), data(), iteration(100), clean_steps(), search_step(0){};
