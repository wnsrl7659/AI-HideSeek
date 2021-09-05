#pragma once
#include "BehaviorNode.h"

class L_ChangeEnemySize : public BaseNode<L_ChangeEnemySize>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
  Agent* enemy;
  float c1, c2, c3;
};