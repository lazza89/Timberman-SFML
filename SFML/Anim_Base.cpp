#include "Anim_Base.h"
#include "SpriteSheet.h"

Anim_Base::Anim_Base(): frameCurrent(0), frameStart(0), frameEnd(0),
	frameRow(0), frameTime(0.f), elapsedTime(0.f), frameActionStart(-1),
	frameActionEnd(-1), loop(false), playing(false){}

Anim_Base::~Anim_Base(){}

void Anim_Base::SetFrame(Frame frame){
	if((frame >= frameStart && frame <= frameEnd) ||
		(frame >= frameEnd && frame <= frameStart))
	{
		frameCurrent = frame;
	}
}

bool Anim_Base::IsInAction(){
	if (frameActionStart == -1 || frameActionEnd == -1){ return true; }
	return (frameCurrent >= frameActionStart && frameCurrent <= frameActionEnd);
}

void Anim_Base::Reset(){
	frameCurrent = frameStart;
	elapsedTime = 0.0f;
	CropSprite();
}

void Anim_Base::Update(const float& dT){
	if (!playing){ return; }
	elapsedTime += dT;
	if (elapsedTime < frameTime){ return; }
	FrameStep();
	CropSprite();
	elapsedTime = 0;
}

void Anim_Base::SetSpriteSheet(SpriteSheet* sheet){ spriteSheet = sheet; }
void Anim_Base::SetStartFrame(Frame frame){ frameStart = frame; }
void Anim_Base::SetEndFrame(Frame frame){ frameEnd = frame; }
void Anim_Base::SetFrameRow(Frame row){ frameRow = row; }
void Anim_Base::SetActionStart(Frame frame){ frameActionStart = frame; }
void Anim_Base::SetActionEnd(Frame frame){ frameActionEnd = frame; }
void Anim_Base::SetFrameTime(float time){ frameTime = time; }
void Anim_Base::SetLooping(bool loop){ loop = loop; }
void Anim_Base::SetName(const std::string& tmpname){ name = tmpname; }

SpriteSheet* Anim_Base::GetSpriteSheet(){ return spriteSheet; }
Frame Anim_Base::GetFrame(){ return frameCurrent; }
Frame Anim_Base::GetStartFrame(){ return frameStart; }
Frame Anim_Base::GetEndFrame(){ return frameEnd; }
Frame Anim_Base::GetFrameRow(){ return frameRow; }
int Anim_Base::GetActionStart(){ return frameActionStart; }
int Anim_Base::GetActionEnd(){ return frameActionEnd; }
float Anim_Base::GetFrameTime(){ return frameTime; }
float Anim_Base::GetElapsedTime(){ return elapsedTime; }
std::string Anim_Base::GetName(){ return name; }
bool Anim_Base::IsLooping(){ return loop; }
bool Anim_Base::IsPlaying(){ return playing; }

void Anim_Base::Play(){ playing = true; }
void Anim_Base::Pause(){ playing = false; }
void Anim_Base::Stop(){ playing = false; Reset(); }