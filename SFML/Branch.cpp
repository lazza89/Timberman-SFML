#include "Branch.h"
#include "StateManager.h"

Branch::Branch(StateManager* manager, const sf::Vector2f& pos) :
	GameObject(manager),
	position(pos)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	stateMgr->GetContext()->textureManager->RequireResource("branch");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("branch"));
	sprite.setOrigin(0, sprite.getLocalBounds().height * 0.5);

	//random direction
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randomPos(0, 1);
	if (randomPos(mt) == static_cast<int>(BranchDirection::Left)) {
		direction = BranchDirection::Left;
	} else {
		direction = BranchDirection::Right;
	}
	
	if (direction == BranchDirection::Left) {
		sprite.setScale(-0.8, 0.8);
	} else {
		sprite.setScale(0.8, 0.8);
	}

	sprite.setPosition(position);
}

Branch::~Branch()
{
}

void Branch::Draw()
{
	window->draw(sprite);
}

void Branch::Update(const sf::Time& deltaTime)
{

}

void Branch::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
	sprite.setPosition(position);
}

const sf::Vector2f Branch::GetPosition()
{
	return position;
}
