#include "State_MainMenu.h"
#include "StateManager.h"

State_MainMenu::State_MainMenu(StateManager* stateManager) : 
	BaseState(stateManager),
	window(nullptr)
{
	window = stateMgr->GetContext()->window;
	gui = stateMgr->GetContext()->window->GetGui();
}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate() {	
	AudioManager* audioMgr = stateMgr->GetContext()->audioManager;
	audioMgr->RequireResource("ButtonSound");

	buttonClicked.setBuffer(*stateMgr->GetContext()->audioManager->GetResource("ButtonSound"));

	gui->setFont("Resources/KOMIKAP_.ttf");

	playButton = tgui::Button::create();
	playButton->setSize(tgui::Layout2d(sf::Vector2f(200, 50)));
	playButton->setOrigin(sf::Vector2f(0.5, 0.5));
	playButton->setPosition(tgui::Layout2d(sf::Vector2f(window->GetRenderWindow()->getSize().x * 0.5, window->GetRenderWindow()->getSize().y * 0.5)));
	playButton->setText("Play");
	playButton->setTextSize(20);

	settingsButton = tgui::Button::create();
	settingsButton->setSize(tgui::Layout2d(sf::Vector2f(200, 50)));
	settingsButton->setOrigin(sf::Vector2f(0.5, 0.5));
	settingsButton->setPosition(tgui::Layout2d(playButton->getPosition().x, playButton->getPosition().y + 50));
	settingsButton->setText("Settings");
	settingsButton->setTextSize(20);

	creditsButton = tgui::Button::create();
	creditsButton->setSize(tgui::Layout2d(sf::Vector2f(200, 50)));
	creditsButton->setOrigin(sf::Vector2f(0.5, 0.5));
	creditsButton->setPosition(tgui::Layout2d(settingsButton->getPosition().x, settingsButton->getPosition().y + 50));
	creditsButton->setText("Credits");
	creditsButton->setTextSize(20);

	quitButton = tgui::Button::create();
	quitButton->setSize(tgui::Layout2d(sf::Vector2f(200, 50)));
	quitButton->setOrigin(sf::Vector2f(0.5, 0.5));
	quitButton->setPosition(tgui::Layout2d(creditsButton->getPosition().x, creditsButton->getPosition().y + 50));
	quitButton->setText("Quit");
	quitButton->setTextSize(20);

	//button signals
	playButton->onPress(&State_MainMenu::ChangeState, this, StateType::Game);
	quitButton->onPress(&State_MainMenu::Quit, this);
	settingsButton->onPress(&State_MainMenu::ChangeState, this, StateType::Settings);
	creditsButton->onPress(&State_MainMenu::ChangeState, this, StateType::Credits);

	playButton->onClick([&]() { buttonClicked.play();});
	settingsButton->onClick([&]() { buttonClicked.play(); });
	creditsButton->onClick([&]() { buttonClicked.play(); });

	gui->add(playButton);
	gui->add(settingsButton);
	gui->add(creditsButton);
	gui->add(quitButton);

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::MainMenu, "Key_Space", &State_MainMenu::PlayWithKeyboard, this);
}


void State_MainMenu::OnDestroy() {
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Space");
}

void State_MainMenu::Activate() {
	buttonClicked.setVolume(stateMgr->GetContext()->generalVolume);

	playButton->setVisible(true);
	settingsButton->setVisible(true);
	quitButton->setVisible(true);
	creditsButton->setVisible(true);

	playButton->setPosition(tgui::Layout2d(sf::Vector2f(window->GetRenderWindow()->getSize().x * 0.5, window->GetRenderWindow()->getSize().y * 0.5)));
	settingsButton->setPosition(tgui::Layout2d(playButton->getPosition().x, playButton->getPosition().y + 50));
	creditsButton->setPosition(tgui::Layout2d(settingsButton->getPosition().x, settingsButton->getPosition().y + 50));
	quitButton->setPosition(tgui::Layout2d(creditsButton->getPosition().x, creditsButton->getPosition().y + 50));

	if (stateMgr->HasState(StateType::Game)) {
		playButton->setText("Resume");
	}
	else {
		playButton->setText("Play");
	}
}

void State_MainMenu::Deactivate() {
	playButton->setVisible(false);
	settingsButton->setVisible(false);
	quitButton->setVisible(false);
	creditsButton->setVisible(false);
}

void State_MainMenu::Draw() {
	gui->draw();
}

void State_MainMenu::Update(const sf::Time& time) {}

void State_MainMenu::PlayWithKeyboard(EventDetails* details)
{
	buttonClicked.play();
	ChangeState(StateType::Game);
}

void State_MainMenu::ChangeState(const StateType& state)
{
	stateMgr->SwitchTo(state);
}

void State_MainMenu::Quit()
{
	stateMgr->GetContext()->window->Close();
}
