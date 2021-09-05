#pragma once
#include "BehaviorNode.h"

class D_CheckRed : public BaseNode<D_CheckRed>
{
public:

protected:
  virtual void on_update(float dt) override;
  virtual void on_exit() override;
};