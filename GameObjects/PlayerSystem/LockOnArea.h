#pragma once

#include <Game_Object/Actor.h>

class Player;
class OBBCollisionComponent;

class LockOnArea
	:public Actor
{
public:
	LockOnArea(Actor* pActor);
	~LockOnArea() = default;
	const std::vector<Actor*>& GetLockOnTargets();

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	void AddTarget(Actor* target);
	virtual void OnCollsion(Actor* other) override;

private:
	Actor* _pActor;
	std::vector<Actor*> _lockOnTargets;

	OBBCollisionComponent* _obbCollisionComponent;
};
