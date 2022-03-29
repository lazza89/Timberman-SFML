#include "Game.h"
Game::Game() :
	window("Chapter 5", sf::Vector2u(1280, 720)), 
	stateManager(&context)
{
	clock.restart();
	srand(time(nullptr));

	context.window = &window;
	context.eventManager = window.GetEventManager();
	context.textureManager = &textureManager;
	context.fontManager = &fontManager;

	fontManager.RequireResource("Main");

	stateManager.SwitchTo(StateType::Game);
}

Game::~Game(){ 
	fontManager.ReleaseResource("Main");
}

sf::Time Game::GetElapsed(){ return clock.getElapsedTime(); }
void Game::RestartClock(){ elapsed = clock.restart(); }
Window* Game::GetWindow(){ return &window; }

void Game::Update(){
	window.Update();
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