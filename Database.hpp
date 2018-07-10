/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:23 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/20 21:02:55 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Types.hpp"
#include <iostream>
#include <memory>
#include <stdlib.h>

#pragma once

class Database {
  public:
	Database() : _data(nullptr), _len(0) {}
	Database(size_t length)
	    : _data((uchar *)calloc(length, sizeof(uchar))), _len(length) {}

	size_t size() { return _len; }
	uchar &operator[](size_t i) { return _data.get()[i]; }

	void store(uint id_corners, int len, bool is_first);
	bool exist(uint id_corners);

  private:
	std::shared_ptr<uchar> _data;
	size_t _len;
};

std::ostream &operator<<(std::ostream &s, Database &db);
std::istream &operator>>(std::istream &s, Database &db);

namespace Databases {
	extern Database upper_corners;
	extern Database lower_corners;
	extern Database corners;

	extern int total_saved;
	extern int total_overridden;
	extern int total_saved_first;
	extern int total_overridden_first;
} // namespace Databases
