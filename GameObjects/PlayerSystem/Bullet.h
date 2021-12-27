#pragma once

#include <Game_Object/Actor.h>

class OBBCollisionComponent;
class DXRInstance;

class Bullet
	:public Actor
{
public:
	Bullet(const float moveSpeed, Actor* targetActor);
	~Bullet() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	float _moveSpeed;
	float _point0X;
	float _point1X;

	float _moveTime;
	float _rotate;


	Actor* _targetActor;

	std::shared_ptr<DXRInstance> _instance;

	SimpleMath::Vector3 _createPoint;

	OBBCollisionComponent* _pCollisionComponent;

};
