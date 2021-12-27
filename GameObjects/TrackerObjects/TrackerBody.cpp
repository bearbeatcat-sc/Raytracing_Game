#include "TrackerBody.h"

#include <algorithm>
#include <Components/Animations/AnimationCommand.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationQue.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include "../GameSystem/GameManager.h"

TrackerBody::TrackerBody(const int maxHP, const std::string& dxrMeshName, GameManager* pGameManager)
	:_hp(maxHP), TargetObject(pGameManager), _maxHP(maxHP)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void TrackerBody::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	if (_isDelete && _AnimationComponent->GetCurrentState() == "End")
	{
		Destroy();
	}
}

void TrackerBody::Init()
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

	auto damageAnimationCommandList = std::make_shared<AnimationCommandList>();
	damageAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(m_Scale, _damageScale, m_Scale, 4.0f));
	//damageAnimationQue->AddAnimation(std::make_shared<Vector3AnimationCommand>(_damageScale, m_Scale, m_Scale, 8.0f));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList,"Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);

	_AnimationComponent->AddAnimationState(destroyAnimationCommandList, "Destroy", AnimationQue::StandardAnimationStateType::AnimationStateType_End);
	_AnimationComponent->AddAnimationState(damageAnimationCommandList,"Damage", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->PlayAnimation("Generate");
}

void TrackerBody::Shutdown()
{
	_instance->Destroy();
	m_pCollisionComponent->Delete();
}

bool TrackerBody::IsDeath()
{
	return _hp <= 0;
}

void TrackerBody::Damage()
{
	_hp = std::clamp(_hp - 1, 0, _maxHP);
	_AnimationComponent->PlayAnimation("Damage");

	_damageScale = (SimpleMath::Vector3::One * (_hp / _maxHP));
}

void TrackerBody::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		Damage();

		if(IsDeath())
		{
			_AnimationComponent->PlayAnimation("Destroy");
			_isDelete = true;
			_pGameManager->AddScore(1000);
			return;
		}
	}
}
