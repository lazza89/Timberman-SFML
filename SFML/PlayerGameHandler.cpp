#include "PlayerGameHandler.h"
#include "StateManager.h"

PlayerGameHandler::PlayerGameHandler(StateManager* manager) : 
	stateMgr(manager),
	player(manager),
	tree(manager),
	isDead(false)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	player.SetPosition(tree.GetBaseTreePos() - sf::Vector2f(200, 100), PlayerDirection::Left);

}

PlayerGameHandler::~PlayerGameHandler(){}


void PlayerGameHandler::Draw()
{
	tree.Draw();
	player.Draw();
}

void PlayerGameHandler::Update(const sf::Time& deltaTime)
{

}

void PlayerGameHandler::ChopLeft()
{
	player.SetPosition(tree.GetBaseTreePos() - sf::Vector2f(200, 100), PlayerDirection::Left);
	if (tree.GetFirstBranchDirection() == BranchDirection::Left && player.GetDirection() == PlayerDirection::Left) {
		std::cout << "dead" << std::endl;
		isDead = true;
	}
	tree.Chop();
}

void PlayerGameHandler::ChopRight()
{
	player.SetPosition(tree.GetBaseTreePos() + sf::Vector2f(200, -100), PlayerDirection::Right);
	if (tree.GetFirstBranchDirection() == BranchDirection::Right && player.GetDirection() == PlayerDirection::Right) {
		std::cout << "dead" << std::endl;
		isDead = true;
	}
	tree.Chop();
}

bool PlayerGameHandler::IsDead()
{
	return isDead;
}


