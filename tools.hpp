/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   tools.h											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: edelangh <edelangh@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2016/03/04 19:05:00 by edelangh		  #+#	#+#			 */
/*   Updated: 2016/03/07 19:32:16 by edelangh		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <string>
#include <vector>
#include "State.hpp"

void	print_map(const State& state);
void	print_map(const Cube& state);
void	print_diff(const Cube& state, const Cube& old);
void	print_dist(Coord pos);
