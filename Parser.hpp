/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Parser.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: edelangh <edelangh@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2016/03/04 14:52:37 by edelangh		  #+#	#+#			 */
/*   Updated: 2016/03/18 16:21:45 by edelangh		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <fstream>
#include "State.hpp"

class	Parser
{
	private:
	public:
		class ParseResult
		{
		public:
			bool		is_random;
			std::string	data;
			int			iteration;
			bool 		forget;
			size_t 		search_step;

			ParseResult();
		};

		Parser() {}
};
