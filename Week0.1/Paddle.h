#pragma once

#include "Entity.h"
#include "FVertexSimple.h"


class Paddle :
    public Entity
{
public:
	float Width;
	float Height;
	float Speed;

	FVector3 Offset;

public:
	Paddle();
	virtual ~Paddle() override;

	void Setup();
	void Shoutdown();

	virtual void Update(float DeltaTime) override;
	virtual void Render(URenderer& Renderer) override;
};

