#include "LockOnSystem.h"

#include <Game_Object/ActorManager.h>

#include "Bullet.h"
#include "LockOnArea.h"
#include "LockOnUIObject.h"

LockOnSystem::LockOnSystem(Actor* pUser)
	:Component(pUser), _isPreviousLockOn(false),_isCurrentLockOn(false)
{
	_pLockOnArea = new LockOnArea(pUser);
	_pLockOnArea->SetScale(SimpleMath::Vector3(30, 30, 300));
	ActorManager::GetInstance().AddActor(_pLockOnArea);
	_pLockOnArea->SetActorName("LockOnArea");
}

LockOnSystem::~LockOnSystem()
{
	_pLockOnArea->Destroy();
}

void LockOnSystem::LockOn()
{
	auto targets = _pLockOnArea->GetLockOnTargets();

	for(int i = 0; i < targets.size(); ++i)
	{
		AddTarget(targets[i]);
	}

	_isCurrentLockOn = true;
}

void LockOnSystem::Attack()
{
	for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		if ((*itr)._target->GetDestroyFlag()) continue;;

		auto target = (*itr)._target;
		auto bullet = new Bullet(1.0f, target);

		auto targetVec = target->GetPosition() - _user->GetPosition();
		targetVec.Normalize();

		bullet->SetPosition(_user->GetPosition() + targetVec * 1.2f);
		bullet->SetScale(SimpleMath::Vector3(0.5f));
		bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.5f, 2.7f));
		ActorManager::GetInstance().AddActor(bullet);
		bullet->SetActorName("Bullet");
	}
}

void LockOnSystem::ClearTarget()
{
	for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		(*itr)._lockOnUI0->Destroy();
		(*itr)._lockOnUI1->Destroy();
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

void LockOnSystem::AddTarget(Actor* target)
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
		new LockOnUIObject(3.0f),
		new LockOnUIObject(3.0f),
	};

	auto pos = targetInfo._target->GetPosition();

	targetInfo._lockOnUI0->SetPosition(pos);
	targetInfo._lockOnUI0->SetScale(SimpleMath::Vector3(3.0f));
	targetInfo._lockOnUI0->SetRotation(SimpleMath::Vector3(18.0f, 0.0f, 0.0f));

	targetInfo._lockOnUI1->SetPosition(pos);
	targetInfo._lockOnUI1->SetScale(SimpleMath::Vector3(3.0f));
	targetInfo._lockOnUI1->SetRotation(SimpleMath::Vector3(-18.0f, 0.0f, 0.0f));

	ActorManager::GetInstance().AddActor(targetInfo._lockOnUI0);
	ActorManager::GetInstance().AddActor(targetInfo._lockOnUI1);

	_targets.push_back(targetInfo);
}
