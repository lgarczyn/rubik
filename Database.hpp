/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:23 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/09 22:45:12 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <memory>
#include "Types.hpp"

#pragma once

class Database {
   public:
    Database() : _data(nullptr), _len(0) {}
    Database(size_t length)
	: _data((uchar *)calloc(length, sizeof(uchar))), _len(length) {}

    size_t size() { return _len; }
    uchar &operator[](size_t i) { return _data.get()[i]; }

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
}
