/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:35:33 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/13 21:18:46 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "State.hpp"
#include "Types.hpp"
#include <algorithm>
#include <string>

using BufferCorner = std::array<std::array<std::array<uchar, 4>, 8>, 8>;
using BufferBorder = std::array<std::array<std::array<uchar, 2>, 12>, 12>;

namespace Heuristics {
	constexpr Score ValidFunction(const Data &data);
	constexpr Score ValidFunctionG1(const Data &data);
	constexpr Score ValidFunctionG2(const Data &data);
	constexpr Score CornerFunction(const Data &data);
	constexpr Score BorderFunction(const Data &data);
	Score DatabaseFunction(const ID &);
	Score DatabaseFunction(const IDG1 &);
	Score DatabaseFunction(const IDG2 &);

	constexpr int SquareDistance(int uid_a, int uid_b);
	constexpr int SquareDistance(Coord a, Coord b);
	constexpr BufferCorner get_dist_table_corners();
	constexpr BufferBorder get_dist_table_borders();
	const BufferCorner dist_table_corners = get_dist_table_corners();
	const BufferBorder dist_table_borders = get_dist_table_borders();
}; // namespace Heuristics