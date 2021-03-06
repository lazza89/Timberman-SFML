#pragma once
#include "Window.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "System_Manager.h"
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
	TextureManager textureManager;
	FontManager fontManager;
	AudioManager audioManager;
	SoundManager soundManager;
	SystemManager systemManager;

	sf::Clock clock;
	sf::Time elapsed;
	void RestartClock();
};