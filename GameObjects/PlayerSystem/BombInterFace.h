#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class AnimationQue;
class DXRInstance;

class BombUIObject;

class BombInterFace
	:public Actor
{
public:
	BombInterFace() = default;
	~BombInterFace() = default;
	void Generate(int index);
	void Throw(int index);

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::vector<BombUIObject*> _bombUIObjects;
};
