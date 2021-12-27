#pragma once


#include "TargetObject.h"

class AnimationQue;
class CollisionComponent;
class Timer;
class AnimationComponent;

class TargetCube
	:public TargetObject
{
public:
	TargetCube(const std::string& dxrMeshName, GameManager* pGameManager);
	~TargetCube() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	CollisionComponent* m_pCollisionComponent;
	SimpleMath::Vector3 _targetRotate;

	std::shared_ptr<Timer> _ChangeTargetRotateTimer;
	std::shared_ptr<AnimationComponent> _AnimationComponent;


};
