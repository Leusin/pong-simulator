#include "CollisionManager.h"
#include <algorithm> // min, max


bool CollisionManager::CheckBallPaddle(FBall & InBall, const FPaddle& InPaddle)
{
	// 사각형의 최소/최대 좌표
	float PaddleMinX 
		= InPaddle.Offset.x - InPaddle.HalfWidth;
	float PaddleMaxX 
		= InPaddle.Offset.x + InPaddle.HalfWidth;
	float PaddleMinY 
		= InPaddle.Offset.y - InPaddle.HalfHeight;
	float PaddleMaxY 
		= InPaddle.Offset.y + InPaddle.HalfHeight;
	
	int Zone = GetRectZone(InBall.Offset.x, InBall.Offset.y, 
		PaddleMinX, PaddleMinY, PaddleMaxX, PaddleMaxY);

	switch (Zone)
	{
	// top, bottom 변의 영역에서, 원의 센터와 수직거리를 검사한다.
	case 1:
	case 7:
	{
		float DistY = (float)fabs(InBall.Offset.y - InPaddle.Offset.y);
		if (DistY <= (InBall.Radius * InBall.Scale + InPaddle.HalfHeight))
		{
			InBall.Velocity.y *= -1.0f;
			if (InBall.Offset.y > InPaddle.Offset.y)
			{
				InBall.Offset.y = InPaddle.Offset.y + (InPaddle.HalfHeight + InBall.Radius * InBall.Scale);
			}
			else
			{
				InBall.Offset.y = InPaddle.Offset.y - (InPaddle.HalfHeight + InBall.Radius * InBall.Scale);
			}
			return true;
		}
	}
	break;

	// left, right 변의 영역에서. 원의 센터와 수평거리를 검사한다.
	case 3:
	case 5:
	{
		float distX = (float)fabs(InBall.Offset.x - InPaddle.Offset.x);
		if (distX <= InBall.Radius * InBall.Scale + InPaddle.HalfHeight)
		{
			InBall.Velocity.x *= -1.0f;
			if (InBall.Offset.x < InPaddle.Offset.x) 
			{
				InBall.Offset.x = InPaddle.Offset.x - (InPaddle.HalfWidth + InBall.Radius * InBall.Scale);
			}
			else 
			{
				InBall.Offset.x = InPaddle.Offset.x + (InPaddle.HalfWidth + InBall.Radius * InBall.Scale);
			}

			return true;
		}

	}
	break;

	// 사각형 영역의 내부
	case 4:

		if (InBall.Offset.x < InPaddle.Offset.x)
		{
			InBall.Offset.x = InPaddle.Offset.x - (InPaddle.HalfWidth + InBall.Radius * InBall.Scale);
		}
		else
		{
			InBall.Offset.x = InPaddle.Offset.x + (InPaddle.HalfWidth + InBall.Radius * InBall.Scale);
		}

		if (InBall.Offset.y < InPaddle.Offset.y)
		{
			InBall.Offset.y = InPaddle.Offset.y - (InPaddle.HalfHeight + InBall.Radius * InBall.Scale);
		}
		else
		{
			InBall.Offset.y = InPaddle.Offset.y + (InPaddle.HalfHeight + InBall.Radius * InBall.Scale);
		}

		return true;
		break;

		// 모서리 영역, 모서리가 원의 내부에 포함되는지 검사한다.
	default:

	{
		float cornerX = (Zone == 0 || Zone == 6) ? 
			InPaddle.Offset.x - InPaddle.HalfWidth : 
			InPaddle.Offset.x + InPaddle.HalfWidth;

		float cornerY = (Zone == 0 || Zone == 2) ? 
			InPaddle.Offset.y - InPaddle.HalfHeight :
			InPaddle.Offset.y + InPaddle.HalfHeight;

		float distanceX = cornerX - InBall.Offset.x;
		float distanceY = cornerY - InBall.Offset.y;

		float distanceSquared = 
			(distanceX * distanceX) + (distanceY * distanceY);
		float radiusSquared = 
			InBall.Radius * InBall.Scale * InBall.Radius * InBall.Scale;

		// 충돌 여부 확인
		if (distanceSquared <= radiusSquared)
		{
			// 공의 위치 조정 (모서리 바깥으로 이동)
			float distance = (float)sqrt(distanceSquared);
			float overlap = InBall.Radius * InBall.Scale - distance;

			// 방향 벡터 반전 (모서리에서 공으로 밀어냄)
			float normalX = -distanceX / distance;
			float normalY = -distanceY / distance;

			if (overlap > 0.0f)
			{
				InBall.Offset.x += normalX * overlap;
				InBall.Offset.y += normalY * overlap;
			}

			// 간단히 속도를 반전 (모서리에 대해 축 방향으로 튕김)
			if (fabs(distanceX) > fabs(distanceY))
			{
				InBall.Velocity.x *= -1.0f; // X축 반전
			}
			else
			{
				InBall.Velocity.y *= -1.0f; // Y축 반전
			}

			return true;
		}

		return false;
	}
	break;

	}

	return false;
}

//   6  |  7  |  8   
// -----|-----|------
//   3  |  4  |  5   
// -----|-----|------
//   0  |  1  |  2   

int CollisionManager::GetRectZone(
	float X, float Y, 
	float minX, float minY, 
	float maxX, float maxY)
{
	int xZone = (X < minX) ? 0 :
		(X > maxX) ? 2 : 1;

	int yZone = (Y < minY) ? 0 :
		(Y > maxY) ? 2 : 1;

	int nZone = xZone + 3 * yZone;

	return nZone;
}
