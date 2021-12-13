/**
 * @file node.h
 * @author Zeid Kootbally (zeidk@umd.edu)
 * @brief This file contains a class to represent a node in a maze.
 * @version 0.1
 * @date 2021-10-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef NODE_H
#define NODE_H

#include <array>

namespace rwa2 {
    /**
     * @brief Class to represent a node (cell) in a maze.
     *
     *A node is just a space delimited by 4 walls
     *
     */
    class Node
    {
    public:
        Node() {
            for (int i = 0; i < 4; i += 1) {
                m_walls[i] = false;
            }
        }
        /**
         * @brief Set the wall of a cell
         *
         * @param direction NORTH, EAST, SOUTH, or WEST
         * @param is_wall true if there is a wall, otherwise false
         */
        void set_wall(int direction, bool is_wall);
        /**
         * @brief Return whether or not there is a wall in a cell
         *
         * @param direction Direction to set for wall (NORTH, EAST, SOUTH, or WEST)
         * @return true There is a wall in the given direction in the cell
         * @return false There is no wall in the given direction in the cell
         */
        bool is_wall(int direction) const;
        /**
         * @brief Compute the number of walls surrounding a node
         * 
         * @return int Number of walls surrounding a node
         */
        int compute_number_of_walls() const;

        std::pair<int,int> get_cords();

        void set_cords(int x,int y);

        void set_visited(bool set);

        bool get_visited();

    private:
        std::array<bool, 4> m_walls; //all four walls in a cell
        std::pair<int , int> cords; //Stores the cordinates of each node 
        bool visited{false};

    };
}
#endif
