#include "Anim_Directional.h"
#include "SpriteSheet.h"

void Anim_Directional::CropSprite(){
	sf::IntRect rect(spriteSheet->GetSpriteSize().x * frameCurrent,
		spriteSheet->GetSpriteSize().y * (frameRow + (short)spriteSheet->GetDirection()),
		spriteSheet->GetSpriteSize().x, spriteSheet->GetSpriteSize().y);
	spriteSheet->CropSprite(rect);
}

void Anim_Directional::FrameStep(){
	if (frameStart < frameEnd){ ++frameCurrent; }
	else { --frameCurrent; }

	if ((frameStart < frameEnd && frameCurrent > frameEnd) ||
		(frameStart > frameEnd && frameCurrent < frameEnd))
	{
		if (loop){ frameCurrent = frameStart; return; }
		frameCurrent = frameEnd;
		Pause();
	}
}

void Anim_Directional::ReadIn(std::stringstream& stream){
	stream >> frameStart >> frameEnd >> frameRow
		>> frameTime >> frameActionStart >> frameActionEnd;
}