#include "Game.h"
Game::Game() :
	window("Chapter 5", sf::Vector2u(1280, 720)), 
	stateManager(&context),
	guiManager(window.GetEventManager(), &context)
{
	clock.restart();
	srand(time(nullptr));

	context.window = &window;
	context.eventManager = window.GetEventManager();
	context.textureManager = &textureManager;
	context.fontManager = &fontManager;
	context.guiManager = &guiManager;

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
	stateManager.Update(elapsed);
	context.guiManager->Update(elapsed.asSeconds());
	GUI_Event guiEvent;
	while (context.guiManager->PollEvent(guiEvent)) {
		window.GetEventManager()->HandleEvent(guiEvent);
	}
}

void Game::Render(){
	window.BeginDraw();
	stateManager.Draw();

	sf::View CurrentView = window.GetRenderWindow()->getView();
	window.GetRenderWindow()->setView(window.GetRenderWindow()->getDefaultView());
	context.guiManager->Render(window.GetRenderWindow());
	window.GetRenderWindow()->setView(CurrentView);

	window.EndDraw();
}

void Game::LateUpdate(){
	stateManager.ProcessRequests();
	RestartClock();
}