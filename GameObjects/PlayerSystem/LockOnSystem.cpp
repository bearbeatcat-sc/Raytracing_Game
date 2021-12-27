#include "LockOnSystem.h"

#include <Game_Object/ActorManager.h>

#include "Bullet.h"
#include "LockOnArea.h"
#include "LockOnUIObject.h"
#include "../TargetObjects/TargetObject.h"

LockOnSystem::LockOnSystem(Actor* pUser)
	:Component(pUser), _isPreviousLockOn(false),_isCurrentLockOn(false)
{
	_pLockOnArea = new LockOnArea(pUser);
	_pLockOnArea->SetScale(SimpleMath::Vector3(50, 30, 500));
	ActorManager::GetInstance().AddActor(_pLockOnArea);
	_pLockOnArea->SetActorName("PlayerLockOnArea");
}

LockOnSystem::~LockOnSystem()
{
	_pLockOnArea->Destroy();
}

void LockOnSystem::LockOn()
{
	auto canLockOnTargets = _pLockOnArea->GetLockOnTargets();

	for(int i = 0; i < canLockOnTargets.size(); ++i)
	{
		if (_targets.size() >= 20) break;

		if (canLockOnTargets[i]->GetDestroyFlag()) continue;;
		if (canLockOnTargets[i]->IsDelete()) continue;;

		AddTarget(canLockOnTargets[i]);
	}

	_isCurrentLockOn = true;
}

void LockOnSystem::Attack()
{
	for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		if ((*itr)._target->GetDestroyFlag()) continue;;
		if ((*itr)._target->IsDelete()) continue;;

		auto target = (*itr)._target;
		auto bullet = new Bullet(1.0f, target);

		auto targetVec = target->GetPosition() - _user->GetPosition();
		targetVec.Normalize();

		bullet->SetPosition(_user->GetPosition() + targetVec * 1.2f);
		bullet->SetScale(SimpleMath::Vector3(0.4f));
		bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
		ActorManager::GetInstance().AddActor(bullet);
		bullet->SetActorName("Bullet");
	}
}

void LockOnSystem::ClearTarget()
{
	for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		(*itr)._lockOnUI0->Destroy();
	}

	_targets.clear();
}

void LockOnSystem::Update()
{
	// 1フレーム、ロックオンをやめたとき
	if(_isCurrentLockOn == false && _isPreviousLockOn == true)
	{
		Attack();
		ClearTarget();
	}

	_isPreviousLockOn = _isCurrentLockOn;
	_isCurrentLockOn = false;
}

void LockOnSystem::DrawProperties()
{
}

void LockOnSystem::AddTarget(TargetObject* target)
{
	for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		if(target == (*itr)._target)
		{
			return;
		}
	}

	auto targetInfo = AttackTargetInfo
	{
		target,
		new LockOnUIObject(3.0f,target),
	};

	auto pos = targetInfo._target->GetPosition();

	targetInfo._lockOnUI0->SetPosition(pos);
	targetInfo._lockOnUI0->SetScale(SimpleMath::Vector3(1.0f));
	targetInfo._lockOnUI0->SetRotation(SimpleMath::Vector3(4.0f, 4.0f, 4.0f));
	ActorManager::GetInstance().AddActor(targetInfo._lockOnUI0);




	_targets.push_back(targetInfo);
}
