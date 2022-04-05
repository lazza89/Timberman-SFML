#pragma once
#include "BaseState.h"
#include "EventManager.h"

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
	sf::Vector2u windowSize;
	sf::Font font;
	sf::Text text;

	void BackToMainMenu(EventDetails* details);
};