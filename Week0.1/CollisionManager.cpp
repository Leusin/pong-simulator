#include "CollisionManager.h"
#include <algorithm> // min, max


bool CollisionManager::CheckBallPaddle(FBall & InBall, const FPaddle& InPaddle)
{
	// �簢���� �ּ�/�ִ� ��ǥ
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
	// top, bottom ���� ��������, ���� ���Ϳ� �����Ÿ��� �˻��Ѵ�.
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

	// left, right ���� ��������. ���� ���Ϳ� ����Ÿ��� �˻��Ѵ�.
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

	// �簢�� ������ ����
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

		// �𼭸� ����, �𼭸��� ���� ���ο� ���ԵǴ��� �˻��Ѵ�.
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

		// �浹 ���� Ȯ��
		if (distanceSquared <= radiusSquared)
		{
			// ���� ��ġ ���� (�𼭸� �ٱ����� �̵�)
			float distance = (float)sqrt(distanceSquared);
			float overlap = InBall.Radius * InBall.Scale - distance;

			// ���� ���� ���� (�𼭸����� ������ �о)
			float normalX = -distanceX / distance;
			float normalY = -distanceY / distance;

			if (overlap > 0.0f)
			{
				InBall.Offset.x += normalX * overlap;
				InBall.Offset.y += normalY * overlap;
			}

			// ������ �ӵ��� ���� (�𼭸��� ���� �� �������� ƨ��)
			if (fabs(distanceX) > fabs(distanceY))
			{
				InBall.Velocity.x *= -1.0f; // X�� ����
			}
			else
			{
				InBall.Velocity.y *= -1.0f; // Y�� ����
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
