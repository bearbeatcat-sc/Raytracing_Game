#pragma once

#include <memory>
#include <vector>
#include <Components/Component.h>

#include "LockOnArea.h"

class SpriteComponent;
class Timer;
class TargetObject;
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
		TargetObject* _target = nullptr;
		int _lockOnCount = 0;

		Actor* _lockOnUI0 = nullptr;
		Actor* _lockOnUI1 = nullptr;
	};

private:
	void Attack();
	void ClearTarget();
	void Update() override;
	void DrawProperties() override;

	void AddTarget(const LockOnArea::LockOnInfo& target);

private:
	LockOnArea* _pLockOnArea;
	std::vector<AttackTargetInfo> _targets;
	std::shared_ptr<Timer> _lockonCoolTimer;
	std::shared_ptr<Timer> _shotCoolTimer;

	const float _lockonCoolTime = 0.002f;
	const float _shotCollTime = 0.1f;

	bool _isPreviousLockOn;
	bool _isCurrentLockOn;

	bool _isGenerateLeft;
	const float m_AimSpeed;
};
