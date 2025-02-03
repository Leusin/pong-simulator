#pragma once

#include "IGameObject.h"

class FBall :
    public IGameObject
{
public:
	virtual void Update(float DeltaTime) override;
	virtual void Render(FGraphicsDX11Engine& Renderer) override;

public:
	FBall();

	FVector3 Offset = { 0.0f , 0.0f, 0.0f };
	FVector3 Velocity = { 0.0f , 0.0f, 0.0f };
	float Speed = 0.05f;
	float Radius = 1.0f; // 하드 코딩
	float Scale = 0.1f;
};