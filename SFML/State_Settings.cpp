#include "State_Settings.h"
#include "StateManager.h"

State_Settings::State_Settings(StateManager* manager) : 
	BaseState(manager),
	stringRes("1280 x 720")
{
	window = stateMgr->GetContext()->window;
	gui = stateMgr->GetContext()->window->GetGui();
}

State_Settings::~State_Settings(){}

void State_Settings::OnCreate()
{
	sf::Vector2u windowPos = window->GetRenderWindow()->getSize();

	exitButton = tgui::Button::create();
	exitButton->setSize(tgui::Layout2d(sf::Vector2f(40, 40)));
	exitButton->setPosition(tgui::Layout2d(sf::Vector2f(windowPos.x - exitButton->getSize().x - 20, 20)));
	exitButton->onPress(&State_Settings::BackToMainMenu, this);
	exitButton->setText("X");

	resolutionBox = tgui::ComboBox::create();
	resolutionBox->addItem("1920 x 1080");
	resolutionBox->addItem("1280 x 720");
	resolutionBox->setSelectedItem("1280 x 720");
	resolutionBox->setSize(tgui::Layout2d(sf::Vector2f(250, 50)));
	resolutionBox->setTextSize(20);
	resolutionBox->setOrigin(0.5, 0.5);
	resolutionBox->setPosition(tgui::Layout2d(sf::Vector2f(windowPos.x * 0.5, windowPos.y * 0.5)));

	resolutionLabel = tgui::Label::create();
	resolutionLabel->setText("Game Resolution");
	resolutionLabel->setTextSize(30);
	resolutionLabel->setOrigin(0.5, 0.5);
	resolutionLabel->setPosition(tgui::Layout2d(sf::Vector2f(resolutionBox->getPosition().x, resolutionBox->getPosition().y - resolutionLabel->getSize().y - 70)));


	gui->add(exitButton);
	gui->add(resolutionBox);
	gui->add(resolutionLabel);
}

void State_Settings::OnDestroy()
{
}

void State_Settings::Activate()
{
	exitButton->setVisible(true);
	resolutionBox->setVisible(true);
	resolutionLabel->setVisible(true);
}

void State_Settings::Deactivate()
{
	exitButton->setVisible(false);
	resolutionBox->setVisible(false);
	resolutionLabel->setVisible(false);
}

void State_Settings::Update(const sf::Time& time)
{


	if (resolutionBox->getSelectedItem() != stringRes) {
		tgui::String tmp = resolutionBox->getSelectedItem();
		
		if (tmp == "1920 x 1080") {
			window->ChangeResolution(sf::Vector2u(1920, 1080));
			stringRes = "1920 x 1080";
		}
		else if (tmp == "1280 x 720") {
			window->ChangeResolution(sf::Vector2u(1280, 720));
			stringRes = "1280 x 720";
		}
	}

}

void State_Settings::Draw()
{
	gui->draw();
}

void State_Settings::BackToMainMenu()
{
	stateMgr->SwitchTo(StateType::MainMenu);
}
