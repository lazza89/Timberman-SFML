#include "Game.h"
Game::Game(): window("Chapter 5", sf::Vector2u(800, 600)), stateManager(&context){
	clock.restart();
	srand(time(nullptr));

	context.wind = &window;
	context.eventManager = window.GetEventManager();

	stateManager.SwitchTo(StateType::Intro);
}

Game::~Game(){ }

sf::Time Game::GetElapsed(){ return clock.getElapsedTime(); }
void Game::RestartClock(){ elapsed = clock.restart(); }
Window* Game::GetWindow(){ return &window; }

void Game::Update(){
	window.Update();
	stateManager.Update(elapsed);
}

void Game::Render(){
	window.BeginDraw();
	// Render here.
	stateManager.Draw();
	window.EndDraw();
}

void Game::LateUpdate(){
	stateManager.ProcessRequests();
	RestartClock();
}