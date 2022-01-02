#pragma once

class AnimationComponent;
class DXRInstance;

#include <Game_Object/Actor.h>

class MirrorBullet
	:public Actor
{
public:
	MirrorBullet(float moveSpeed, float moveTime, const SimpleMath::Vector3& vec, const SimpleMath::Vector3& maxScale);
	~MirrorBullet() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void Expand();


private:
	SimpleMath::Vector3 _moveVec;
	SimpleMath::Vector3 _maxScale;

	float _moveSpeed;

	std::shared_ptr<DXRInstance> _instance;
	std::shared_ptr<AnimationComponent> _AnimationComponent;

	std::shared_ptr<Timer> _moveTimer;
};
