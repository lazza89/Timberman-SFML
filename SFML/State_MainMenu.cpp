#include "State_MainMenu.h"
#include "StateManager.h"

State_MainMenu::State_MainMenu(StateManager* stateManager)
	: BaseState(stateManager) {}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate() {
	font.loadFromFile("Resources/KOMIKAP_.ttf");
	text.setFont(font);
	text.setString(sf::String("MAIN MENU:"));
	text.setCharacterSize(18);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	text.setPosition(400, 100);

	buttonSize = sf::Vector2f(300.0f, 32.0f);
	buttonPos = sf::Vector2f(400, 200);
	buttonPadding = 4; // 4px.

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for (int i = 0; i < 3; ++i) {
		sf::Vector2f buttonPosition(buttonPos.x, buttonPos.y +
			(i * (buttonSize.y + buttonPadding)));
		rects[i].setSize(buttonSize);
		rects[i].setFillColor(sf::Color::Red);

		rects[i].setOrigin(buttonSize.x / 2.0f, buttonSize.y / 2.0f);
		rects[i].setPosition(buttonPosition);

		labels[i].setFont(font);
		labels[i].setString(sf::String(str[i]));
		labels[i].setCharacterSize(12);

		sf::FloatRect rect = labels[i].getLocalBounds();
		labels[i].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);

		labels[i].setPosition(buttonPosition);
	}

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::MainMenu, "Mouse_Left", &State_MainMenu::MouseClick, this);
}

void State_MainMenu::OnDestroy() {
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void State_MainMenu::Activate() {
	if (stateMgr->HasState(StateType::Game)
		&& labels[0].getString() == "PLAY")
	{
		labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = labels[0].getLocalBounds();
		labels[0].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}
}

void State_MainMenu::MouseClick(EventDetails* details) {
	sf::Vector2i mousePos = details->mouse;

	float halfX = buttonSize.x / 2.0f;
	float halfY = buttonSize.y / 2.0f;
	for (int i = 0; i < 3; ++i) {
		if (mousePos.x >= rects[i].getPosition().x - halfX &&
			mousePos.x <= rects[i].getPosition().x + halfX &&
			mousePos.y >= rects[i].getPosition().y - halfY &&
			mousePos.y <= rects[i].getPosition().y + halfY)
		{
			if (i == 0) {
				stateMgr->SwitchTo(StateType::Game);
			}
			else if (i == 1) {
				// Credits state.
			}
			else if (i == 2) {
				stateMgr->GetContext()->window->Close();
			}
		}
	}
}

void State_MainMenu::Draw() {
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(text);
	for (int i = 0; i < 3; ++i) {
		window->draw(rects[i]);
		window->draw(labels[i]);
	}
}

void State_MainMenu::Update(const sf::Time& time) {}
void State_MainMenu::Deactivate() {}