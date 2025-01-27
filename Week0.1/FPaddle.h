#pragma once

#include "IGameObejct.h"

class FPaddle :
    public IGameObejct
{
public:
	FPaddle();

	virtual void Update(float DeltaTime) override;
	virtual void Render(FGraphicsDX11Engine& Renderer) override;

public:
	FVector3 Offset;
	float Width;
	float Height;
	float Speed;

};

