#include "State_Paused.h"
#include "StateManager.h"

State_Paused::State_Paused(StateManager* stateManager) : 
	BaseState(stateManager),
	windowSize(1920, 1080)
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

State_Paused::~State_Paused(){}

void State_Paused::OnCreate(){
	SetTransparent(true); // Set our transparency flag.
	font.loadFromFile("Resources/KOMIKAP_.ttf");
	text.setFont(font);
	text.setString(sf::String("Game Over!"));
	text.setCharacterSize(34);
	text.setStyle(sf::Text::Bold);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	rect.setSize(sf::Vector2f(windowSize));
	rect.setPosition(0,0);
	rect.setFillColor(sf::Color(0,0,0,150));

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Paused,"Key_Space",&State_Paused::Unpause,this);
}

void State_Paused::OnDestroy(){
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Paused,"Key_Space");
}

void State_Paused::Draw(){
	sf::RenderWindow* wind = stateMgr->GetContext()->window->GetRenderWindow();
	wind->draw(rect);
	wind->draw(text);
}

void State_Paused::Unpause(EventDetails* details){
	stateMgr->Remove(StateType::Game);
	stateMgr->Remove(StateType::Paused);
	stateMgr->SwitchTo(StateType::MainMenu); 
}

void State_Paused::Activate(){}
void State_Paused::Deactivate(){}
void State_Paused::Update(const sf::Time& time){}