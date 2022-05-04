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
	bool IsDelete();
	void GenerateTraceEffect();
	void GenerateRedTraceEffect();
	void Explode();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void Exploding();
	void OnCollsion(Actor* other) override;

private:
	float _addmoveSpeed;
	float _moveSpeed;
	float _moveTime;
	float _rotate;
	float _addRotate;

	bool _isDeleteFlag;
	bool _isExploding;

	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<DXRInstance> _instance;

	std::shared_ptr<Timer> _explodeTimer;
	std::shared_ptr<Timer> _explodingTimer;

	const float _explodeTime = 3.0f;

	OBBCollisionComponent* _pCollisionComponent;
	SimpleMath::Vector3 _moveVec;
	std::shared_ptr<Timer> _traceEffectTimer;
};
