#include <pch.h>
#include "L_ChaseAgent.h"
#include "Agent/BehaviorAgent.h"

void L_ChaseAgent::on_enter()
{
    int randenemy = RNG::range(0, 3);
    Vec3 furthestPoint;
    bool targetFound = false;

    // get a list of all current agents
    const auto &allAgents = agents->get_all_agents();

    // and our agent's position
    const auto &currPos = agent->get_position();

    // make sure it's not our agent
    while (true)
    {
      if (allAgents[randenemy] != agent)
      {
        break;
      }
      else
      {
        randenemy = RNG::range(0, 3);
      }
    }

    const auto& agentPos = allAgents[randenemy]->get_position();
    const float distance = Vec3::Distance(currPos, agentPos);

    furthestPoint = agentPos;
    targetFound = true;

    if (targetFound == true)
    {
        targetPoint = furthestPoint;
		    BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a viable agent
    {
        on_failure();
    }
}

void L_ChaseAgent::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

