#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Window.h"
#include <TGUI/TGUI.hpp>
#include <memory>
#include "SFML/Audio.hpp"

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
	Window* window;

	tgui::Button::Ptr backButton;
	tgui::ComboBox::Ptr resolutionBox;
	tgui::Label::Ptr resolutionLabel;
	tgui::Button::Ptr saveButton;
	tgui::Slider::Ptr volumeSlider;
	tgui::Label::Ptr volumeLabel;

	std::string stringRes;

	sf::Sound buttonSound;
	sf::Sound backButtonSound;
	//sf::Sound sliderSoundTest;

	void BackToMainMenu(EventDetails* event = nullptr);
	void ChangedResolution();
	void SaveSettings();
};

