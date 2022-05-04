#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class DXRInstance;

class ChangeResultUIObject
	:public Actor
{
public:
	ChangeResultUIObject() = default;
	~ChangeResultUIObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;
	//std::shared_ptr<AnimationComponent> _AnimationComponent;

};
