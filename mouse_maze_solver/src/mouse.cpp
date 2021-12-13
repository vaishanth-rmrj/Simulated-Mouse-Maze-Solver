#include "../include/mouse/mouse.h"
#include "../include/api/api.h"
#include "../include/util/util.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include<unistd.h>
#include <algorithm>

// func to move mouse forward and change position of the mouse
void rwa2::Mouse::move_forward(int direction)
{

    API::moveForward();
    if(direction == 0){
        m_y += 1;
    } else if(direction == 1){
        m_x += 1;
    } else if(direction == 2){
        m_y -= 1;        
    } else if(direction == 3){
        m_x -= 1;
    } else {}
    
    API::setColor(m_x, m_y, 'G');
}

// func to turn the mouse right and change position of the mouse
void rwa2::Mouse::turn_right()
{
    API::turnRight();
    if (m_direction < 3)
    {
        m_direction += 1;
    }
    else
    {
        m_direction = 0;
    }
}

// func to turn the mouse left and change position of the mouse
void rwa2::Mouse::turn_left()
{
    API::turnLeft();
    if (m_direction > 0)
    {
        m_direction -= 1;
    }
    else
    {
        m_direction = 3;
    }
}

// func to orient the mouse in north direction if facing any other direction
void rwa2::Mouse::orient_north(){

    if(m_direction != 0){
        if(m_direction == 1){
            turn_left();
        } else if(m_direction == 2){
            turn_left();
            turn_left();
        } else if(m_direction == 3){
            turn_right();
        } else {}
    } 
}

// displays the walls surrounding the mouse in each node
void rwa2::Mouse::reveal_walls()
{   
    int x = m_x;
    int y = m_y;
    int direction = m_direction;
    if (API::wallFront())
    {   
        m_maze.at(x).at(y).set_wall(direction, true);
    }
    if (API::wallRight())
    {   
        int right_dir = direction;
        if (direction < 3)
        {
            right_dir += 1;
        }
        else
        {
            right_dir = 0;
        }

        m_maze.at(x).at(y).set_wall(right_dir, true);
    }
    if (API::wallLeft())
    {
        int left_dir = direction;
        if (direction > 0)
        {
            left_dir -= 1;
        }
        else
        {
            left_dir = 3;
        }

        m_maze.at(x).at(y).set_wall(left_dir, true);
    }

    // loops through all node objects to set/display the walls every time
    for (int x = 0; x < m_maze_width; x += 1)
    {
        for (int y = 0; y < m_maze_height; y += 1)
        {
            if (m_maze.at(x).at(y).is_wall(direction::NORTH))
            {
                API::setWall(x, y, 'n');
            }

            if (m_maze.at(x).at(y).is_wall(direction::EAST))
            {
                API::setWall(x, y, 'e');
            }

            if (m_maze.at(x).at(y).is_wall(direction::SOUTH))
            {
                API::setWall(x, y, 's');
            }

            if (m_maze.at(x).at(y).is_wall(direction::WEST))
            {
                API::setWall(x, y, 'w');
            }
            //set the text for that node providing info abt the number of walls 
            API::setText(x, y, std::to_string(m_maze.at(x).at(y).compute_number_of_walls()));
        }
    }
}

// checks if the node has been already visited
bool rwa2::Mouse::already_visited(int x, int y ,std::vector<std::pair<int,int>> &visited){

    std::pair<int,int> temp_pos{x,y};

    if (std::find(visited.begin(), visited.end(), temp_pos) != visited.end() && ((x>=0&&y>=0) && (x<16 && y<16)))
    {
        return true;
    }else
    {   
        return false;
    }
}

void rwa2::Mouse::clear_all_node_color(){
    for (int x = 0; x < m_maze_width; x += 1)
    {
        for (int y = 0; y < m_maze_height; y += 1)
            {                
                API::setColor(x, y, 'k');
                
            }
    }
}

// displays the temporariliy determined navigation path
void rwa2::Mouse::display_nav_path()
{        
    clear_all_node_color();

    usleep(300);

    // the navigation stack is inverted 
    std::stack<std::pair<int,int>> temp_stack;
    while (!nav_stack.empty())
    {
        temp_stack.push(nav_stack.top());
        nav_stack.pop();            
    }

    while (!temp_stack.empty())
    {
        API::setColor(temp_stack.top().first, temp_stack.top().second, 'y');
        temp_stack.pop();
        usleep(500);

    }
        

}

void rwa2::Mouse::display_final_path()
{
    clear_all_node_color();  

    // set the color and text for first node
    API::setColor(0,0, 'G');
    API::setText(0,0,"Start");

    // print some text info
    std::cerr << "Mission Accomplished : Goal destination reached :)" << '\n';
    std::cerr << "------------- [Visited Nodes :"<< mouse_node_hist.size() << " ] -------------------------" << '\n';
    // set color and text for all visited nodes
    while (!mouse_node_hist.empty())
    {
        std::pair<int, int> temp{mouse_node_hist.back().first,mouse_node_hist.back().second};
        API::setColor(temp.first, temp.second, 'B');
        API::setText(temp.first, temp.second,"V");

        std::cerr << "[" << mouse_node_hist.back().first << ", " << mouse_node_hist.back().second << "]" << '\n';
        mouse_node_hist.pop_back();
    }
    std::cerr << "-------------------------------------------------" << '\n';

    // set the color and text for the goal node
     std::pair<int, int> temp{mouse_route.top().first, mouse_route.top().second};
    API::setColor(temp.first, temp.second, 'G');
    API::setText(temp.first, temp.second,"Goal");

    mouse_route.pop();

    std::cerr << "------------- [Route Nodes : "<< mouse_route.size() <<" ] -------------------------" << '\n';
    // set the color and text for all the path nodes
    while (!mouse_route.empty())
    {
        std::pair<int, int> temp{mouse_route.top().first, mouse_route.top().second};
        API::setColor(temp.first, temp.second, 'G');
        API::setText(temp.first, temp.second,"Path");

        std::cerr << "[" << mouse_route.top().first << ", " << mouse_route.top().second << "]" << '\n';
        mouse_route.pop();
    }
    std::cerr << "-------------------------------------------------" << '\n';

            
}

void rwa2::Mouse::clear_stack(std::stack<std::pair<int, int>> stk)
{
    while (!stk.empty())
    {
        m_maze.at(stk.top().first).at(stk.top().second).set_visited(false);

        stk.pop();
    }
}

// check if the node is visited already by temporary navigation path
bool rwa2::Mouse::check_visted_for_generated(int x, int y, std::vector<std::pair<int,int>> &visited){
    std::pair<int,int> temp{x,y};

    if (std::find(visited.begin(), visited.end(), temp) != visited.end() && ((x>=0&&y>=0) && (x<16&&y<16)))
    {
        return false;
    }else
    {   
        return true;
    }
}

// func to generate the navigation path of the mouse
void rwa2::Mouse::generate_path(std::pair<int, int> n, std::pair<int, int> g)
{
    bool path_generation_complete{false};

    if (((n.first >= 0) && (n.second >= 0)) && ((n.first < 16) && (n.second < 16)))
    {
        while (!path_generation_complete)
        {

            if ((n.first == g.first) && (n.second == g.second))
            {
                display_nav_path();

                path_generation_complete = true;

                clear_stack(nav_stack);
                nav_visited.clear();
            }
            else
            {   
                
                if (!m_maze.at(n.first).at(n.second).is_wall(0) && (check_visted_for_generated(n.first, n.second + 1, nav_visited)))
                {
                    // check in the north direction
                    m_maze.at(n.first).at(n.second).set_visited(true);
                    n.second += 1;
                    
                    nav_stack.push({n.first, n.second});
                    nav_visited.push_back({n.first, n.second});
                }
                else if ((!m_maze.at(n.first).at(n.second).is_wall(1)) && (check_visted_for_generated(n.first + 1, n.second, nav_visited)))
                {
                    // check in the eat direction
                    m_maze.at(n.first).at(n.second).set_visited(true);
                    n.first += 1;

                    
                    nav_stack.push({n.first, n.second});
                    nav_visited.push_back({n.first, n.second});

                } 
                else if ((!m_maze.at(n.first).at(n.second).is_wall(2)) && (check_visted_for_generated(n.first, n.second - 1, nav_visited)))
                {
                    // check in the south direction
                    m_maze.at(n.first).at(n.second).set_visited(true);
                    n.second -= 1;

                    nav_stack.push({n.first, n.second});
                    nav_visited.push_back({n.first, n.second});

                } else if (!m_maze.at(n.first).at(n.second).is_wall(3) && (check_visted_for_generated(n.first - 1, n.second, nav_visited)))
                {
                    // check in the west direction
                    m_maze.at(n.first).at(n.second).set_visited(true);
                    n.first -= 1;
                    
                    nav_stack.push({n.first, n.second});
                    nav_visited.push_back({n.first, n.second});
                }
                else
                {

                    std::pair<int, int> temp_top{nav_stack.top().first, nav_stack.top().second};
                    nav_stack.pop();

                    std::pair<int, int> temp2{nav_stack.top().first, nav_stack.top().second};

                    int x_diff{temp2.first - temp_top.first};
                    int y_difft{temp2.second - temp_top.second};

                    if(!(x_diff == 0 && y_difft == 1)){
                        if (x_diff == 1 && y_difft == 0)
                        {
                            n.first += 1;

                        } else if (x_diff == 0 && y_difft == -1)
                        {
                            n.second -= 1;

                        } else if (x_diff == -1 && y_difft == 0)
                        {   
                            n.first -= 1;

                        } else {

                        }
                    } else {
                        n.second += 1; 
                    }
                }
            }
        }
    }
}

// func to back track the route
void rwa2::Mouse::backtrack_path(){

    orient_north();

    API::setColor(mouse_route.top().first, mouse_route.top().second, 'B');

    std::pair<int, int> temp_top{mouse_route.top().first, mouse_route.top().second};
    mouse_route.pop();                    

    std::pair<int, int> temp2{mouse_route.top().first, mouse_route.top().second};
    int x_diff{temp2.first - temp_top.first};
    int y_difft{temp2.second - temp_top.second};

    if(!(x_diff == 0 && y_difft == 1)){
        if (x_diff == 1 && y_difft == 0)
        {
            turn_right();
            move_forward(m_direction);

        } else if (x_diff == 0 && y_difft == -1)
        {
            turn_right();
            turn_right();
            move_forward(m_direction);

        } else if (x_diff == -1 && y_difft == 0)
        {   
            turn_left();
            move_forward(m_direction);

        } else {

        }
    } else {
        move_forward(m_direction); 
    }
}

bool rwa2::Mouse::search_maze(const std::pair<int, int> goal){

    // always orient the mouse towards north
    orient_north();

    if (m_x == goal.first && m_y == goal.second)
    {
        API::setColor(m_x, m_y, 'Y');
        return true;
    }
    else
    {   
        if ((!API::wallFront()) && (!already_visited(m_x, m_y + 1, mouse_node_hist)))
        {
            move_forward(m_direction);

            mouse_route.push({m_x, m_y});                
            mouse_node_hist.push_back(m_maze.at(m_x).at(m_y).get_cords());

            API::setColor(m_x, m_y, 'G');

        } else if (!API::wallRight() && (!already_visited(m_x + 1, m_y, mouse_node_hist))){

            turn_right();
            move_forward(m_direction);

            mouse_route.push({m_x, m_y});
            mouse_node_hist.push_back(m_maze.at(m_x).at(m_y).get_cords());

            API::setColor(m_x, m_y, 'G');
            
        }
        else
        {
            turn_right();
            turn_right();
            if (!API::wallFront() && (!already_visited(m_x, m_y - 1, mouse_node_hist)))
            {
                move_forward(m_direction);
                mouse_route.push({m_x, m_y});
                API::setColor(m_x, m_y, 'G');

                mouse_node_hist.push_back(m_maze.at(m_x).at(m_y).get_cords());

            } 
            else if (!API::wallRight() && (!already_visited(m_x - 1, m_y, mouse_node_hist))) 
            {
                turn_right();
                move_forward(m_direction);

                mouse_route.push({m_x, m_y});
                mouse_node_hist.push_back(m_maze.at(m_x).at(m_y).get_cords());

                API::setColor(m_x, m_y, 'G');
                
            }
            else
            {                   
                backtrack_path(); // backtracking                
            } 
            
        }
        
    }

    std::pair<int, int> current_node = std::make_pair(m_x, m_y);
    
    generate_path(current_node, goal);

    return false;
}