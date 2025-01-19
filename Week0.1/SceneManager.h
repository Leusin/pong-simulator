#pragma once
#include <memory>
#include "URenderer.h"
#include "Scene.h"

class SceneManager
{
public:
	void LoadScene(std::shared_ptr<Scene> newScene);
	void Update();
	void Render(URenderer& renderer);

private:
	std::shared_ptr<Scene> currentScene;
};