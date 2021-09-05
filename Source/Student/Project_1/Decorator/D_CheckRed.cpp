#include <pch.h>
#include "D_CheckRed.h"


void D_CheckRed::on_update(float dt)
{
  const auto color = agent->get_color();

  BehaviorNode* child = children.front();

  child->tick(dt);

  if (color.x > 0.8)
  {
    on_success();
  }
  else
  {
    on_failure();
  }
}

void D_CheckRed::on_exit()
{
  // we want the node and child to repeat, so go back to ready
  set_status(NodeStatus::READY);
}
