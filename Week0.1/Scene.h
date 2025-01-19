#pragma once
#include <vector>
#include <memory>
#include "URenderer.h"

class Entity;

class Scene
{
public:
	void Initialize();
	void Update();
	void Render(URenderer& renderer);

private:
	std::vector<std::shared_ptr<Entity>> entities;
};