#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font> {
public:
	FontManager() : ResourceManager("Resources/fonts.cfg") {}
	sf::Font* Load(const std::string& path) {
		sf::Font* font = new sf::Font();
		if (!font->loadFromFile(path))
		{
			delete font;
			font = nullptr;
			std::cerr << "! Failed to load font: " << path << std::endl;
		}
		return font;
	}
};