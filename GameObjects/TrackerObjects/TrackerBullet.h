#pragma once

#include <Components/Collsions/SphereCollisionComponent.h>
#include <Device/Raytracing/DXRInstance.h>

#include "../TargetObjects/TargetObject.h"

class OBBCollisionComponent;
class AnimationQue;
class Timer;
class AnimationComponent;

class TrackerBullet
	:public TargetObject
{
public:
	TrackerBullet(const float moveSpeed, Actor* targetActor, GameManager* pGameManager,float point0x,float point1x);
	~TrackerBullet() = default;

private:
	void UpdateActor() override;
	void Init() override; 
	void Shutdown() override;
	void OnCollsion(Actor * other) override;

private:
	float _moveSpeed;
	float _point0X;
	float _point1X;
	float _moveTime;

	Actor* _targetActor;
	GameManager* _pGameManager;
	bool _isDelete;

	std::shared_ptr<AnimationComponent> _AnimationComponent;

	SimpleMath::Vector3 _createPoint;

	OBBCollisionComponent* _pCollisionComponent;

	float _rotate;
};
