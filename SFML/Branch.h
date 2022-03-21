#pragma once
#include "GameObject.h"

enum class BranchDirection {
	Left,
	Right
};

class Branch : public GameObject
{
public:
	Branch(StateManager* manager, const sf::Vector2f& pos);
	~Branch();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;

	void SetPosition(const sf::Vector2f& position);
	const sf::Vector2f GetPosition();

private:
	sf::Vector2f position;
	BranchDirection direction;
	sf::RenderWindow* window;
	sf::Sprite sprite;

};

