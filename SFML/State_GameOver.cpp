#include "State_GameOver.h"
#include "StateManager.h"

State_GameOver::State_GameOver(StateManager* stateManager)
	: BaseState(stateManager){}

State_GameOver::~State_GameOver(){}

void State_GameOver::OnCreate(){
	elapsed = 0;
	font.loadFromFile("Resources/KOMIKAP_.ttf");
	text.setFont(font);
	text.setCharacterSize(16);
	text.setString("You beat the game! Congratulations!");
	text.setFillColor(sf::Color::White);
	text.setOrigin(text.getLocalBounds().width / 2, 
		text.getLocalBounds().height / 2);
	text.setPosition(400, 300);

	stateMgr->Remove(StateType::Game);
}

void State_GameOver::OnDestroy(){}

void State_GameOver::Activate(){}
void State_GameOver::Deactivate(){}

void State_GameOver::Update(const sf::Time& time){
	elapsed += time.asSeconds();
	if(elapsed >= 5.0f){
		stateMgr->Remove(StateType::GameOver);
		stateMgr->SwitchTo(StateType::MainMenu);
	}
}

void State_GameOver::Draw(){
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(text);
}