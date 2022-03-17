#include "State_Game.h"
#include "StateManager.h"

State_Game::State_Game(StateManager* stateManager)
	: BaseState(stateManager) {}

State_Game::~State_Game() {}

void State_Game::OnCreate() {
	texture.loadFromFile("Resources/Texture/mushroom.png");
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	increment = sf::Vector2f(400.0f, 400.0f);
	
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);
}

void State_Game::OnDestroy() {
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
}

void State_Game::Update(const sf::Time& time) {
	sf::Vector2u windSize = stateMgr->GetContext()->wind->GetWindowSize();
	sf::Vector2u textSize = texture.getSize();

	if ((sprite.getPosition().x > windSize.x - textSize.x && increment.x > 0) || (sprite.getPosition().x < 0 && increment.x < 0))
	{
		increment.x = -increment.x;
	}

	if ((sprite.getPosition().y > windSize.y - textSize.y && increment.y > 0) || (sprite.getPosition().y < 0 && increment.y < 0))
	{
		increment.y = -increment.y;
	}

	sprite.setPosition(sprite.getPosition().x + (increment.x * time.asSeconds()), sprite.getPosition().y + (increment.y * time.asSeconds()));
}

void State_Game::Draw() {
	stateMgr->GetContext()->wind->GetRenderWindow()->draw(sprite);
}

void State_Game::MainMenu(EventDetails* details) {
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details) {
	stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Activate() {}
void State_Game::Deactivate() {}