#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;
class Timer;
class CollisionComponent;
class RigidBodyComponent;


class Tracker
	:public Actor
{
public:
	Tracker(Actor* pPlayer);
	~Tracker() = default;
	void AddPoint(std::vector<SimpleMath::Vector3>& points);


private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;
	void Movement();

private:
	Actor* _pPlayer;
	std::shared_ptr<DXRInstance> _instance;

	SimpleMath::Vector3 _targetRotate;

	std::shared_ptr<Timer> _ChangeTargetRotateTimer;
	
	CollisionComponent* m_pCollisionComponent;
	std::vector<SimpleMath::Vector3> _movePoints;
	float _moveSpeed;
};
