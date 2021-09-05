#pragma once
#include "BehaviorNode.h"

class D_CheckGreen : public BaseNode<D_CheckGreen>
{
public:

protected:
  virtual void on_update(float dt) override;
  virtual void on_exit() override;
};