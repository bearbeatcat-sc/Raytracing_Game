#pragma once

#include "TargetObject.h"

class AnimationQue;
class CollisionComponent;
class Timer;
class AnimationComponent;

class TrackerCube
	:public TargetObject
{
public:
	TrackerCube(const std::string& dxrMeshName, GameManager* pGameManager);
	bool AnimationUpdate();
	~TrackerCube() = default;

private:
	void Move();
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	CollisionComponent* m_pCollisionComponent;
	SimpleMath::Vector3 _targetRotate;

	std::shared_ptr<Timer> _ChangeTargetRotateTimer;
	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<AnimationQue> _GenerateAnimationQue;
	std::shared_ptr<AnimationQue> _DestroyAnimationQue;

	bool _isDelete;
	float _moveSpeed;
};
