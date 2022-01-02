#include "Player.h"

#include <Utility/Time.h>
#include <Utility/Random.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>
#include <imgui/imgui.h>
#include <Utility/Camera.h>
#include <Utility/Math/MathUtility.h>

#include "LockOnSystem.h"
#include "MirrorBullet.h"
#include "Components/Collsions/CollisionManager.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "PlayerCamera.h"
#include "../GameSystem/GameManager.h"

Player::Player(const SimpleMath::Vector3& pos, GameManager* pGameManager)
	:Actor(), _moveSpeed(0.01f),_pGameManager(pGameManager)
{
	SetPosition(pos);
	SetTag("Player");
}



void Player::ShotMirror()
{
	auto forward = _pPlayerCamera->GetForward();
	auto playerPos = GetPosition();

	auto qu = MathUtility::LookAt(playerPos * forward * 3.0f, playerPos);

	auto mirrorbullet = new MirrorBullet(128.0f * _moveSpeed, 1.0f, forward, SimpleMath::Vector3(16.0f,8.0f,1.0f));
	mirrorbullet->SetPosition(playerPos + forward * 3.0f);
	mirrorbullet->SetScale(SimpleMath::Vector3(0.4f));
	ActorManager::GetInstance().AddActor(mirrorbullet);
}

void Player::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	//_instance->SetMatrix(mtx);

	Movement();
	SpeedUp();

	if (DirectXInput::GetInstance().IsKey(DIK_SPACE))
	{
		LockOn();
	}

	if(DirectXInput::GetInstance().IsKeyDown(DIK_Z))
	{
		ShotMirror();
	}

	auto cameraPitch = _pPlayerCamera->GetPitch();
	SetRotation(SimpleMath::Vector3(cameraPitch, 0, 0));
}

void Player::LockOn()
{
	_lockOnSystem->LockOn();

}

void Player::Init()
{



	//_instance = DXRPipeLine::GetInstance().AddInstance("BlueCube", 0);
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);


	//auto mtx = GetWorldMatrix();
	//_instance->SetMatrix(mtx);
	//_instance->CreateRaytracingInstanceDesc();



	_pPlayerCamera = new PlayerCamera(this);
	SetChild(_pPlayerCamera);

	_lockOnSystem = std::make_shared<LockOnSystem>(this);
	AddComponent(_lockOnSystem);
}

void Player::Shutdown()
{
	_pCollisionComponent->Delete();
	//_instance->Destroy();
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
	//auto trackerPos = _pTracker->GetPosition();
	//auto pos = trackerPos;

	//SetPosition(SimpleMath::Vector3::Lerp(GetPosition(), pos, Time::DeltaTime * _moveSpeed));

	_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 4.0f, 1.0f, _maxMoveSpeed);
	_moveSpeed = _maxMoveSpeed;

	auto setPos = GetPosition() + (SimpleMath::Vector3::Backward * _moveSpeed * Time::DeltaTime);
	SetPosition(setPos);
}

