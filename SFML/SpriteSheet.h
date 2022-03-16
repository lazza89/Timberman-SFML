#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "TextureManager.h"
#include "Anim_Directional.h"
#include <unordered_map>

using Animations = std::unordered_map<std::string,Anim_Base*>;

class SpriteSheet{
public:
	SpriteSheet(TextureManager* textMgr);
	~SpriteSheet();

	void CropSprite(const sf::IntRect& rect);
	sf::Vector2i GetSpriteSize()const;
	sf::Vector2f GetSpritePosition()const;
	void SetSpriteSize(const sf::Vector2i& size);
	void SetSpritePosition(const sf::Vector2f& pos);

	void SetDirection(const Direction& dir);
	Direction GetDirection()const;

	bool LoadSheet(const std::string& file);
	void ReleaseSheet();

	Anim_Base* GetCurrentAnim();
	bool SetAnimation(const std::string& name, 
		const bool& play = false, 
		const bool& loop = false);

	void Update(const float& dT);
	void Draw(sf::RenderWindow* wnd);
private:
	std::string texture;
	sf::Sprite sprite;
	sf::Vector2i spriteSize;
	sf::Vector2f spriteScale;
	Direction direction;

	std::string animType;
	Animations animations;
	Anim_Base* animationCurrent;

	TextureManager* textureManager;
};