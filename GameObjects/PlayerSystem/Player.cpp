#include "Player.h"

#include <Utility/Time.h>
#include <Utility/Random.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>
#include <imgui/imgui.h>
#include <Utility/Camera.h>
#include <Components/SpriteComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include "AimCursor.h"
#include "Bullet.h"
#include "LockOnSystem.h"
#include "PlayerCamera.h"
#include "../GameSystem/GameManager.h"

Player::Player(const SimpleMath::Vector3& pos, GameManager* pGameManager)
	:Actor(), _pGameManager(pGameManager), _isGenerateLeft(false),_playerState(PlayerState_Stay), _moveSpeed(8.0f)
{
	SetPosition(pos);
	SetTag("Player");
}


void Player::Shot()
{
	//auto bullet = new Bullet(1.0f, pTarget);

	//bullet->SetScale(SimpleMath::Vector3(0.4f));
	//bullet->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 2.4f));
	//ActorManager::GetInstance().AddActor(bullet);
	//bullet->SetActorName("Bullet");
	//bullet->Destroy(10.0f);

	//_isGenerateLeft = !_isGenerateLeft;

	//if (_isGenerateLeft)
	//{
	//	bullet->SetPosition(GetPosition() + SimpleMath::Vector3(1, -1.0f, 0) * 2.0f);

	//	return;
	//}

	//bullet->SetPosition(GetPosition() + SimpleMath::Vector3(-1, -1.0f, 0) * 2.0f);

	_pPlayerCamera->Shake(0.1f, 0.1f);


}

void Player::SetPlayerState(PlayerState playerState)
{
	_playerState = playerState;
}

void Player::Move()
{
	auto position = GetPosition();
	position += Time::DeltaTime * _moveSpeed * SimpleMath::Vector3::Backward;

	SetPosition(position);
}

const SimpleMath::Vector3& Player::GetCursorPosition()
{
	return _pAimCursor->GetCursorPosition();
}

void Player::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	//_instance->SetMatrix(mtx);
	MoveCusor();

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().IsTrigger(GamePad_RightTrigger))
		{
			LockOn();
		}
	}
	else
	{
		if (DirectXInput::GetInstance().IsKey(DIK_SPACE))
		{
			LockOn();
		}
	}

	auto cameraPitch = _pPlayerCamera->GetPitch();
	auto cameraYaw = _pPlayerCamera->GetYaw();
	SetRotation(SimpleMath::Vector3(cameraYaw, cameraPitch, 0));

	switch(_playerState)
	{
	case PlayerState_Stay:
		break;

	case PlayerState_Move:
		Move();
		break;
	}

	
}

void Player::LockOn()
{
	_lockOnSystem->LockOn();

}


void Player::MoveCusor()
{
	_pAimCursor->MoveCusor();
}

void Player::Init()
{



	//_instance = DXRPipeLine::GetInstance().AddInstance("BlueCube", 0);
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale * 1.4f, "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	//auto mtx = GetWorldMatrix();
	//_instance->SetMatrix(mtx);
	//_instance->CreateRaytracingInstanceDesc();

	_pAimCursor = new AimCursor();
	SetChild(_pAimCursor);

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

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		DirectXInput::GetInstance().OnVibration(0, 60000, 60000, 1.0f);
	}
}

void Player::OnCollsion(Actor* other)
{
	if(other->IsContainsTag("Target"))
	{
		Damage();
	}
}

