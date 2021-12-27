#pragma once

#include <Game_Object/Actor.h>

class TargetObject;
class Player;
class OBBCollisionComponent;

class LockOnArea
	:public Actor
{
public:
	LockOnArea(Actor* pActor);
	~LockOnArea() = default;
	const std::vector<TargetObject*>& GetLockOnTargets();

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	void AddTarget(TargetObject* target);
	virtual void OnCollsion(Actor* other) override;

private:
	Actor* _pActor;
	std::vector<TargetObject*> _lockOnTargets;

	OBBCollisionComponent* _obbCollisionComponent;
};
