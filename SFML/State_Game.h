#pragma once
#include <memory>
#include "BaseState.h"
#include "EventManager.h"
#include "Bee.h"
#include "Cloud.h"
#include "Tree.h"
#include "Player.h"

class State_Game : public BaseState{
public:
	State_Game(StateManager* stateManager);
	~State_Game();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(EventDetails* details);
	void Pause(EventDetails* details);

	void MoveLeftAndChop(EventDetails* details);
	void MoveRightAndChop(EventDetails* details);


private:
	sf::Sprite background;
	Tree tree;

	std::vector <std::unique_ptr<Bee>> beeVector;
	std::vector <std::unique_ptr<Cloud>> cloudVector;
};