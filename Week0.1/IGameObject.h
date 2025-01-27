#pragma once

#include "FGraphicsDX11Engine.h"

class IGameObject
{
public:
	virtual void Update(float DeltaTime) abstract;
	virtual void Render(FGraphicsDX11Engine& renderer) abstract;
};
