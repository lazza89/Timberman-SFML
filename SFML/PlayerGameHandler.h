#pragma once
#include <SFML/Graphics.hpp>
#include "Tree.h"
#include "Player.h"


class PlayerGameHandler
{
public:
	PlayerGameHandler(StateManager* manager);
	~PlayerGameHandler();

	void Draw();
	void Update(const sf::Time& deltaTime);

	void ChopLeft();
	void ChopRight();

private:
	StateManager* stateMgr;
	sf::RenderWindow* window;

	Player player;
	Tree tree;

};

