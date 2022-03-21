#include "Branch.h"
#include "StateManager.h"

Branch::Branch(StateManager* manager, const sf::Vector2f& pos) :
	GameObject(manager),
	position(pos)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	stateMgr->GetContext()->textureManager->RequireResource("branch");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("branch"));
	sprite.setScale(0.8, 0.8);
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

void Branch::SetPosition(const sf::Vector2f& pos)
{
	this->position = pos;
	sprite.setPosition(position);
}

const sf::Vector2f Branch::GetPosition()
{
	return position;
}
