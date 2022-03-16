#pragma once
#include "BaseState.h"
#include <SFML/Graphics.hpp>

class State_GameOver : public BaseState{
public:
	State_GameOver(StateManager* stateManager);
	~State_GameOver();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();
private:
	sf::Font font;
	sf::Text text;
	float elapsed;
};