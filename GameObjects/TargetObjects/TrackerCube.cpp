#include "TrackerCube.h"

#include <Components/AnimationComponent.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

#include <Components/AnimationComponent.h>
#include <Components/AnimationQue.h>
#include <Components/Vector3AnimationCommand.h>

#include "../GameSystem/GameManager.h"
#include "../PlayerSystem/Player.h"

TrackerCube::TrackerCube(const std::string& dxrMeshName, GameManager* pGameManager)
	:TargetObject(pGameManager), _isDelete(false), _moveSpeed(1.0f)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_ChangeTargetRotateTimer = std::make_shared<Timer>(3.0f);
}

bool TrackerCube::AnimationUpdate()
{
	if (_GenerateAnimationQue->IsPlay())
	{
		return true;
	}

	if (_isDelete && !_DestroyAnimationQue->IsPlay())
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
	return false;
}

void TrackerCube::Move()
{
	auto pos = GetPosition();
	auto target = _pGameManager->GetPlayer();

	if(target == nullptr)
	{
		return;
	}

	auto currentPos = SimpleMath::Vector3::Lerp(pos, target->GetPosition(), Time::DeltaTime * _moveSpeed);
	SetPosition(currentPos);
}

void TrackerCube::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	AnimationUpdate();
	Move();
}

void TrackerCube::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");

	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	_GenerateAnimationQue = std::make_shared<AnimationQue>();
	_GenerateAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3::One, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_DestroyAnimationQue = std::make_shared<AnimationQue>();
	_DestroyAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::One, SimpleMath::Vector3(2.0f), m_Scale, 2.0f));
	_DestroyAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3(2.0f), SimpleMath::Vector3::Zero, m_Scale, 1.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	_AnimationComponent->AddAnimationQue("Generate", _GenerateAnimationQue);
	_AnimationComponent->AddAnimationQue("Destroy", _DestroyAnimationQue);
	_AnimationComponent->PlayAnimation("Generate");
}

void TrackerCube::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

void TrackerCube::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		_AnimationComponent->PlayAnimation("Destroy");
		_isDelete = true;
		_pGameManager->AddScore(1000);
	}
}
