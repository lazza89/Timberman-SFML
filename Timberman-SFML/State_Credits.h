#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Credits : public BaseState{
public:
	State_Credits(StateManager* stateManager);
	~State_Credits();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void Continue(EventDetails* details);
private:
	sf::Sprite creditsSprite;
	sf::Sprite tgui;
	sf::Sprite sfml;
	sf::Text text;
	sf::Text exitText;
	sf::Vector2u windowSize;
};