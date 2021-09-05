#include <pch.h>
#include "L_ChangeEnemyColor.h"
#include "Agent/BehaviorAgent.h"

void L_ChangeEnemyColor::on_enter()
{
  c1 = RNG::range(0.0f, 1.0f);
  c2 = RNG::range(0.0f, 1.0f);
  c3 = RNG::range(0.0f, 1.0f);

  float closestDistance = std::numeric_limits<float>().max();
  Vec3 furthestPoint;
  bool targetFound = false;

  // get a list of all current agents
  const auto& allAgents = agents->get_all_agents();

  // and our agent's position
  const auto& currPos = agent->get_position();

  for (const auto& a : allAgents)
  {
    // make sure it's not our agent
    if (a != agent)
    {
      const auto& agentPos = a->get_position();
      const float distance = Vec3::Distance(currPos, agentPos);

      if (distance < closestDistance && distance < 10.0f)
      {
        closestDistance = distance;
        furthestPoint = agentPos;
        enemy = a;
        targetFound = true;
      }
    }
  }

  if (targetFound == true)
  {
    //targetPoint = furthestPoint;
    BehaviorNode::on_leaf_enter();
  }
  else // couldn't find a viable agent
  {
    on_failure();
  }
}

void L_ChangeEnemyColor::on_update(float dt)
{
    enemy->set_color(Vec3(c1, c2, c3));

    on_success();

    display_leaf_text();
}

