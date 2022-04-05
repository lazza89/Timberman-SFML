#include "State_GameOver.h"
#include "StateManager.h"

State_GameOver::State_GameOver(StateManager* stateManager) : 
	BaseState(stateManager),
	windowSize(1920, 1080)
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

State_GameOver::~State_GameOver(){}

void State_GameOver::OnCreate(){
	SetTransparent(true); // Set our transparency flag.
	stateMgr->GetContext()->fontManager->RequireResource("Main");
	text.setFont(*stateMgr->GetContext()->fontManager->GetResource("Main"));
	text.setFillColor(sf::Color::Black);
	text.setString(sf::String("Game Over!"));
	text.setCharacterSize(45);
	text.setStyle(sf::Text::Bold);


	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::GameOver, "Key_Space", &State_GameOver::BackToMainMenu, this);
}

void State_GameOver::OnDestroy(){
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Paused, "Key_Space");
}

void State_GameOver::Activate(){}
void State_GameOver::Deactivate(){}

void State_GameOver::Update(const sf::Time& time){

}

void State_GameOver::Draw(){
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(text);
}

void State_GameOver::BackToMainMenu(EventDetails* details)
{
	stateMgr->Remove(StateType::Game);
	stateMgr->Remove(StateType::GameOver);
	stateMgr->SwitchTo(StateType::MainMenu);
}
