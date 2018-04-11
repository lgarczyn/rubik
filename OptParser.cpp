/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:34:19 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:10 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <string>

char *get_cmd_opt(char **begin, uint count, const std::string &option) {
	for (uint i = 0; i < count; i++) {
		if (!option.compare(begin[i]) && i < count - 1)
			return (begin[i + 1]);
	}
	return NULL;
}

bool is_cmd_opt(char **begin, uint count, const std::string &option) {
	for (uint i = 0; i < count; i++) {
		if (!option.compare(begin[i]))
			return (true);
	}
	return false;
}
