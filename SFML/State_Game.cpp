#include "State_Game.h"
#include "StateManager.h"

State_Game::State_Game(StateManager* stateManager) :
	BaseState(stateManager),
	playerGame(stateManager),
	score(stateManager),
	mt(rd()),
	randomPitch(0.8f, 1.8f)
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

State_Game::~State_Game() {}

void State_Game::OnCreate() {

	AudioManager* audioMgr = stateMgr->GetContext()->audioManager;
	audioMgr->RequireResource("Pop1");
	audioMgr->RequireResource("Death");

	logPopSound.setBuffer(*audioMgr->GetResource("Pop1"));
	logPopSound.setPitch(randomPitch(mt));

	deathSound.setBuffer(*audioMgr->GetResource("Death"));

	for (int i = 0; i < 4; i++) {
		beeVector.push_back(std::make_unique<Bee>(stateMgr));
	}
	for (int i = 0; i < 3; i++) {
		cloudVector.push_back(std::make_unique<Cloud>(stateMgr));
	}

	stateMgr->GetContext()->textureManager->RequireResource("arrow");
	arrowSprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("arrow"));
	arrowSprite.setOrigin(arrowSprite.getLocalBounds().width * 0.5, arrowSprite.getLocalBounds().height * 0.5);
	arrowSprite.setPosition(1920 * 0.5, 1080* 0.83);
	arrowSprite.setScale(1.7, 1.7);

	//background stuff
	stateMgr->GetContext()->textureManager->RequireResource("background");
	background.setTexture(*stateMgr->GetContext()->textureManager->GetResource("background"));

	stateMgr->GetContext()->textureManager->RequireResource("trees");
	trees.setTexture(*stateMgr->GetContext()->textureManager->GetResource("trees"));

	sf::Vector2f targeSize = view.getSize();
	background.setScale(targeSize.x / background.getLocalBounds().width, targeSize.y / background.getLocalBounds().height);
	trees.setScale(targeSize.x / background.getLocalBounds().width, targeSize.y / background.getLocalBounds().height);

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_Left", &State_Game::MoveLeftAndChop, this);
	evMgr->AddCallback(StateType::Game, "Key_Right", &State_Game::MoveRightAndChop, this);
	evMgr->AddCallback(StateType::Game, "Key_P", &State_Game::Pause, this);

}

void State_Game::OnDestroy() {
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_Left");
	evMgr->RemoveCallback(StateType::Game, "Key_Right");
}

void State_Game::Update(const sf::Time& time) {
	score.Update(time);

	if (playerGame.IsDead() || score.isTimesUp()) {
		deathSound.play();
		stateMgr->SwitchTo(StateType::GameOver);
	}

	for (auto& itr : cloudVector) {
		itr->Update(time);
	}
	playerGame.Update(time);
	for (auto& itr : beeVector) {
		itr->Update(time);
	}
}

void State_Game::Draw() {
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();

	window->draw(background);
	for (auto& itr : cloudVector) {
		itr->Draw();
	}
	window->draw(trees);
	playerGame.Draw();
	for (auto& itr : beeVector) {
		itr->Draw();
	}

	if (score.GetScore() == 0) {
		window->draw(arrowSprite);
	}
	else {
		score.Draw();
	}
}

void State_Game::MainMenu(EventDetails* details) {
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* details) {
	stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::MoveLeftAndChop(EventDetails* details)
{
	playerGame.ChopLeft();
	score.AddScore(1);
	logPopSound.setPitch(randomPitch(mt));
	logPopSound.play();
}

void State_Game::MoveRightAndChop(EventDetails* details)
{
	playerGame.ChopRight();
	score.AddScore(1);
	logPopSound.setPitch(randomPitch(mt));
	logPopSound.play();
}

void State_Game::Activate() {
	logPopSound.setVolume(stateMgr->GetContext()->generalVolume);
	deathSound.setVolume(stateMgr->GetContext()->generalVolume);
}
void State_Game::Deactivate() {

}