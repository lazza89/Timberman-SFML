#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include <TGUI/TGUI.hpp>
#include <memory>

class State_Settings : public BaseState
{
public:
	State_Settings(StateManager* manager);
	~State_Settings();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

private:
	tgui::Gui* gui;
	sf::RenderWindow* window;

	tgui::Button::Ptr exitButton;
	tgui::ComboBox::Ptr resolutionBox;
	tgui::Label::Ptr resolutionLabel;

	std::string stringRes;

	void BackToMainMenu();
};

