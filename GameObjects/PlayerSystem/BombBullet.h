#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class OBBCollisionComponent;
class DXRInstance;
class TargetObject;

class BombBullet
	:public Actor
{
public:
	BombBullet(const float moveSpeed, const SimpleMath::Vector3& vec);
	~BombBullet() = default;

private:
	void UpdateActor() override;
	void Explode();
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	float _addmoveSpeed;
	float _moveSpeed;
	float _moveTime;
	float _rotate;
	float _addRotate;


	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<DXRInstance> _instance;

	OBBCollisionComponent* _pCollisionComponent;
	SimpleMath::Vector3 _moveVec;
	std::shared_ptr<Timer> _traceEffectTimer;
};
