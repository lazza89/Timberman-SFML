#pragma once
#include "GameObject.h"
#include "Branch.h"
#include <memory>
#include <list>

class Tree : public GameObject
{
public:
	Tree(StateManager* manager);
	~Tree();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;

	void Chop();

private:
	sf::RenderWindow* window;
	sf::Sprite sprite;

	std::list<std::unique_ptr<Branch>> branchVector;
};

