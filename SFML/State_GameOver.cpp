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
	text.setString("Game Over!");
	text.setCharacterSize(45);
	text.setStyle(sf::Text::Bold);
	textSpace = text;
	textSpace.setString("Press SPACE to continue");
	textSpace.setCharacterSize(30);
	textSpace.setOrigin(textSpace.getLocalBounds().width * 0.5, textSpace.getLocalBounds().height * 0.5);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x * 0.5, windowSize.y * 0.35);
	textSpace.setPosition(text.getPosition().x, text.getPosition().y + 50);

	textBackground.setTexture(*stateMgr->GetContext()->textureManager->GetResource("scoreBoard"));
	textBackground.setOrigin(textBackground.getLocalBounds().width * 0.5, textBackground.getLocalBounds().height * 0.5);
	textBackground.setScale(2, 2);
	textBackground.setPosition(text.getPosition().x, text.getPosition().y + 30);

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
	window->draw(textBackground);
	window->draw(text);
	window->draw(textSpace);
}

void State_GameOver::BackToMainMenu(EventDetails* details)
{
	stateMgr->Remove(StateType::Game);
	stateMgr->SwitchTo(StateType::MainMenu);
}
