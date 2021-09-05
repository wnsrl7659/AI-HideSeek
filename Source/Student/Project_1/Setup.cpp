#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create your inital agents
  agents->create_behavior_agent("AgentOne", BehaviorTreeTypes::AgentOne);
  agents->create_behavior_agent("AgentTwo", BehaviorTreeTypes::AgentTwo);
  agents->create_behavior_agent("AgentThree", BehaviorTreeTypes::AgentThree);
  agents->create_behavior_agent("AgentFour", BehaviorTreeTypes::AgentFour);

  const auto& allAgents = agents->get_all_agents();

  float rx, rz;
  for (const auto& a : allAgents)
  {
    rx = RNG::range(0.0f, 100.0f);
    rz = RNG::range(0.0f, 100.0f);
    a->set_position(Vec3(rx, 0, rz));

  }
    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees
}