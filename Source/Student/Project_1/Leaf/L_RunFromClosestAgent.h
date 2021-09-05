#pragma once
#include "BehaviorNode.h"

class L_RunFromClosestAgent : public BaseNode<L_RunFromClosestAgent>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};