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

Tree::~Tree(){}

void Tree::Draw()
{
	for (auto& itr : branchVector) {
		itr->Draw();
	}
	window->draw(sprite);
}

void Tree::Update(const sf::Time& deltaTime){}

sf::Vector2f Tree::GetPosition()
{
	return sprite.getPosition();
}

sf::Vector2f Tree::GetBaseTreePos()
{
	return sprite.getPosition() + sf::Vector2f(0, sprite.getGlobalBounds().height * 0.5);
}

BranchDirection Tree::GetFirstBranchDirection()
{
	auto itr = branchVector.rbegin();
	return (*itr)->GetDirection();
}

const std::list<std::unique_ptr<Branch>>& Tree::GetBranchVector()
{
	return branchVector;
}

void Tree::Chop()
{
	branchVector.pop_back();
	for (auto& itr : branchVector) {
		itr->SetPosition(itr->GetPosition() + sf::Vector2f(0, 100));
	}
	branchVector.push_front(std::make_unique<Branch>(stateMgr, sf::Vector2f(sprite.getPosition().x, 0)));	
}
