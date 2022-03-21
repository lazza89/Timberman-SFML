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

	sf::Vector2f branchPos(sprite.getPosition().x, 0);
	for (int i = 0; i < 5; i++) {
		branchVector.push_back(std::make_unique<Branch>(stateMgr, branchPos));
		branchPos += sf::Vector2f(0, 100);
	}
}

Tree::~Tree()
{
}

void Tree::Draw()
{
	for (auto& itr : branchVector) {
		itr->Draw();
	}
	window->draw(sprite);
}

void Tree::Update(const sf::Time& deltaTime)
{

}
