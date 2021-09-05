#pragma once
#include "BehaviorNode.h"

class D_CheckBlue : public BaseNode<D_CheckBlue>
{
public:

protected:
  virtual void on_update(float dt) override;
  virtual void on_exit() override;
};