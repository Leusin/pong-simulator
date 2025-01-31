#pragma once
#include <memory>
#include "FGraphicsDX11Engine.h"
#include "Scene.h"

class SceneManager
{
public:
	void LoadScene(std::shared_ptr<Scene> newScene);
	void Update();
	void Render(FGraphicsDX11Engine& renderer);

private:
	std::shared_ptr<Scene> currentScene;
};