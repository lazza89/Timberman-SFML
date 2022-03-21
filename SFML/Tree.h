#pragma once
#include "GameObject.h"
#include "Branch.h"
#include <memory>

class Tree : public GameObject
{
public:
	Tree(StateManager* manager);
	~Tree();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;

private:
	sf::RenderWindow* window;
	sf::Sprite sprite;

	std::vector<std::unique_ptr<Branch>> branchVector;
};

