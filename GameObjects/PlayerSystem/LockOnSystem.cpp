#include "LockOnSystem.h"

#include <algorithm>
#include <Device/WindowApp.h>
#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>
#include <Utility/CameraManager.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>
#include <Utility/Camera.h>

#include "Bullet.h"
#include "BombBullet.h"
#include "Player.h"
#include "LockOnArea.h"
#include "../TargetObjects/TargetObject.h"


LockOnSystem::LockOnSystem(Actor* pUser)
	:Component(pUser), _isPreviousLockOn(false), _isCurrentLockOn(false), m_AimSpeed(3.0f)
{
	_pLockOnArea = new LockOnArea(pUser);
	_pLockOnArea->SetScale(SimpleMath::Vector3(100, 200, 500));
	ActorManager::GetInstance().AddActor(_pLockOnArea);
	_pLockOnArea->SetActorName("PlayerLockOnArea");

	_lockonCoolTimer = std::make_shared<Timer>(_lockonCoolTime);
	_shotCoolTimer = std::make_shared<Timer>(_shotCollTime);
}

LockOnSystem::~LockOnSystem()
{
	_pLockOnArea->Destroy();
}

void LockOnSystem::LockOn()
{
	//auto canLockOnTargets = _pLockOnArea->GetLockOnTargets();
	//_lockonCoolTimer->Update();

	//for(int i = 0; i < canLockOnTargets.size(); ++i)
	//{
	//	if (canLockOnTargets[i]._pTarget->GetDestroyFlag()) continue;;
	//	if (canLockOnTargets[i]._pTarget->IsDelete()) continue;;

	//	AddTarget(canLockOnTargets[i]);
	//}

	//_isCurrentLockOn = true;

	Attack();
}


void LockOnSystem::Attack()
{
	_shotCoolTimer->Update();

	if (!_shotCoolTimer->IsTime())
	{
		return;
	}

	_shotCoolTimer->Reset();

	auto cursorPosition = static_cast<Player*>(_user)->GetCursorPosition();
	auto playerPosition = _user->GetPosition();
	auto vec = cursorPosition - playerPosition;
	vec.Normalize();

	auto bullet = new BombBullet(1.0f, vec);



	bullet->SetScale(SimpleMath::Vector3(0.4f));
	//bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
	ActorManager::GetInstance().AddActor(bullet);
	bullet->SetActorName("Bullet");
	bullet->Destroy(10.0f);
	bullet->SetPosition(_user->GetPosition() + vec * 4.0f);

	auto rotate = MathUtility::LookAt(bullet->GetPosition(), _user->GetPosition());
	bullet->SetRotation(rotate);

	static_cast<Player*>(_user)->Shot();

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		DirectXInput::GetInstance().OnVibration(0, 30000, 0.0f, 0.1f);
	}
	return;


	//for(auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	//{
	//	if ((*itr)._target->GetDestroyFlag()) continue;;
	//	if ((*itr)._target->IsDelete()) continue;;

	//	const int targetCount = (*itr)._lockOnCount;
	//	auto target = (*itr)._target;

	//	//for(int i = 0; i < targetCount; ++i)
	//	//{
	//	//	auto bullet = new Bullet(1.0f, target);

	//	//	auto targetVec = target->GetPosition() - _user->GetPosition();
	//	//	targetVec.Normalize();

	//	//	bullet->SetPosition(_user->GetPosition() + targetVec * 1.2f);
	//	//	bullet->SetScale(SimpleMath::Vector3(0.4f));
	//	//	bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
	//	//	ActorManager::GetInstance().AddActor(bullet);
	//	//	bullet->SetActorName("Bullet");
	//	//}


	//	//auto targetVec = target->GetPosition() - _user->GetPosition();
	//	//targetVec.Normalize();

	//	auto mainCamera = CameraManager::GetInstance().GetMainCamera();
	//	SimpleMath::Vector3 toWorldPos = MathUtility::ScreenToWorld(_aimPosition.x, _aimPosition.y, 0, 1920, 1080, mainCamera->GetFar(), mainCamera->GetNear(), mainCamera->GetViewMat(), mainCamera->GetProjectMat());

	//	auto vec = (toWorldPos - _user->GetPosition());
	//	vec.Normalize();

	//	auto bullet = new Bullet(2.0f, target);

	//	bullet->SetScale(SimpleMath::Vector3(0.4f));
	//	bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
	//	ActorManager::GetInstance().AddActor(bullet);
	//	bullet->SetActorName("Bullet");
	//	bullet->Destroy(10.0f);

	//	_isGenerateLeft = !_isGenerateLeft;

	//	if (_isGenerateLeft)
	//	{
	//		bullet->SetPosition(_user->GetPosition() + SimpleMath::Vector3(1, -1.0f, 0) * 2.0f);
	//		if (DirectXInput::GetInstance().IsActiveGamePad())
	//		{
	//			DirectXInput::GetInstance().OnVibration(0, 30000, 0.0f, 0.1f);
	//		}
	//		return;
	//	}

	//	bullet->SetPosition(_user->GetPosition() + SimpleMath::Vector3(-1, -1.0f, 0) * 2.0f);

	//	static_cast<Player*>(_user)->Shot(target, vec);

	//	if (DirectXInput::GetInstance().IsActiveGamePad())
	//	{
	//		DirectXInput::GetInstance().OnVibration(0, 0.0f, 30000, 0.1f);
	//	}
	//	return;
	//}
}

void LockOnSystem::ClearTarget()
{

	_targets.clear();
}

void LockOnSystem::Update()
{
	// 1フレーム、ロックオンをやめたとき
	//if(_isCurrentLockOn == false && _isPreviousLockOn == true)
	//{
	//	Attack();
	//	ClearTarget();
	//	_lockonCoolTimer->Reset();
	//}


	ClearTarget();

	_isPreviousLockOn = _isCurrentLockOn;
	_isCurrentLockOn = false;
}



void LockOnSystem::DrawProperties()
{
}

void LockOnSystem::AddTarget(const LockOnArea::LockOnInfo& target)
{
	for (auto itr = _targets.begin(); itr != _targets.end(); ++itr)
	{
		if (target._pTarget == (*itr)._target)
		{
			return;
		}
	}

	//if(!_lockonCoolTimer->IsTime())
	//{
	//	return;
	//}

	//_lockonCoolTimer->Reset();


	auto targetInfo = AttackTargetInfo
	{
		target._pTarget,
		target._lockOnCount,
	};








	_targets.push_back(targetInfo);
}
