#pragma once

#include "IGameObject.h"
#include "FGraphicsDX11Engine.h"

class FPaddle :
    public IGameObject
{
public:
	virtual void Update(float DeltaTime) override;
	virtual void Render(FGraphicsDX11Engine& Renderer) override;

public:
	FVector3 Offset = { 0.0f , 0.0f, 0.0f };
	float HalfWidth = 0.05f; // 하드 코딩
	float HalfHeight = 0.2f; // 하드 코딩
	float Speed = 1.0f;

	bool bMoveUp;
	bool bMoveDown;
};

