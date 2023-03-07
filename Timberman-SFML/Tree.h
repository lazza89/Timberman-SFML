#pragma once
#include "GameObject.h"
#include "Branch.h"
#include "FlyingLog.h"
#include <memory>
#include <list>

class Tree : public GameObject
{
public:
	Tree(StateManager* manager);
	~Tree();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;

	sf::Vector2f GetPosition();
	sf::Vector2f GetBaseTreePos();
	BranchDirection GetFirstBranchDirection();
	const std::list<std::unique_ptr<Branch>>& GetBranchVector();
	void ChopLeft();
	void ChopRight();

private:
	Window* window;
	sf::Sprite sprite;

	void Chop();
	std::list<FlyingLog> logList;
	float logLifeTime;
	std::list<std::unique_ptr<Branch>> branchVector;
};

