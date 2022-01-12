#include "MirrorCube.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "../GameSystem/GameManager.h"
#include "../PlayerSystem/Player.h"

MirrorCube::MirrorCube(GameManager* pGameManager,bool controllFlag,float angle)
	:_pGameManager(pGameManager),_radius(6.0f), _angle(angle), _rotateSpeed(3.0f),_controllFlag(controllFlag)
{
	_instance = DXRPipeLine::GetInstance().AddInstance("WhiteCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void MirrorCube::UpdateActor()
{
	if(_controllFlag)
	{
		if (DirectXInput::GetInstance().IsKey(DIK_LEFTARROW))
		{
			Rotate(-1.0f);
		}

		if (DirectXInput::GetInstance().IsKey(DIK_RIGHTARROW))
		{
			Rotate(1.0f);
		}
	}
	else
	{
		if (DirectXInput::GetInstance().IsKey(DIK_A))
		{
			Rotate(-1.0f);
		}

		if (DirectXInput::GetInstance().IsKey(DIK_D))
		{
			Rotate(1.0f);
		}
	}


	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);

}

void MirrorCube::Init()
{
	SetTag("Mirror");
	SetActorName("MirrorCube");

	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "AllHitObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	auto pPlayer = _pGameManager->GetPlayer();

	if (pPlayer == nullptr)
	{
		throw std::runtime_error("Unable to retrieve the player.");
	}

	Rotate(0.0f);
}

void MirrorCube::Shutdown()
{
	m_pCollisionComponent->Delete();
	_instance->Destroy();
}

void MirrorCube::OnCollsion(Actor* other)
{

}

void MirrorCube::Rotate(float angle)
{
	auto pPlayer = _pGameManager->GetPlayer();

	if (pPlayer == nullptr)return;



	_angle += Time::DeltaTime * _rotateSpeed * angle;

	auto playerPosition = pPlayer->GetPosition();

	SimpleMath::Vector3 pos = playerPosition + SimpleMath::Vector3(_radius * cos(_angle), 0, _radius * sin(_angle));

	SetPosition(pos);


	auto rotate = MathUtility::LookAt(GetPosition(), playerPosition);
	SetRotation(rotate);

}
