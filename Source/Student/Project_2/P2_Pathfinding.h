#pragma once
#include "Misc/PathfindingDetails.hpp"

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
    struct Node
    {
      int y;
      int x;
      int parentX;
      int parentY;
      float gCost;
      float hCost;
      float fCost;
    };


    std::list<Vec3> makePath(Node** map, GridPos dest);
    double calculateH(int x, int y, GridPos dest, Heuristic value);
    bool isValid(int x, int y);
    void mapchanged();
private:
  std::vector<Node> openlist;
  bool** closedlist;
  Node** allmap;
  int mh;
  int mw;
};