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
	NineSideCube(GameManager* pGameManager, float radius);
	~NineSideCube() = default;

private:
	virtual void UpdateActor() override;
	void CreateBody();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Move();
	void Rotate(float angle);

private:

	CollisionComponent* m_pCollisionComponent;

	std::shared_ptr<AnimationComponent> _AnimationComponent;

	float _cos;
	float _moveSpeed;
	const float _verticalRange;


	float _angle;
	float _radius;
	const float _rotateSpeed;
};
