#pragma once
#include <unordered_map>

class InputManager
{
public:
	void Update();
	bool IsKeyPressed(int keyCode) const;
	bool IsMouseButtonPressed(int button) const;

private:
	std::unordered_map<int, bool> keyStates;
};