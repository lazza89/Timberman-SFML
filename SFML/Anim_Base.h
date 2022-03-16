#pragma once
#include <string>
class SpriteSheet;

using Frame = unsigned int;

class Anim_Base{
	friend class SpriteSheet;
public:
	Anim_Base();
	virtual ~Anim_Base();

	void SetSpriteSheet(SpriteSheet* sheet);
	void SetFrame(Frame frame);
	void SetStartFrame(Frame frame);
	void SetEndFrame(Frame frame);
	void SetFrameRow(Frame row);
	void SetActionStart(Frame frame);
	void SetActionEnd(Frame frame);
	void SetFrameTime(float time);
	void SetLooping(bool loop);
	void SetName(const std::string& name);

	SpriteSheet* GetSpriteSheet();
	Frame GetFrame();
	Frame GetStartFrame();
	Frame GetEndFrame();
	Frame GetFrameRow();
	int GetActionStart();
	int GetActionEnd();
	float GetFrameTime();
	float GetElapsedTime();
	bool IsLooping();
	bool IsPlaying();
	bool IsInAction();
	std::string GetName();

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const float& dT);

	friend std::stringstream& operator >>(
		std::stringstream& stream, Anim_Base& a)
	{
		a.ReadIn(stream);
		return stream;
	}
protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

	Frame frameCurrent;
	Frame frameStart;
	Frame frameEnd;
	Frame frameRow;
	int frameActionStart;
	int frameActionEnd;
	float frameTime;
	float elapsedTime;
	bool loop;
	bool playing;

	std::string name;

	SpriteSheet* spriteSheet;
};