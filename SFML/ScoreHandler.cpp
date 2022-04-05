#include "ScoreHandler.h"
#include "StateManager.h"

ScoreHandler::ScoreHandler(StateManager* manager) : 
	stateMgr(manager),
	score(0),
	time(sf::Time::Zero)
{
	//init texture
	stateMgr->GetContext()->textureManager->RequireResource("scoreBoard");

	scoreRect.setSize(sf::Vector2f(400, 100));
	scoreRect.setTexture(stateMgr->GetContext()->textureManager->GetResource("scoreBoard"));
	scoreRect.setOrigin(scoreRect.getSize().x * 0.5, scoreRect.getSize().y * 0.5);
	scoreRect.setPosition(1920 * 0.5, 1080 * 0.85);

	scoreText.setFont(*stateMgr->GetContext()->fontManager->GetResource("Main"));
	scoreText.setCharacterSize(35);
	scoreText.setPosition(scoreRect.getPosition().x - scoreRect.getSize().x * 0.2, scoreRect.getPosition().y - scoreRect.getSize().y * 0.22);


	scoreText.setString("Score: 0");
}

ScoreHandler::~ScoreHandler()
{
	//stateMgr->GetContext()->textureManager->ReleaseResource("scoreBoard");
}

void ScoreHandler::Draw()
{
	stateMgr->GetContext()->window->GetRenderWindow()->draw(scoreRect);
	stateMgr->GetContext()->window->GetRenderWindow()->draw(scoreText);
}

void ScoreHandler::Update(const sf::Time& time)
{
	scoreText.setString("Score: " + std::to_string(score));
}

void ScoreHandler::AddScore(int n)
{
	score += n;
}

void ScoreHandler::StartTime()
{
	clock.restart();
}
