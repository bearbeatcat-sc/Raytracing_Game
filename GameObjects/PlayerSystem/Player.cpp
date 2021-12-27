#include "Player.h"

#include <Utility/CameraManager.h>
#include <Utility/Time.h>
#include <Utility/Random.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <imgui/imgui.h>
#include <Utility/Camera.h>

#include "LockOnSystem.h"
#include "Components/Collsions/CollisionManager.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "../PointLightObject.h"
#include "PlayerCamera.h"
#include "../GameSystem/GameManager.h"

Player::Player(const SimpleMath::Vector3& pos, GameManager* pGameManager)
	:Actor(), _moveSpeed(0.01f),_pGameManager(pGameManager)
{
	SetPosition(pos);
	SetTag("Player");
}

void Player::SetTracker(Actor* pTracker)
{
	_pTracker = pTracker;
}

void Player::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	Movement();
	SpeedUp();

	if (DirectXInput::GetInstance().IsKey(DIK_SPACE))
	{
		_lockOnSystem->LockOn();
	}

}

void Player::Init()
{



	_instance = DXRPipeLine::GetInstance().AddInstance("GrayCube", 0);
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);


	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_lockOnSystem = std::make_shared<LockOnSystem>(this);
	AddComponent(_lockOnSystem);

	_pPlayerCamera = new PlayerCamera(_pTracker);
	SetChild(_pPlayerCamera);
}

void Player::Shutdown()
{
	_pCollisionComponent->Delete();
	_instance->Destroy();
}

void Player::Damage()
{
	_pPlayerCamera->Shake(0.4f, 0.1f);
	_pGameManager->AddScore(-100);
}

void Player::OnCollsion(Actor* other)
{
	if(other->IsContainsTag("Target"))
	{
		Damage();
	}
}



void Player::SpeedUp()
{
	_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 0.1f, 0.01f, 1.0f);

}

void Player::Movement()
{
	auto trackerPos = _pTracker->GetPosition();
	auto pos = trackerPos;

	SetPosition(SimpleMath::Vector3::Lerp(GetPosition(), pos, Time::DeltaTime * _moveSpeed));
}

