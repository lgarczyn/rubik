/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Parser.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: edelangh <edelangh@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2016/03/04 15:03:37 by edelangh		  #+#	#+#			 */
/*   Updated: 2016/03/18 17:42:10 by edelangh		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Parser.hpp"
#include "tools.hpp"
#include <fstream>
#include <regex>

Parser::ParseResult::ParseResult():is_random(true), data(), iteration(100), forget(), search_step(0){};
