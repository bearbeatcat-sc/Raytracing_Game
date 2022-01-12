#pragma once

#include <Game_Object/Actor.h>

class AnimationComponent;
class OBBCollisionComponent;
class DXRInstance;
class TargetObject;

class Bullet
	:public Actor
{
public:
	Bullet(const float moveSpeed, TargetObject* targetActor);
	~Bullet() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	float _addmoveSpeed;
	float _moveSpeed;
	float _point0X;
	float _point1X;

	float _moveTime;
	float _rotate;
	float _addRotate;

	std::shared_ptr<AnimationComponent> _AnimationComponent;

	TargetObject* _targetActor;

	std::shared_ptr<DXRInstance> _instance;

	SimpleMath::Vector3 _createPoint;

	OBBCollisionComponent* _pCollisionComponent;
	bool _isTargetLost;

	SimpleMath::Vector3 _moveVec;

};
