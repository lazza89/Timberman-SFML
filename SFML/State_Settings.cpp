#include "State_Settings.h"
#include "StateManager.h"

State_Settings::State_Settings(StateManager* manager) : 
	BaseState(manager),
	stringRes("1280 x 720")
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window = stateMgr->GetContext()->window;
	gui = stateMgr->GetContext()->window->GetGui();
}

State_Settings::~State_Settings(){}

void State_Settings::OnCreate()
{
	sf::Vector2u windowPos = window->GetRenderWindow()->getSize();

	exitButton = tgui::Button::create();
	exitButton->setSize(150, 50);
	exitButton->setPosition(50, windowPos.y * 0.9);
	exitButton->onPress(&State_Settings::BackToMainMenu, this);
	exitButton->setTextSize(22);
	exitButton->setText("Back");

	resolutionBox = tgui::ComboBox::create();
	resolutionBox->addItem("1920 x 1080");
	resolutionBox->addItem("1280 x 720");
	resolutionBox->addItem("800 x 600");
	resolutionBox->setSelectedItem("1280 x 720");
	resolutionBox->setSize(250, 50);
	resolutionBox->setTextSize(20);
	resolutionBox->setOrigin(0.5, 0.5);
	resolutionBox->setPosition(windowPos.x * 0.5, windowPos.y * 0.5);

	resolutionLabel = tgui::Label::create();
	resolutionLabel->setText("Game Resolution");
	resolutionLabel->setTextSize(30);
	resolutionLabel->setOrigin(0.5, 0.5);
	resolutionLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y - resolutionLabel->getSize().y - 70);

	saveButton = tgui::Button::create();
	saveButton->setSize(150, 50);
	saveButton->setText("Save");
	saveButton->setTextSize(22);
	saveButton->setPosition(windowPos.x - 200, windowPos.y * 0.9);
	saveButton->onPress(&State_Settings::SaveSettings, this);

	gui->add(exitButton);
	gui->add(saveButton);
	gui->add(resolutionBox);
	gui->add(resolutionLabel);
}

void State_Settings::OnDestroy()
{
}

void State_Settings::Activate()
{
	exitButton->setVisible(true);
	saveButton->setVisible(true);
	resolutionBox->setVisible(true);
	resolutionLabel->setVisible(true);
}

void State_Settings::Deactivate()
{
	exitButton->setVisible(false);
	saveButton->setVisible(false);
	resolutionBox->setVisible(false);
	resolutionLabel->setVisible(false);
}

void State_Settings::Update(const sf::Time& time)
{

}

void State_Settings::Draw()
{
	gui->draw();
}

void State_Settings::BackToMainMenu()
{
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Settings::ChangedResolution()
{
	sf::Vector2u windowPos = window->GetRenderWindow()->getSize();
	exitButton->setPosition(50, windowPos.y * 0.9);
	resolutionBox->setPosition(windowPos.x * 0.5, windowPos.y * 0.5);
	resolutionLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y - resolutionLabel->getSize().y - 70);
	saveButton->setPosition(windowPos.x -200, windowPos.y * 0.9);
}

void State_Settings::SaveSettings()
{
	if (resolutionBox->getSelectedItem() != stringRes) {
		tgui::String tmp = resolutionBox->getSelectedItem();

		if (tmp == "1920 x 1080") {
			window->ChangeResolution(sf::Vector2u(1920, 1080));
			ChangedResolution();
			stringRes = "1920 x 1080";
		}
		else if (tmp == "1280 x 720") {
			window->ChangeResolution(sf::Vector2u(1280, 720));
			ChangedResolution();
			stringRes = "1280 x 720";
		}
		else if (tmp == "800 x 600") {
			window->ChangeResolution(sf::Vector2u(800, 600));
			ChangedResolution();
			stringRes = "800 x 600";
		}
	}
}
