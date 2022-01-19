#pragma once
#include <memory>

#include "TargetObject.h"

class GameManager;
class CollisionComponent;
class DXRInstance;
class AnimationComponent;

class NineSideCube
	:public TargetObject
{
public:
	NineSideCube(GameManager* pGameManager, float radius,float destroyTime);
	~NineSideCube() = default;

private:
	virtual void UpdateActor() override;
	void CreateBody();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Move();
	void Rotate(float angle);
	void ActiveAction() override;

	CollisionComponent* m_pCollisionComponent;

	std::shared_ptr<AnimationComponent> _AnimationComponent;

	float _cos;
	const float _moveSpeed;
	const float _bodyDestoryTime;
	const float _verticalRange;


	float _angle;
	float _radius;
	const float _rotateSpeed;
};
