/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   State.hpp										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: edelangh <edelangh@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2016/03/04 14:16:53 by edelangh		  #+#	#+#			 */
/*   Updated: 2016/03/18 19:03:56 by edelangh		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef STATE_HPP
# define STATE_HPP

#include <functional>
#include <vector>
#include <iostream>
#include "Types.hpp"

class State;

using indexer = Score (*)(const State&);

class State {
public:

    enum Movement {
        None = 0,
        Up = 1,
        Right = 2,
        Down = 3,
        Left = 4,
        Front = 5,
        Back = 6,

        Movement_Start = 1,
        Movement_End = 7,

        Mask = 7,
        Reversed = 8,
        Halfturn = 16,

        R_None = Reversed | None,
        R_Up = Reversed | Up,
        R_Right = Reversed | Right,
        R_Down = Reversed | Down,
        R_Left = Reversed | Left,
        R_Front = Reversed | Front,
        R_Back = Reversed | Back,

        H_None = Halfturn | None,
        H_Up = Halfturn | Up,
        H_Right = Halfturn | Right,
        H_Down = Halfturn | Down,
        H_Left = Halfturn | Left,
        H_Front = Halfturn | Front,
        H_Back = Halfturn | Back,
    };

    //constructors
    inline State();
    inline State(bool is_del);
    inline State(const ID& id);
    inline State(const State& clone);
    inline State(const string& scramble);
    inline State(int scramble_count);
    inline State(const State& parent, Movement direction);
    inline State(const State& parent, const string& scramble);
    inline State& operator=(const State& ra);

    //getters
    constexpr Movement						get_movement() const;
    constexpr uint							get_distance() const;
    constexpr uint 							get_weight() const;
    constexpr const ID&						get_id() const;
    constexpr bool 							is_final() const;
    constexpr bool                          is_solvable() const;

    //logic wrappers
    constexpr void							get_candidates(std::vector<State>& candidates) const;

    //ID Cube conversions
    constexpr Data							get_data() const;
    constexpr Cube							get_cube() const;
    constexpr static ID			            id_from_data(const Data& data);
    constexpr static Data		            data_from_id(const ID id);
    constexpr static Cube		            cube_from_data(const Data& data);
    constexpr static ID			            id_from_cube(const Cube& cube);
    constexpr static Cube		            cube_from_id(const ID data);

    //indexers
    constexpr static Score					indexer_astar(const State&);
    constexpr static Score					indexer_uniform(const State&);
    constexpr static Score					indexer_greedy(const State&);
    constexpr static indexer				get_index = indexer_astar;

    constexpr bool                          operator==(const State& ra) const;

    //debug
    inline static void					    _apply_scramble(Data& data, const string& scramble);
    constexpr static void					_apply_movement(Data& data, Movement m, int turns);
    constexpr static void					_apply_movement(Data& data, Movement m);
    constexpr static int					_get_turns(Movement m);
private:
    //optimized constructor
    State(const State& parent, Movement direction, const Data& data);

    //calculations
    inline void						        _apply_data(const Data& data);
    constexpr static Finder			        _calculate_finder(const Cube& Cube);

    //members, should add up to 128bits :D
    ID								        _id;
    uint16_t						        _movement:16;
    uchar							        _weight:8;
    uchar							        _distance:8;
public:

    static const Data			       solution_data;
    static const Cube			       solution_cube;
    static const Finder		           solution_finder;
    static const Color			       solution_colors[];
};
inline std::ostream&					        operator<<(std::ostream& s, const State::Movement c);

struct custom_hash
{
	public:
		constexpr size_t operator()(const State& l) const noexcept;
};

#include "State_Encoding.cpp"
#include "State_Header.cpp"

#endif
