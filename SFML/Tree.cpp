#include "Tree.h"
#include "StateManager.h"

Tree::Tree(StateManager* manager) :
	GameObject(manager)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	stateMgr->GetContext()->textureManager->RequireResource("tree");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("tree"));
	sprite.setOrigin(sprite.getLocalBounds().width * 0.5, sprite.getLocalBounds().height * 0.5);
	sprite.setPosition(window->getSize().x * 0.5, window->getSize().y * 0.2);
}

Tree::~Tree()
{
}

void Tree::Draw()
{
	window->draw(sprite);
}

void Tree::Update(const sf::Time& deltaTime)
{

}
