#pragma once

#include "FPaddle.h"
#include "FBall.h"

class CollisionManager
{
public:
	bool CheckBallPaddle(FBall & InBall, const FPaddle& InPaddle);

private:
	int GetRectZone(float X, float Y, float minX, float minY, float maxX, float maxY);
};