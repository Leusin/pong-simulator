#pragma once
#include "URenderer.h"

class Entity
{
public:
	virtual void Update() = 0;
	virtual void Render(URenderer& renderer) = 0;
};
