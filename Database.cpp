/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:34:34 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/20 21:05:10 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Database.hpp"

std::ostream &operator<<(std::ostream &s, Database &db) {
	s.write((char *)&(db[0]), db.size());
	return s;
}

std::istream &operator>>(std::istream &s, Database &db) {
	s.read((char *)&(db[0]), db.size());
	return s;
}

void Database::store(uint id_corners, int len, bool is_first) {
	uint id = id_corners;
	if (!is_first)
		if (operator[](id) > 0)
			Databases::total_overridden++;
		else
			Databases::total_saved++;
	else if (operator[](id) > 0)
		Databases::total_overridden_first++;
	else
		Databases::total_saved_first++;
	operator[](id) = len;
}

bool Database::exist(uint id) {
	// id = Encoding::floor_index_upper_corners(id);
	if (id == 0)
		return true;
	if (operator[](id) > 0)
		return true;
	return false;
}

Database Databases::upper_corners;
Database Databases::lower_corners;
Database Databases::corners;

int Databases::total_saved;
int Databases::total_overridden;
int Databases::total_saved_first;
int Databases::total_overridden_first;
