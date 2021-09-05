#include <pch.h>
#include "L_MoveToCenter.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCenter::on_enter()
{
    // set animation, speed, etc

  const auto& currPos = agent->get_position();

  targetPoint = Vec3(0, 0, 0);

    if (currPos.x < 50.0f)
    {
      targetPoint.x = 5;
    }
    else
    {
      targetPoint.x = -5;
    }

    if (currPos.z < 50.0f)
    {
      targetPoint.z = 5;
    }
    else
    {
      targetPoint.z = -5;
    }

    targetPoint = targetPoint + currPos;
    //agent->look_at_point(targetPoint);

	BehaviorNode::on_leaf_enter();
}

void L_MoveToCenter::on_update(float dt)
{
  Vec3 center = Vec3(50.0f, 0.0f, 50.0f);
  const auto& currPos = agent->get_position();
  const float distance = Vec3::Distance(currPos, center);

  if (distance < 5.0f)
  {
    on_failure();
  }
  else
  {
    const auto result = agent->move_toward_point((targetPoint), dt);

    if (result == true)
    {
      on_success();
    }

    display_leaf_text();
  }

}
