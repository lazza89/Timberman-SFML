#include "ScoreHandler.h"
#include "StateManager.h"

ScoreHandler::ScoreHandler(StateManager* manager) : 
	stateMgr(manager),
	score(0),
	timeRectSize(400, 50),
	timeShrink(65),
	timesUp(false)
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

	timeRect.setSize(timeRectSize);
	timeRect.setOrigin(timeRect.getSize().x * 0.5, timeRect.getSize().y * 0.5);
	timeRect.setPosition(scoreRect.getPosition().x, scoreRect.getPosition().y - 100);
	timeRect.setFillColor(sf::Color::Green);
}

ScoreHandler::~ScoreHandler()
{}

void ScoreHandler::Draw()
{
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(scoreRect);
	window->draw(scoreText);
	window->draw(timeRect);
}

void ScoreHandler::Update(const sf::Time& time)
{
	if(score != 0)
		timeRectSize = sf::Vector2f(timeRectSize.x - timeShrink * time.asSeconds(), timeRectSize.y);
	
	timeRect.setSize(timeRectSize);
	scoreText.setString("Score: " + std::to_string(score));
	
	if (timeRectSize.x <= 0) {
		timesUp = true;
	}
}

void ScoreHandler::AddScore(int n)
{
	score += n;
	timeRectSize.x += 10;
}

bool ScoreHandler::isTimesUp()
{
	return timesUp;
}
