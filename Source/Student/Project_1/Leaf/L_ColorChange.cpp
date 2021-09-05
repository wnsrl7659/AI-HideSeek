#include <pch.h>
#include "L_ColorChange.h"
#include "Agent/BehaviorAgent.h"

void L_ColorChange::on_enter()
{

  c1 = RNG::range(0.0f, 1.0f);
  c2 = RNG::range(0.0f, 1.0f);
  c3 = RNG::range(0.0f, 1.0f);

  BehaviorNode::on_leaf_enter();
}

void L_ColorChange::on_update(float dt)
{
    agent->set_color(Vec3(c1, c2, c3));

    on_success();

    display_leaf_text();
}

