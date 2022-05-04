﻿#include "NineSideCube.h"

#include <algorithm>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "NineSideCubeBody.h"
#include "TargetCube.h"
#include "../GameSystem/GameManager.h"
#include "../PlayerSystem/Player.h"

NineSideCube::NineSideCube(GameManager* pGameManager,float radius, float destroyTime)
	:TargetObject(pGameManager,destroyTime), _cos(0.0f), _moveSpeed(10.0f), _verticalRange(30.0f),
	_angle(0.0f), _radius(radius), _rotateSpeed(1.0f), _bodyDestoryTime(destroyTime)
{
	SetTag("NineSideCube");
}

void NineSideCube::UpdateActor()
{
	//Move();
	SetRotation(m_EulerRotation);
	Rotate(1.0f);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);

	//_radius = std::clamp(_radius - Time::DeltaTime * 1.4f, 0.0f, 100.0f);

	

	if(_AnimationComponent->GetCurrentState() == "End")
	{
		DestoryOrder();
		return;
	}

	if(GetChildren().size() == 0)
	{
		DestoryOrder();
		return;
	}

	_deleteTimer->Update();
	if (_deleteTimer->IsTime() && _AnimationComponent->GetCurrentState() != "Destroy")
	{
		_AnimationComponent->PlayAnimation("Destroy");
		return;
	}

}

void NineSideCube::CreateBody()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (y == 1 && x == 1 && z == 1)
				{
					continue;
				}

				auto pos = SimpleMath::Vector3(-2.0f + (2.0f * x), -2.0f + (2.0f * y), -2.0f + (2.0f * z));
				auto targetCube = new NineSideCubeBody(1, _bodyDestoryTime * 0.9f, "RedCube", _pGameManager);
				targetCube->SetPosition(pos);
				targetCube->SetScale(SimpleMath::Vector3::One);
				SetChild(targetCube);
			}
		}
	}
}

void NineSideCube::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("RedClearCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(),GetScale(), "TargetObject");
	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	CreateBody();

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto rotateAnimation = std::make_shared<AnimationCommandList>();
	rotateAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(10, 10, 10), m_EulerRotation, 0.01f));

	auto destroyAnimation = std::make_shared<AnimationCommandList>();
	destroyAnimation->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale, 6.0f));

	_AnimationComponent->AddAnimationState(rotateAnimation, "Rotate", "Rotate");
	_AnimationComponent->AddAnimationState(destroyAnimation, "Destroy",AnimationQue::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("Rotate");
}

void NineSideCube::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

void NineSideCube::OnCollsion(Actor* other)
{
}

void NineSideCube::Rotate(float angle)
{
	auto pPlayer = _pGameManager->GetPlayer();

	if (pPlayer == nullptr)return;



	_angle += Time::DeltaTime * _rotateSpeed * angle;

	auto playerPosition = pPlayer->GetPosition();

	SimpleMath::Vector3 pos = playerPosition + SimpleMath::Vector3(_radius * cos(_angle), 0, _radius * sin(_angle));

	SetPosition(pos);

}

void NineSideCube::ActiveAction(Actor* pPlayer)
{

}
