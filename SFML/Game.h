#pragma once
#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include <iostream>

class Game{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
private:
	SharedContext context;
	Window window;
	StateManager stateManager;
	sf::Clock clock;
	sf::Time elapsed;
	void RestartClock();
};