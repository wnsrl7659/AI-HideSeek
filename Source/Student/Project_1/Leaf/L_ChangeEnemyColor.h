#pragma once
#include "BehaviorNode.h"

class L_ChangeEnemyColor : public BaseNode<L_ChangeEnemyColor>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
  Agent* enemy;
  float c1, c2, c3;
};