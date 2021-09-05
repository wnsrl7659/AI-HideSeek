#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_CheckGreen.h"
#include "Decorator/D_CheckBlue.h"
#include "Decorator/D_CheckRed.h"

// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"

// Student Leaf Nodes
#include "Leaf/L_RunFromClosestAgent.h"
#include "Leaf/L_MoveToCenter.h"
#include "Leaf/L_ChaseAgent.h"
#include "Leaf/L_ColorChange.h"
#include "Leaf/L_ChangeEnemyColor.h"
#include "Leaf/L_ChangeEnemySize.h"
#include "Leaf/L_SizeChange.h"