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
#include "BombInterFace.h"
#include "Player.h"
#include "LockOnArea.h"
#include "../TargetObjects/TargetObject.h"


LockOnSystem::LockOnSystem(Actor* pUser)
	:Component(pUser),  m_AimSpeed(3.0f), _generateBombBulletCount(0)
{



	_pBombInterFace = new BombInterFace();
	_user->SetChild(_pBombInterFace);
	_pBombInterFace->SetPosition(SimpleMath::Vector3(-0.69f, -0.43f, 1.14f));
	_pBombInterFace->SetScale(SimpleMath::Vector3(0.3f,0.02f,1.0f));
	_pBombInterFace->SetRotation(SimpleMath::Vector3(-0.87f, 0, 0));

	_generatedBombBullets.emplace(0, nullptr);
	_generatedBombBullets.emplace(1, nullptr);
	_generatedBombBullets.emplace(2, nullptr);
}

LockOnSystem::~LockOnSystem()
{

	for(auto bullet : _generatedBombBullets)
	{
		if(bullet.second != nullptr)
		{
			bullet.second->Destroy();
		}
	}

	_generatedBombBullets.clear();
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


bool LockOnSystem::IsGenerateBullet()
{
	for(auto bulletData : _generatedBombBullets)
	{
		if(bulletData.second == nullptr)
		{
			return true;
		}
	}

	return false;
}

void LockOnSystem::Attack()
{

	if ( !IsGenerateBullet())
	{
		return;
	}


	GenerateBullet();



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



void LockOnSystem::UpdateBombUIs()
{
	//auto playerPosition = _user->GetPosition();
	//auto basePosition = playerPosition + SimpleMath::Vector3(0.5f, 0, 2);

	//for(int i = 0; i < 3; ++i)
	//{
	//	auto bombUI = _bombUIObjects[i];
	//	auto position = basePosition + SimpleMath::Vector3(0.25f * i, 0, 0);

	//	bombUI->SetPosition(position);
	//}
}

void LockOnSystem::Update()
{
	Explosion();

	for(auto itr = _generatedBombBullets.begin(); itr != _generatedBombBullets.end();)
	{
		if((*itr).second != nullptr)
		{
			// ボムが消滅したときにUIを再生成する。
			if ((*itr).second->IsDelete())
			{
				(*itr).second->Destroy();
				(*itr).second = nullptr;

				auto index = (*itr).first;
				_pBombInterFace->Generate(index);

				continue;
			}
		}
		++itr;
	}


}

void LockOnSystem::Explosion()
{
	if(DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().IsDownTrigger(GamePad_LeftTrigger))
		{
			for (auto itr = _generatedBombBullets.begin(); itr != _generatedBombBullets.end(); ++itr)
			{
				if ((*itr).second == nullptr) continue;

				(*itr).second->Explode();
				(*itr).second->Destroy();
				(*itr).second = nullptr;

				auto index = (*itr).first;
				_pBombInterFace->Generate(index);

				return;
			}
		}
	}
	else
	{
		if (DirectXInput::GetInstance().IsKeyDown(DIK_Z))
		{
			for (auto itr = _generatedBombBullets.begin(); itr != _generatedBombBullets.end(); ++itr)
			{
				if ((*itr).second == nullptr) continue;

				(*itr).second->Explode();
				(*itr).second->Destroy();
				(*itr).second = nullptr;

				auto index = (*itr).first;
				_pBombInterFace->Generate(index);

				return;
			}
		}
	}

}



void LockOnSystem::DrawProperties()
{
}

void LockOnSystem::GenerateBullet()
{
	auto cursorPosition = static_cast<Player*>(_user)->GetCursorPosition();
	auto playerPosition = _user->GetPosition();
	auto vec = cursorPosition - playerPosition;
	vec += SimpleMath::Vector3(0, 2, 0);
	vec.Normalize();

	auto bullet = new BombBullet(1.0f, vec);

	bullet->SetScale(SimpleMath::Vector3(6.0f));
	//bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
	ActorManager::GetInstance().AddActor(bullet);
	bullet->SetActorName("Bullet");
	bullet->Destroy(5.0f);
	bullet->SetPosition(cursorPosition);

	auto rotate = MathUtility::LookAt(bullet->GetPosition(), _user->GetPosition());
	bullet->SetRotation(rotate);

	AddBullet(bullet);

	static_cast<Player*>(_user)->Shot();

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		DirectXInput::GetInstance().OnVibration(0, 30000, 0.0f, 0.1f);
	}
}

int LockOnSystem::FindIndex()
{
	for (auto bulletData : _generatedBombBullets)
	{
		if (bulletData.second == nullptr)
		{
			return bulletData.first;
		}
	}
}

void LockOnSystem::AddBullet(BombBullet* pBombBullet)
{
	auto index = FindIndex();
	_generatedBombBullets[index] = pBombBullet;
	_pBombInterFace->Throw(index);

}

