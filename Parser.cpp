/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgarczyn <lgarczyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:45:02 by lgarczyn          #+#    #+#             */
/*   Updated: 2018/04/17 00:20:41 by lgarczyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cstdlib>
#include <fstream>
#include <random>
#include <regex>
#include <vector>

Parser::ParseResult::ParseResult()
    : is_random(true), data(), iteration(100), clean_steps(), search_step(0){};

vector<Move> parse_data_arg(const std::string &s) {
	std::stringstream ss = std::stringstream(s);
	vector<Move> r;

	while (ss) {
		char c = ss.get();
		Move m(std::make_pair(c, ss.peek()));

		if (m.direction)
			r.push_back(m);
	}
	return r;
}

vector<Move> get_random_scramble(int scramble_count) {
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(Move::Direction_Start, Move::Direction_End);
	std::uniform_int_distribution<int> turns(1, 3);

	std::cout << "GENERATING CUBE WITH PATTERN:" << std::endl;

	vector<Move> moves;

	int previous = Move::None;
	for (int i = 0; i < scramble_count; i++) {
		int m = uni(rng);
		while (m == previous || Move::is_commutative(previous, m))
			m = uni(rng);
		previous = m;
		int t = turns(rng);
		Move move = Move((Move::Direction)m, t);

		moves.push_back(move);

		std::cout << move;
	}
	std::cout << std::endl;
	return moves;
}

vector<Move> Parser::ParseResult::get_data() const {
	if (is_random == false)
		return parse_data_arg(data);
	return get_random_scramble(iteration);
}

int display_help(const char *path = "npuzzle") {
	std::cout << "Usage: " << path << " [-h] " << std::endl
	          << "1: [-i ITERATION]" << std::endl
	          << "2: [-m MOVEMENTS]" << std::endl
	          << "[-c CLEAN_STEPS]" << std::endl
	          << "[-f1] [-f2] [-f3]" << std::endl
	          << "[--greedy] [--uniform]" << std::endl;
	return 0;
}

char *get_cmd_opt(char **begin, uint count, const std::string &option) {
	for (uint i = 0; i < count; i++) {
		if (!option.compare(begin[i]) && i < count - 1)
			return begin[i + 1];
	}
	return NULL;
}

bool is_cmd_opt(char **begin, uint count, const std::string &option) {
	for (uint i = 0; i < count; i++) {
		if (!option.compare(begin[i]))
			return true;
	}
	return false;
}

Parser::ParseResult Parser::parse_args(unsigned int ac, char **av) {
	Parser::ParseResult result;
	try {
		char *cmd;

		// skipping first arg
		ac--;
		av++;

		if (is_cmd_opt(av, ac, "-h"))
			exit(display_help(av[0]));
		cmd = get_cmd_opt(av, ac, "-i");
		if (cmd) {
			result.iteration = std::stoi(cmd);
			result.is_random = true;
		}
		cmd = get_cmd_opt(av, ac, "-m");
		if (cmd) {
			result.data = cmd;
			result.is_random = false;
		}
		cmd = get_cmd_opt(av, ac, "-c");
		if (cmd) {
			result.clean_steps = std::stoi(cmd);
		}
		return result;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}
