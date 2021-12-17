#pragma once

#include <memory>
#include <vector>
#include <Components/Component.h>

class LockOnArea;

class LockOnSystem
	:public Component
{
public:
	LockOnSystem(Actor* pUser);
	~LockOnSystem();
	void LockOn();

	struct AttackTargetInfo
	{
		Actor* _target;

		Actor* _lockOnUI0;
		Actor* _lockOnUI1;
	};

private:
	void Attack();
	void ClearTarget();
	void Update() override;
	void DrawProperties() override;

	void AddTarget(Actor* target);

private:
	LockOnArea* _pLockOnArea;
	std::vector<AttackTargetInfo> _targets;

	bool _isPreviousLockOn;
	bool _isCurrentLockOn;
};
