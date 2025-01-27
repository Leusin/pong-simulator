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
	FVector3 Offset = { 1.0f , 1.0f, 1.0f };
	float HelfWidth = 0.5f; // �ϵ� �ڵ�
	float Speed = 1.0f;
};

