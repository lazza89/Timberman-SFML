#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

enum class GUI_ElementState{
	Neutral,
	Focused,
	Clicked
};

struct GUI_Style{
	GUI_Style(): 
		textSize(12),
		textCenterOrigin(false),
		backgroundImageColor(255,255,255,255)
	{
		sf::Color none = sf::Color(0, 0, 0, 0);
		backgroundColor	= none;
		elementColor = none;
		textColor = none;
	}

	sf::Vector2f size; // Element size.
	// Background properties.
	sf::Color backgroundColor;
	sf::Color elementColor;
	std::string backgroundImage;
	sf::Color backgroundImageColor;
	// Text properties.
	sf::Color textColor;
	std::string textFont;
	sf::Vector2f textPadding;
	unsigned int textSize;
	bool textCenterOrigin;
	// Glyph properties.
	std::string glyph;
	sf::Vector2f glyphPadding;
};

struct GUI_Visual{
	sf::RectangleShape backgroundSolid;
	sf::Sprite backgroundImage;
	sf::Sprite glyph;
	sf::Text text;
};