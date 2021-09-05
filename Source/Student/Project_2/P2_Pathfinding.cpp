#include <pch.h>
#include <stack>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

inline bool operator == (const AStarPather::Node& lhs, const AStarPather::Node& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
inline bool operator < (const AStarPather::Node& lhs, const AStarPather::Node& rhs)
{
  return lhs.fCost < rhs.fCost;
}

double AStarPather::calculateH(int x, int y, GridPos dest, Heuristic value) {
  double dx, dy;
  dx = abs(x - dest.col);
  dy = abs(y - dest.row);

  switch(value)
  {
    case Heuristic::OCTILE:
      return sqrt(2) * std::min(dx, dy) + std::max(dx, dy) - std::min(dx, dy);
      break;
    case Heuristic::CHEBYSHEV:
      return std::max(dx, dy);
      break;
    case Heuristic::MANHATTAN:
      return (dx + dy);
      break;
    case Heuristic::EUCLIDEAN:
      return sqrt(dx * dx + dy * dy);
      break;
  }
  double H = (sqrt((x - dest.col) * (x - dest.col)
    + (y - dest.row) * (y - dest.row)));
  return H;
}

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */

  /*
  mh = terrain->get_map_height();
  mw = terrain->get_map_width();

  closedlist = new bool* [mw];
  for (int i = 0; i < mw; i++)
  {
    closedlist[i] = new bool[mh];
  }

  allmap = new Node* [mw];
  for (int i = 0; i < mw; i++)
  {
    allmap[i] = new Node[mh];
  }

  for (int x = 0; x < mw; x++)
  {
    for (int y = 0; y < mh; y++)
    {
      allmap[x][y].fCost = FLT_MAX;
      allmap[x][y].gCost = FLT_MAX;
      allmap[x][y].hCost = FLT_MAX;
      allmap[x][y].parentX = -1;
      allmap[x][y].parentY = -1;
      allmap[x][y].x = x;
      allmap[x][y].y = y;

      closedlist[x][y] = false;
    }
  }

  openlist.clear();
  */
  Callback cb = std::bind(&AStarPather::mapchanged, this);
  Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::mapchanged()
{
  for (int i = 0; i < mw; i++)
  {
    delete[] closedlist[i];
  }
  delete[] closedlist;

  for (int i = 0; i < mw; i++)
  {
    delete[] allmap[i];
  }
  delete[] allmap;

  //////////////////////////
  
  mh = terrain->get_map_height();
  mw = terrain->get_map_width();

  closedlist = new bool* [mw];
  for (int i = 0; i < mw; i++)
  {
    closedlist[i] = new bool[mh];
  }

  allmap = new Node * [mw];
  for (int i = 0; i < mw; i++)
  {
    allmap[i] = new Node[mh];
  }

  for (int x = 0; x < mw; x++)
  {
    for (int y = 0; y < mh; y++)
    {
      allmap[x][y].fCost = FLT_MAX;
      allmap[x][y].gCost = FLT_MAX;
      allmap[x][y].hCost = FLT_MAX;
      allmap[x][y].parentX = -1;
      allmap[x][y].parentY = -1;
      allmap[x][y].x = x;
      allmap[x][y].y = y;

      closedlist[x][y] = false;
    }
  }

  openlist.clear();
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */

  for (int i = 0; i < mw; i++)
  {
    delete[] closedlist[i];
  }
  delete[] closedlist;

  for (int i = 0; i < mw; i++)
  {
    delete[] allmap[i];
  }
  delete[] allmap;
}

bool AStarPather::isValid(int x, int y) {
  if (terrain->is_valid_grid_position(y, x)) {
    if (!terrain->is_wall(y, x)) {
      return true;
    }
  }
  return false;
}
PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

    // WRITE YOUR CODE HERE

GridPos start = terrain->get_grid_position(request.start);
GridPos goal = terrain->get_grid_position(request.goal);


int x, y;

if (request.newRequest)
{
  if (terrain->is_wall(goal) || !terrain->is_valid_grid_position(goal))
  {
    return PathResult::IMPOSSIBLE;
  }
  if (request.start == request.goal)
  {
    return PathResult::COMPLETE;
  }

  x = start.col;
  y = start.row;
  allmap[x][y].fCost = 0.0;
  allmap[x][y].gCost = 0.0;
  allmap[x][y].hCost = 0.0;
  allmap[x][y].parentX = x;
  allmap[x][y].parentY = y;

  openlist.emplace_back(allmap[x][y]);
}


while (!openlist.empty() && openlist.size() < (mh * mw))
{
  Node node;

  // Pop cheapest node off Open List (parent node)
  do {
    float temp = FLT_MAX;
    std::vector<Node>::iterator itNode;
    for (std::vector<Node>::iterator it = openlist.begin(); it != openlist.end(); it = next(it))
    {
      Node n = *it;
      if (n.fCost < temp)
      {
        temp = n.fCost;
        itNode = it;
      }
    }
    node = *itNode;
    openlist.erase(itNode);
  } while (isValid(node.x, node.y) == false);

  x = node.x;
  y = node.y;

  //If node is the Goal Node, then path found (RETURN ¡°found¡±).
  if ((x == goal.col) && (y == goal.row))
  {
    ///////////
    request.path = makePath(allmap, goal);

    //color
    for (std::vector<Node>::iterator it = openlist.begin(); it != openlist.end(); it = next(it))
    {
      GridPos temp;
      Node n = *it;

      temp.col = n.x;
      temp.row = n.y;
      if (request.settings.debugColoring)
      {
        terrain->set_color(temp, Colors::Blue);
      }
    }
    for (int x = 0; x < mw; x++)
    {
      for (int y = 0; y < mh; y++)
      {
        if (closedlist[x][y])
        {
          GridPos temp;
          temp.col = x;
          temp.row = y;

          if (request.settings.debugColoring)
          {
            terrain->set_color(temp, Colors::Yellow);
          }
        }
      }
    }

    openlist.clear();
    for (int x = 0; x < mw; x++)
    {
      for (int y = 0; y < mh; y++)
      {
        closedlist[x][y] = false;
      }
    }
    
    ///////////
    return PathResult::COMPLETE;
  }

  //For (all neighboring child nodes) 
  for (int newX = -1; newX <= 1; newX++) {
    for (int newY = -1; newY <= 1; newY++) {
      if (abs(newX) + abs(newY) == 2)
      {
        if (!isValid(x, y + newY) || !isValid(x + newX, y))
        {
          continue;
        }
      }

        if (newX == 0 && newY == 0)
        {
          continue;
        }

        //Compute its cost, f(x) = g(x) + h(x)
        float gNew, hNew, fNew;

        if (abs(newX) + abs(newY) == 2)
        {
          gNew = node.gCost + sqrt(2);
        }
        else
        {
          gNew = node.gCost + 1.0;
        }
        hNew = calculateH(x + newX, y + newY, goal, request.settings.heuristic);
        fNew = gNew + hNew * request.settings.weight;


        if (isValid(x + newX, y + newY)) {
          std::vector<Node>::iterator findit = std::find(openlist.begin(), openlist.end(), allmap[x + newX][y + newY]);
          //If child node isn¡¯t on Open and Closed list, put it on Open List
          if (findit == openlist.end() && closedlist[x + newX][y + newY] == false)
          {
            allmap[x + newX][y + newY].fCost = fNew;
            allmap[x + newX][y + newY].gCost = gNew;
            allmap[x + newX][y + newY].hCost = hNew;
            allmap[x + newX][y + newY].parentX = x;
            allmap[x + newX][y + newY].parentY = y;
            openlist.emplace_back(allmap[x + newX][y + newY]);
          }
          //Else if child node is on Open or Closed List,
          else if (findit != openlist.end() || closedlist[x + newX][y + newY] == true)
          {
            // AND this new one is cheaper
            if (allmap[x + newX][y + newY].fCost > fNew)
            {
              //then take the old expensive one off both lists 
              closedlist[x + newX][y + newY] = false;
              if (findit != openlist.end())
              {
                openlist.erase(findit);
              }

              //and put this new cheaper one on the Open List.
              allmap[x + newX][y + newY].fCost = fNew;
              allmap[x + newX][y + newY].gCost = gNew;
              allmap[x + newX][y + newY].hCost = hNew;
              allmap[x + newX][y + newY].parentX = x;
              allmap[x + newX][y + newY].parentY = y;
              openlist.emplace_back(allmap[x + newX][y + newY]);
            }
          }
        }
      }
    }
    //Place parent node on the Closed List (we¡¯re done with it)
    closedlist[x][y] = true;
    //color
    for (std::vector<Node>::iterator it = openlist.begin(); it != openlist.end(); it = next(it))
    {
      GridPos temp;
      Node n = *it;

      temp.col = n.x;
      temp.row = n.y;

      if (request.settings.debugColoring)
      {
        terrain->set_color(temp, Colors::Blue);
      }
    }
    for (int x = 0; x < mw; x++)
    {
      for (int y = 0; y < mh; y++)
      {
        if (closedlist[x][y])
        {
          GridPos temp;
          temp.col = x;
          temp.row = y;

          if (request.settings.debugColoring)
          {
            terrain->set_color(temp, Colors::Yellow);
          }
        }
      }
    }

    //If taken too much time this frame (or in single step mode)
    if (request.settings.singleStep == true)
    {
      //abort search for now and resume next frame (RETURN ¡°working¡±).
      return PathResult::PROCESSING;
    }
  }

  openlist.clear();
  for (int x = 0; x < mw; x++)
  {
    for (int y = 0; y < mh; y++)
    {
      closedlist[x][y] = false;
    }
  }
  //Open List empty, thus no path possible (RETURN ¡°fail¡±).
  return PathResult::IMPOSSIBLE;
}

std::list<Vec3> AStarPather::makePath(Node** map, GridPos dest) {
  int x = dest.col;
  int y = dest.row;
  std::stack<Node> path;
  std::list<Vec3> usablePath;

  while (!(map[x][y].parentX == x && map[x][y].parentY == y)
    && map[x][y].x != -1 && map[x][y].y != -1)
  {
    path.push(map[x][y]);
    int tempX = map[x][y].parentX;
    int tempY = map[x][y].parentY;
    x = tempX;
    y = tempY;

  }
  path.push(map[x][y]);

  while (!path.empty()) {
    Node top = path.top();
    path.pop();
    Vec3 temp;
    temp = terrain->get_world_position(top.y, top.x);

    usablePath.emplace_back(temp);
  }
  return usablePath;
}