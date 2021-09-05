#include <pch.h>
#include "L_SizeChange.h"
#include "Agent/BehaviorAgent.h"

void L_SizeChange::on_enter()
{

  c1 = RNG::range(0.5f, 2.0f);
  c2 = RNG::range(0.5f, 2.0f);
  c3 = RNG::range(0.5f, 2.0f);

  BehaviorNode::on_leaf_enter();
}

void L_SizeChange::on_update(float dt)
{
    agent->set_scaling(Vec3(c1, c2, c3));
    on_success();

    display_leaf_text();
}

