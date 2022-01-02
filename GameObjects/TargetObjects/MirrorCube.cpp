#include "MirrorCube.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>

MirrorCube::MirrorCube(GameManager* pGameManager, const SimpleMath::Vector3& addScale, const SimpleMath::Vector3& maxScale)
	:TargetObject(pGameManager), _addScale(addScale), _maxScale(maxScale),
	_isKnockBack(false), _knockBackAmount(40.0f)
{
	_instance = DXRPipeLine::GetInstance().AddInstance("WhiteCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void MirrorCube::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
}

void MirrorCube::Init()
{
	SetTag("Target");
	SetActorName("MirrorCube");


	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "TargetObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);


	_pAnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_pAnimationComponent);

	auto knockback = std::make_shared<AnimationCommandList>();
	_pKnockBackAnimationCommand = std::make_shared<Vector3AnimationCommand>(GetPosition(), GetPosition() + GetBackward() * _knockBackAmount, m_Position,8.0f);
	_pExpandkAnimationCommand = std::make_shared<Vector3AnimationCommand>(GetScale(),GetScale(),m_Scale,4.0f);
	knockback->AddAnimation(_pKnockBackAnimationCommand);
	knockback->AddAnimation(_pExpandkAnimationCommand);

	_pAnimationComponent->AddAnimationState(knockback, "Hit", AnimationQue::AnimationStateType_None);
}

void MirrorCube::Shutdown()
{
}

void MirrorCube::OnCollsion(Actor* other)
{
	if(other->IsContainsTag("Bullet"))
	{
		Hit();
	}
}

void MirrorCube::Expand()
{
	auto scale = GetScale() + _addScale;
	scale.Clamp(GetScale(), _maxScale);

	_pExpandkAnimationCommand->_start = GetScale();
	_pExpandkAnimationCommand->_target = scale;
}

void MirrorCube::KnockBack()
{
	_pKnockBackAnimationCommand->_start = GetPosition();
	_pKnockBackAnimationCommand->_target = GetPosition() + SimpleMath::Vector3::Backward * _knockBackAmount;



	_pAnimationComponent->PlayAnimation("Hit");
}

void MirrorCube::Hit()
{
	KnockBack();
	Expand();
}
