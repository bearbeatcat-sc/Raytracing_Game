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

	struct LockOnInfo
	{
		LockOnInfo(TargetObject* pTarget,int lockOnCount)
			:_pTarget(pTarget),_lockOnCount(lockOnCount)
		{
			
		}

		TargetObject* _pTarget = nullptr;
		int _lockOnCount = 0;
	};

	const std::vector<LockOnInfo>& GetLockOnTargets();

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	void AddTarget(LockOnInfo target);
	virtual void OnCollsion(Actor* other) override;

private:
	Actor* _pActor;
	std::vector<LockOnInfo> _lockOnTargets;

	OBBCollisionComponent* _obbCollisionComponent;
};
