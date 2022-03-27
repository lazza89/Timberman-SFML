#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_MainMenu : public BaseState{
public:
	State_MainMenu(StateManager* stateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Play(EventDetails* details);
	void Quit(EventDetails* details);

	void Update(const sf::Time& time);
	void Draw();

private:

};