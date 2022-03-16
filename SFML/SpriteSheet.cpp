#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(TextureManager* textMgr) 
	:textureManager(textMgr), animationCurrent(nullptr), 
	spriteScale(1.f, 1.f), direction(Direction::Right){}

SpriteSheet::~SpriteSheet(){ ReleaseSheet(); }

void SpriteSheet::ReleaseSheet(){
	textureManager->ReleaseResource(texture);
	animationCurrent = nullptr;
	while(animations.begin() != animations.end()){
		delete animations.begin()->second;
		animations.erase(animations.begin());
	}
}

sf::Vector2i SpriteSheet::GetSpriteSize()const{ return spriteSize; }
sf::Vector2f SpriteSheet::GetSpritePosition()const{ return sprite.getPosition(); }
Direction SpriteSheet::GetDirection()const{ return direction; }
Anim_Base* SpriteSheet::GetCurrentAnim(){ return animationCurrent; }

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size){
	spriteSize = size;
	sprite.setOrigin(spriteSize.x / 2, spriteSize.y);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f& pos){ sprite.setPosition(pos); }

void SpriteSheet::SetDirection(const Direction& dir){
	if (dir == direction){ return; }
	direction = dir;
	animationCurrent->CropSprite();
}

void SpriteSheet::CropSprite(const sf::IntRect& rect){ sprite.setTextureRect(rect); }

bool SpriteSheet::LoadSheet(const std::string& file){
	std::ifstream sheet;
	sheet.open(file);
	if(sheet.is_open()){
		ReleaseSheet(); // Release current sheet resources.
		std::string line;
		while(std::getline(sheet,line)){
			if (line[0] == '|'){ continue; }
			std::stringstream keystream(line);
			std::string type;
			keystream >> type;
			if(type == "Texture"){
				if (texture != ""){
					std::cerr << "! Duplicate texture entries in: " << file << std::endl;
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!textureManager->RequireResource(texture)){
					std::cerr << "! Could not set up the texture: " << texture << std::endl;
					continue;
				}
				texture = texture;
				sprite.setTexture(*textureManager->GetResource(texture));
			} else if(type == "Size"){
				keystream >> spriteSize.x >> spriteSize.y;
				SetSpriteSize(spriteSize);
			} else if(type == "Scale"){
				keystream >> spriteScale.x >> spriteScale.y;
				sprite.setScale(spriteScale);
			} else if(type == "AnimationType"){
				keystream >> animType;
			} else if(type == "Animation"){
				std::string name;
				keystream >> name;
				if (animations.find(name) != animations.end()){
					std::cerr << "! Duplicate animation(" << name << ") in: " << file << std::endl;
					continue;
				}
				Anim_Base* anim	= nullptr;
				if(animType == "Directional"){
					anim = new Anim_Directional();
				} else {
					std::cerr << "! Unknown animation type: " << animType << std::endl;
					continue;
				}

				keystream >> *anim;
				anim->SetSpriteSheet(this);
				anim->SetName(name);
				anim->Reset();
				animations.emplace(name,anim);

				if (animationCurrent){ continue; }
				animationCurrent = anim;
				animationCurrent->Play();
			}
		}
		sheet.close();
		return true;
	}
	std::cerr << "! Failed loading spritesheet: " << file << std::endl;
	return false;
}

bool SpriteSheet::SetAnimation(const std::string& name, 
	const bool& play, const bool& loop)
{
	auto itr = animations.find(name);
	if (itr == animations.end()){ return false; }
	if (itr->second == animationCurrent){ return false; }
	if (animationCurrent){ animationCurrent->Stop(); }
	animationCurrent = itr->second;
	animationCurrent->SetLooping(loop);
	if(play){ animationCurrent->Play(); }
	animationCurrent->CropSprite();
	return true;
}

void SpriteSheet::Update(const float& dT){ animationCurrent->Update(dT); }
void SpriteSheet::Draw(sf::RenderWindow* wnd){ wnd->draw(sprite); }