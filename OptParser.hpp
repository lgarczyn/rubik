/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:34:06 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/09 23:06:21 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

bool is_cmd_opt(char** data, unsigned int len, const std::string& option);
char* get_cmd_opt(char** data, unsigned int len, const std::string& option);
