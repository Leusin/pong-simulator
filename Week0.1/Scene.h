#pragma once

#include <vector>
#include <memory>
#include "FGraphicsDX11Engine.h"

class IGameObejct;

class Scene
{
public:
	void Initialize();
	void Update();
	void Render(FGraphicsDX11Engine& renderer);

private:
	std::vector<std::shared_ptr<IGameObejct>> entities;
};