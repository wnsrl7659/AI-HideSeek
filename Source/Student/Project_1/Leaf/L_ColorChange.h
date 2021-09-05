#pragma once
#include "BehaviorNode.h"

class L_ColorChange : public BaseNode<L_ColorChange>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
  float c1, c2, c3;
};