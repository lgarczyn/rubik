/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:34:34 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/11 20:15:04 by lgarczyn         ###   ########.fr       */
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

Database Databases::upper_corners;
Database Databases::lower_corners;
Database Databases::corners;
