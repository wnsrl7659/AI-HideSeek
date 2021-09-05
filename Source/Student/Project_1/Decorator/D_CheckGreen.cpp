#include <pch.h>
#include "D_CheckGreen.h"


void D_CheckGreen::on_update(float dt)
{
  const auto color = agent->get_color();

  BehaviorNode* child = children.front();

  child->tick(dt);

  if (color.y > 0.8)
  {
    on_success();
  }
  else
  {
    on_failure();
  }
}

void D_CheckGreen::on_exit()
{
  // we want the node and child to repeat, so go back to ready
  set_status(NodeStatus::READY);
}
