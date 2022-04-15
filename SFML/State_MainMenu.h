#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "TGUI/TGUI.hpp"
#include "Window.h"
#include "SFML/Audio.hpp"

class State_MainMenu : public BaseState{
public:
	State_MainMenu(StateManager* stateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

private:
	tgui::Gui* gui;
	Window* window;

	sf::Sprite timbermanLogo;

	tgui::Button::Ptr playButton;
	tgui::Button::Ptr quitButton;
	tgui::Button::Ptr settingsButton;
	tgui::Button::Ptr creditsButton;

	sf::Sound buttonClicked;

	void PlayWithKeyboard(EventDetails* details);
	void ChangeState(const StateType& state);
	void Quit();
};