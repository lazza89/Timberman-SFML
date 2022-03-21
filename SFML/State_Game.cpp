#include "State_Game.h"
#include "StateManager.h"

State_Game::State_Game(StateManager* stateManager) :
	BaseState(stateManager),
	tree(stateManager)
{}

State_Game::~State_Game() {}

void State_Game::OnCreate() {
	for (int i = 0; i < 20; i++) {
		beeVector.push_back(std::make_unique<Bee>(stateMgr));
	}
	for (int i = 0; i < 3; i++) {
		cloudVector.push_back(std::make_unique<Cloud>(stateMgr));
	}

	stateMgr->GetContext()->textureManager->RequireResource("background");
	background.setTexture(*stateMgr->GetContext()->textureManager->GetResource("background"));

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
	sf::Vector2f targeSize = stateMgr->GetContext()->window->GetRenderWindow()->getView().getSize();
	background.setScale(targeSize.x / background.getLocalBounds().width, targeSize.y / background.getLocalBounds().height);

	tree.Update(time);

	for (auto& itr : cloudVector) {
		itr->Update(time);
	}
	for (auto& itr : beeVector) {
		itr->Update(time);
	}
}

void State_Game::Draw() {
	stateMgr->GetContext()->window->GetRenderWindow()->draw(background);
	for (auto& itr : cloudVector) {
		itr->Draw();
	}
	tree.Draw();
	for (auto& itr : beeVector) {
		itr->Draw();
	}
}

void State_Game::MainMenu(EventDetails* details) {
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details) {
	stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Activate() {}
void State_Game::Deactivate() {}