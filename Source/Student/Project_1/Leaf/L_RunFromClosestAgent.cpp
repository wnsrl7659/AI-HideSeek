#include <pch.h>
#include "L_RunFromClosestAgent.h"
#include "Agent/BehaviorAgent.h"

void L_RunFromClosestAgent::on_enter()
{
    // set animation, speed, etc

    // find the agent that is the closest from this one
    float closestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    bool targetFound = false;

    // get a list of all current agents
    const auto &allAgents = agents->get_all_agents();

    // and our agent's position
    const auto &currPos = agent->get_position();

    for (const auto & a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {
            const auto &agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance < closestDistance && distance < 10.0f)
            {
                closestDistance = distance;
                furthestPoint = agentPos;
                targetFound = true;
            }
        }
    }

    if (targetFound == true)
    {
        //targetPoint = furthestPoint;
      if (closestDistance < 0.5f)
      {
        targetPoint = currPos - Vec3(5,0,5);
      }
      else
      {
        targetPoint = currPos - (furthestPoint - currPos);
      }
		    BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a viable agent
    {
        on_failure();
    }
}

void L_RunFromClosestAgent::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

