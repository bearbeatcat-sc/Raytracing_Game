#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class DXRInstance;

class ScoreResultObject
	:public Actor
{
public:
	ScoreResultObject();
	~ScoreResultObject();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;

	std::shared_ptr<AnimationComponent> _AnimationComponent;
};
