#include "State_MainMenu.h"
#include "StateManager.h"

State_MainMenu::State_MainMenu(StateManager* stateManager)
	: BaseState(stateManager) {}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate() {	
	EventManager* eventMgr = stateMgr->GetContext()->eventManager;
	eventMgr->AddCallback(StateType::MainMenu, "MainMenu_Play", &State_MainMenu::Play, this);
	eventMgr->AddCallback(StateType::MainMenu, "MainMenu_Quit", &State_MainMenu::Quit, this);
}


void State_MainMenu::OnDestroy() {
	EventManager* eventMgr = stateMgr->GetContext()->eventManager;
	eventMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
	eventMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
}

void State_MainMenu::Activate() {

}


void State_MainMenu::Draw() {
}

void State_MainMenu::Update(const sf::Time& time) {}
void State_MainMenu::Deactivate() {}

void State_MainMenu::Play(EventDetails* details)
{
	stateMgr->SwitchTo(StateType::Game);
}

void State_MainMenu::Quit(EventDetails* details)
{
	stateMgr->GetContext()->window->Close();
}
