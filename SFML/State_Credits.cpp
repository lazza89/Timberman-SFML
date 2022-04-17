#include "State_Credits.h"
#include "StateManager.h"

State_Credits::State_Credits(StateManager* stateManager)
	: BaseState(stateManager){
	view.reset(sf::FloatRect(0, 0, 1280, 720));
	windowSize = sf::Vector2u(1280, 720);
}

State_Credits::~State_Credits(){}

void State_Credits::OnCreate(){
	stateMgr->GetContext()->textureManager->RequireResource("tgui");
	stateMgr->GetContext()->textureManager->RequireResource("sfml");

	creditsSprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("logo"));
	creditsSprite.setOrigin(creditsSprite.getLocalBounds().width * 0.5, creditsSprite.getLocalBounds().height * 0.5);
	creditsSprite.setPosition(windowSize.x * 0.5, -100);

	text.setFont(*stateMgr->GetContext()->fontManager->GetResource("Main"));
	text.setString("Created by lazza89 with: ");
	text.setCharacterSize(25);
	text.setOrigin(text.getLocalBounds().width * 0.5, text.getLocalBounds().height * 0.5);
	text.setPosition(creditsSprite.getPosition().x, creditsSprite.getPosition().y - 150);

	tgui.setTexture(*stateMgr->GetContext()->textureManager->GetResource("tgui"));
	tgui.setOrigin(tgui.getLocalBounds().width * 0.5, tgui.getLocalBounds().height * 0.5);
	tgui.setScale(0.8, 0.8);

	sfml.setTexture(*stateMgr->GetContext()->textureManager->GetResource("sfml"));
	sfml.setOrigin(sfml.getLocalBounds().width * 0.5, sfml.getLocalBounds().height * 0.5);
	sfml.setScale(0.2, 0.2);

	exitText.setFont(*stateMgr->GetContext()->fontManager->GetResource("Main"));
	exitText.setString("SPACE to exit");
	exitText.setCharacterSize(25);
	exitText.setPosition(20, 670);

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Credits,"Key_Space",&State_Credits::Continue,this);
}

void State_Credits::OnDestroy(){
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Credits,"Key_Space");
}

void State_Credits::Update(const sf::Time& time){
	sf::Vector2u windowSize = stateMgr->GetContext()->window->GetRenderWindow()->getSize();
	
	if (creditsSprite.getPosition().y < 720 * 0.5) {
		creditsSprite.setPosition(creditsSprite.getPosition().x, creditsSprite.getPosition().y + 50 * time.asSeconds());
		text.setPosition(creditsSprite.getPosition().x, creditsSprite.getPosition().y - 150);
		tgui.setPosition(creditsSprite.getPosition().x + 350, creditsSprite.getPosition().y - 150);
		sfml.setPosition(creditsSprite.getPosition().x - 350, creditsSprite.getPosition().y - 150);
	}
}

void State_Credits::Draw(){
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(creditsSprite);
	window->draw(text);
	window->draw(sfml);
	window->draw(tgui);
	window->draw(exitText);
}

void State_Credits::Continue(EventDetails* details){
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Credits::Activate(){
	creditsSprite.setPosition(windowSize.x * 0.5, -100);
}

void State_Credits::Deactivate(){
}