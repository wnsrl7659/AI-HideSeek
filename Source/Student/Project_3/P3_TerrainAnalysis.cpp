#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    // WRITE YOUR CODE HERE


  int w = terrain->get_map_width();
  int h = terrain->get_map_height();
  /*
  Vec3 tempo = terrain->get_world_position(0, 0);
  Vec3 tempt = terrain->get_world_position(1, 0);

  float diff = abs(tempo.x - tempt.x);
  */
  float closest = FLT_MAX;
  int savedx, savedy;

  for (int i = -1; i <= w; i++)
  {
    for (int j = -1; j <= h; j++)
    {
      if (i == col && j == row)
      {
        continue;
      }
      if (i == -1 || i == w || j == -1 || j == h || terrain->is_wall(j, i))
      {
        //out bound or wall
        int dx = abs(col - i);
        int dy = abs(row - j);

        float dist = sqrt(dx * dx + dy * dy);
        if (dist < closest)
        {
          closest = dist;
          savedx = dx;
          savedy = dy;
        }
      }
    }
  }

  //int dx = savedx * diff;
  //int dy = savedy * diff;

  return closest; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    // WRITE YOUR CODE HERE
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  Vec3 tempo = terrain->get_world_position(0, 0);
  Vec3 tempt = terrain->get_world_position(1, 0);

  float diff = abs(tempo.x - tempt.x);
  float halfdiff = diff / 2;

  Vec3 line0P0 = terrain->get_world_position(row0, col0);
  Vec3 line0P1 = terrain->get_world_position(row1, col1);


  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (terrain->is_wall(j, i))
      {
        Vec2 P1, P2, P3, P4, center;
        Vec3 temp = terrain->get_world_position(j, i);
        center.x = temp.z;
        center.y = temp.x;

        //++
        P1.x = center.x + (halfdiff + 0.1f);
        P1.y = center.y + (halfdiff + 0.1f);
        //+-
        P2.x = center.x + (halfdiff + 0.1f);
        P2.y = center.y - (halfdiff + 0.1f);
        //--
        P3.x = center.x - (halfdiff + 0.1f);
        P3.y = center.y - (halfdiff + 0.1f);
        //-+
        P4.x = center.x - (halfdiff + 0.1f);
        P4.y = center.y + (halfdiff + 0.1f);
       

        if (line_intersect(Vec2(line0P0.z, line0P0.x), Vec2(line0P1.z, line0P1.x), P1, P2) ||
          line_intersect(Vec2(line0P0.z, line0P0.x), Vec2(line0P1.z, line0P1.x), P2, P3) || 
          line_intersect(Vec2(line0P0.z, line0P0.x), Vec2(line0P1.z, line0P1.x), P3, P4) || 
          line_intersect(Vec2(line0P0.z, line0P0.x), Vec2(line0P1.z, line0P1.x), P4, P1))
        {
          return false;
        }
      }
    }
  }


  return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE

  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (!terrain->is_wall(j, i))
      {
        float d = distance_to_closest_wall(j, i);
        layer.set_value(j, i, (1.0f / (d * d)));
      }
    }
  }
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      int count = 0;

      for (int ii = 0; ii < w; ii++)
      {
        for (int jj = 0; jj < h; jj++)
        {
          if (i == ii && j == jj)
          {
            count++;
          }
          else if (is_clear_path(j, i, jj, ii))
          {
            count++;
          }
        }
      }
      float result = (float)count / 160.0f;
      if (result > 1.0f)
      {
        result = 1.0f;
      }
      layer.set_value(j, i, result);
    }
  }
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      layer.set_value(j, i, 0.0);
    }
  }

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (is_clear_path(row, col, j, i))
      {
        layer.set_value(j, i, 1.0);
      }
    }
  }

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float value = layer.get_value(j, i);

      if (value == 0.0)
      {
        if (!terrain->is_wall(j, i))
        {
          float l, r, u, d;
          //left
          if (i != 0)
          {
            l = layer.get_value(j, i - 1);
          }
          else
          {
            l = 0;
          }
          //right
          if (i != w - 1)
          {
            r = layer.get_value(j, i + 1);
          }
          else
          {
            r = 0;
          }
          //up
          if (j != h - 1)
          {
            u = layer.get_value(j + 1, i);
          }
          else
          {
            u = 0;
          }
          //down
          if (j != 0)
          {
            d = layer.get_value(j - 1, i);
          }
          else
          {
            d = 0;
          }

          if (l == 1.0 || r == 1.0 || d == 1.0 || u == 1.0)
          {
            layer.set_value(j, i, 0.5);
          }
        }
      }
    }
  }
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  Vec3 agentPos = agent->get_position();

  GridPos agentGrid = terrain->get_grid_position(agentPos);

  //x = row, z = col
  Vec3 agentFor = agent->get_forward_vector();


  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (i == agentGrid.col && j == agentGrid.row)
      {
        layer.set_value(j, i, 1.0);
        continue;
      }
      Vec2 u, v;
      //view
      u.x = agentFor.z;
      u.y = agentFor.x;
      //other
      v.x = i - agentGrid.col;
      v.y = j - agentGrid.row;

      float dotuv = u.x * v.x + u.y * v.y;
      float ul = sqrt(u.x * u.x + u.y * u.y);
      float vl = sqrt(v.x * v.x + v.y * v.y);

      float cosang = dotuv / (ul * vl);

      if (cosang <= -0.05f)
      {
        continue;
      }

      if (is_clear_path(agentGrid.row, agentGrid.col, j, i))
      {
        layer.set_value(j, i, 1.0);
      }
    }
  }

}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    // WRITE YOUR CODE HERE
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  float templayer[40][40] = { 0 };

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (terrain->is_wall(j,i))
      {
        continue;
      }
      float maximum = 0.0f;
      //1) Get the value of each neighbor and apply decay factor
      for (int x = -1; x <= 1; x++)
      {
        for (int y = -1; y <= 1; y++)
        {
          //check bound
          if (x == 0 && y == 0)
          {
            continue;
          }
          else if (i == 0 && x == -1)
          {
            continue;
          }
          else if (i == w - 1 && x == 1)
          {
            continue;
          }
          else if (j == 0 && y == -1)
          {
            continue;
          }
          else if (j == h - 1 && y == 1)
          {
            continue;
          }

          if ((abs(x) + abs(y)) == 2)
          {
            if (terrain->is_wall(j + y, i) || terrain->is_wall(j, i + x))
            {
              continue;
            }
          }

          //get value 
          float oldvalue = layer.get_value(j + y, i + x);
          float dist = sqrt(x * x + y * y);
          float newvalue = oldvalue * exp(-1 * dist * decay);
          if (newvalue > maximum)
          {
            maximum = newvalue;
          }
        }
      }
      //2) Keep the highest value from step 1
      //maximum
      //3) Linearly interpolate from the cell's current value to the value from step 2
      //   with the growing factor as a coefficient.  Make use of the lerp helper function.
      float oldinflu = layer.get_value(j, i);
      float newinflu = (1.0f - growth) * oldinflu + growth * maximum;
      //4) Store the value from step 3 in a temporary layer.
      templayer[i][j] = newinflu;
    }
  }

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      layer.set_value(j, i, templayer[i][j]);
    }
  }
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  float templayer[40][40];

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float maximum = 0.0f;
      //1) Get the value of each neighbor and apply decay factor
      for (int x = -1; x <= 1; x++)
      {
        for (int y = -1; y <= 1; y++)
        {
          if (x == 0 && y == 0)
          {
            continue;
          }
          else if (i == 0 && x == -1)
          {
            continue;
          }
          else if (i == w - 1 && x == 1)
          {
            continue;
          }
          else if (j == 0 && y == -1)
          {
            continue;
          }
          else if (j == h - 1 && y == 1)
          {
            continue;
          }


          if ((abs(x) + abs(y)) == 2)
          {
            if (terrain->is_wall(j + y, i) || terrain->is_wall(j, i + x))
            {
              continue;
            }
          }

          //get value 
          float oldvalue = layer.get_value(j + y, i + x);
          float dist = sqrt(x * x + y * y);
          float newvalue = oldvalue * exp(-1 * dist * decay);
          if (abs(newvalue) > abs(maximum))
          {
            maximum = newvalue;
          }
        }
      }
      //2) Keep the highest ABSOLUTE from step 1
      //maximum
      //3) Linearly interpolate from the cell's current value to the value from step 2
      //   with the growing factor as a coefficient.  Make use of the lerp helper function.
      float oldinflu = layer.get_value(j, i);
      float newinflu = (1.0f - growth) * oldinflu + growth * maximum;
      //4) Store the value from step 3 in a temporary layer.
      templayer[i][j] = newinflu;
    }
  }

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      layer.set_value(j, i, templayer[i][j]);
    }
  }
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE

  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  float maximum = 0.0f;
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float value = layer.get_value(j, i);
      if (maximum < value)
      {
        maximum = value;
      }
    }
  }
  if (maximum == 0.0f)
  {
    return;
  }
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float result;
      float value = layer.get_value(j, i);
      if (value > 0.0f)
      {
        result = value / maximum;
        layer.set_value(j, i, result);
      }
    }
  }
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  float maximum = 0.0f;
  float minimum = 0.0f;
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float value = layer.get_value(j, i);
      if (maximum < value)
      {
        maximum = value;
      }
      if (minimum > value)
      {
        minimum = value;
      }
    }
  }

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float result;
      float value = layer.get_value(j, i);
      if (value > 0.0f)
      {
        if (maximum == 0.0f)
        {
          continue;
        }
        result = value / maximum;
        layer.set_value(j, i, result);
      }
      else if (value < 0.0f)
      {
        if (minimum == 0.0f)
        {
          continue;
        }
        result = value / (-1.0 * minimum);
        layer.set_value(j, i, result);
      }
    }
  }
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // WRITE YOUR CODE HERE
  //First, clear out the old values in the map layer by setting any negative value to 0.
  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float value = layer.get_value(j, i);
      if (value < 0.0f)
      {
        layer.set_value(j, i, 0.0f);
      }
    }
  }

  //Then, for every cell in the layer that is within the field of view cone, from the
  //enemy agent, mark it with the occupancy value.

  Vec3 agentPos = enemy->get_position();

  GridPos agentGrid = terrain->get_grid_position(agentPos);

  //x = row, z = col
  Vec3 agentFor = enemy->get_forward_vector();


  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (i == agentGrid.col && j == agentGrid.row)
      {
        layer.set_value(j, i, occupancyValue);
        continue;
      }
      Vec2 u, v;
      //view
      u.x = agentFor.z;
      u.y = agentFor.x;
      //other
      v.x = i - agentGrid.col;
      v.y = j - agentGrid.row;


      float dotuv = u.x * v.x + u.y * v.y;
      float ul = sqrt(u.x * u.x + u.y * u.y);
      float vl = sqrt(v.x * v.x + v.y * v.y);

      if (vl < closeDistance)
      {
        if (is_clear_path(agentGrid.row, agentGrid.col, j, i))
        {
          layer.set_value(j, i, occupancyValue);
          continue;
        }
      }

      float cosang = dotuv / (ul * vl);

      float fang = cos(fovAngle * 0.5 * PI / 180.0);


      if (cosang <= (fang - 0.05f))
      {
        continue;
      }

      if (is_clear_path(agentGrid.row, agentGrid.col, j, i))
      {
        layer.set_value(j, i, occupancyValue);
      }
    }
  }
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

     //WRITE YOUR CODE HERE
  Vec3 agentPos = enemy->get_position();
  GridPos agentGrid = terrain->get_grid_position(agentPos);

  int w = terrain->get_map_width();
  int h = terrain->get_map_height();

  float maximum = 0.0;
  float savecol = -1;
  float saverow = -1;

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      float value = layer.get_value(j, i);

      if (maximum < value)
      {
        maximum = value;
        savecol = i;
        saverow = j;
      }
      else if (maximum == value)
      {
        float dx0 = agentGrid.col - savecol;
        float dy0 = agentGrid.row - saverow;

        float dx1 = agentGrid.col - i;
        float dy1 = agentGrid.row - j;

        float l0 = sqrt(dx0 * dx0 + dy0 * dy0);
        float l1 = sqrt(dx1 * dx1 + dy1 * dy1);

        if (l0 > l1)
        {
          savecol = i;
          saverow = j;
        }
      }
    }
  }
  
  if (savecol == -1 && saverow == -1)
  {
    return false;
  }
  else
  {
    enemy->path_to(terrain->get_world_position(saverow, savecol));
    return true;
  }
    
    return false; // REPLACE THIS
}
