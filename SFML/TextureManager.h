#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>

class TextureManager: public ResourceManager<TextureManager, sf::Texture>{
public:
	TextureManager(): ResourceManager("Resources/textures.cfg"){}

	sf::Texture* Load(const std::string& path){
		sf::Texture* texture = new sf::Texture();
		if(!texture->loadFromFile(path))
		{
			delete texture;
			texture = nullptr;
			std::cerr << "! Failed to load texture: " << path << std::endl;
		}
		return texture;
	}
};