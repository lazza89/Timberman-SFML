#include "Game.h"
Game::Game() :
	window("Timberman SFML", sf::Vector2u(1280, 720)), 
	stateManager(&context),
	soundManager(&audioManager)
{
	clock.restart();
	srand(time(nullptr));

	context.window = &window;
	context.eventManager = window.GetEventManager();
	context.textureManager = &textureManager;
	context.fontManager = &fontManager;
	context.audioManager = &audioManager;
	context.soundManager = &soundManager;

	systemManager.GetSystem<S_Sound>(System::Sound)->SetUp(&audioManager, &soundManager);

	fontManager.RequireResource("Main");

	stateManager.SwitchTo(StateType::MainMenu);
}

Game::~Game(){ 
	fontManager.ReleaseResource("Main");
}

sf::Time Game::GetElapsed(){ return clock.getElapsedTime(); }
void Game::RestartClock(){ elapsed = clock.restart(); }
Window* Game::GetWindow(){ return &window; }

void Game::Update(){
	window.Update();
	soundManager.Update(elapsed.asSeconds());
	stateManager.Update(elapsed);
}

void Game::Render(){
	window.BeginDraw();
	stateManager.Draw();

	sf::View CurrentView = window.GetRenderWindow()->getView();
	window.GetRenderWindow()->setView(window.GetRenderWindow()->getDefaultView());
	window.GetRenderWindow()->setView(CurrentView);

	window.EndDraw();
}

void Game::LateUpdate(){
	stateManager.ProcessRequests();
	RestartClock();
}