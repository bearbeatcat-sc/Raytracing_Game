#pragma once

#include <Game_Object/Actor.h>

class GameManager;

class TrackerShotObject
	:public Actor
{
public:
	TrackerShotObject(Actor* pTarget, GameManager* pGameManager,const float bulletSpeed);
	~TrackerShotObject() = default;

private:
	void Shot();
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	Actor* _pTarget;

	GameManager* _pGameManager;
	std::shared_ptr<Timer> _ShootCoolTimer;
	int _generateCount;
	int _maxBulletCount = 6;

	float _bulletSpeed;
};
