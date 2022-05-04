#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class AnimationQue;
class DXRInstance;

class BombUIObject
	:public Actor
{
public:
	BombUIObject() = default;
	~BombUIObject() = default;
	void Generate();
	void Throw();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _bombUIInstance;
	std::shared_ptr<DXRInstance> _bombThrowUIInstance;
	std::shared_ptr<AnimationComponent> _AnimationComponent;

	bool _isThrow;
};
