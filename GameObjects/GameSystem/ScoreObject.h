#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class AnimationQue;
class DXRInstance;

class ScoreObject
	:public Actor
{
public:
	ScoreObject(const int number);
	~ScoreObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	 std::string _meshName;
	 std::shared_ptr<DXRInstance> _instance;

	 std::shared_ptr<AnimationComponent> _AnimationComponent;
	 std::shared_ptr<AnimationQue> _GenerateAnimationQue;
};
