#include "State_MainMenu.h"
#include "StateManager.h"

State_MainMenu::State_MainMenu(StateManager* stateManager)
	: BaseState(stateManager) {}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate() {	
	GUI_Manager* gui = stateMgr->GetContext()->guiManager;
	gui->LoadInterface(StateType::MainMenu, "main_menu.interface", "MainMenu");
	gui->GetInterface(StateType::MainMenu, "MainMenu")->SetPosition(sf::Vector2f(100, 100));

	EventManager* eventMgr = stateMgr->GetContext()->eventManager;
	eventMgr->AddCallback(StateType::MainMenu, "MainMenu_Play", &State_MainMenu::Play, this);
	eventMgr->AddCallback(StateType::MainMenu, "MainMenu_Quit", &State_MainMenu::Quit, this);
}


void State_MainMenu::OnDestroy() {
	stateMgr->GetContext()->guiManager->RemoveInterface(StateType::MainMenu, "MainMenu");
	EventManager* eventMgr = stateMgr->GetContext()->eventManager;
	eventMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
	eventMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
}

void State_MainMenu::Activate() {
	auto& play = *stateMgr->GetContext()->guiManager->GetInterface(StateType::MainMenu, "MainMenu")->GetElement("Play");
	
	if (stateMgr->HasState(StateType::Game)) {
		play.SetText("Resume");
	}
	else {
		play.SetText("Play");
	}
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
