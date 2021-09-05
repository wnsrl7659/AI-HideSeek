#pragma once
#include "BehaviorNode.h"

class L_ChaseAgent : public BaseNode<L_ChaseAgent>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};