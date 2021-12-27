﻿#include "TargetCube.h"

#include <Components/Animations/AnimationComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/Vector3AnimationCommand.h>

#include "../GameSystem/GameManager.h"

TargetCube::TargetCube(const std::string& dxrMeshName, GameManager* pGameManager)
	:TargetObject(pGameManager)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_ChangeTargetRotateTimer = std::make_shared<Timer>(3.0f);


}

void TargetCube::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	if (_AnimationComponent->GetCurrentState() == "Generate")
	{
		return;
	}

	if (_isDelete && _AnimationComponent->GetCurrentState() == "End")
	{
		Destroy();
	}

	auto rotate = GetVecRotation();
	SetRotation(SimpleMath::Vector3::Lerp(rotate, _targetRotate, Time::DeltaTime));

	_ChangeTargetRotateTimer->Update();
	if (_ChangeTargetRotateTimer->IsTime())
	{
		const float rand_x = Random::GetRandom(-10.0f, 10.0f);
		const float rand_y = Random::GetRandom(-10.0f, 10.0f);

		_ChangeTargetRotateTimer->Reset();
		_targetRotate = SimpleMath::Vector3(rand_x, rand_y, 0);

	}
}

void TargetCube::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");
	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3::One, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimationCommandList = std::make_shared<AnimationCommandList>();
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::One, SimpleMath::Vector3(2.0f), m_Scale, 4.0f));
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3(2.0f), SimpleMath::Vector3::Zero, m_Scale, 8.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->AddAnimationState(destroyAnimationCommandList, "Destroy", AnimationQue::StandardAnimationStateType::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("Generate");

}

void TargetCube::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

void TargetCube::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		_AnimationComponent->PlayAnimation("Destroy");
		_isDelete = true;
		_pGameManager->AddScore(1000);
		return;
	}
}
