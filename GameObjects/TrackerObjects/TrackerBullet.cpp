#include "TrackerBullet.h"

#include <Components/Animations/AnimationCommand.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/AnimationQue.h>

#include "../GameSystem/GameManager.h"

TrackerBullet::TrackerBullet(const float moveSpeed, Actor* targetActor,GameManager* pGameManager, float point0x, float point1x)
	:TargetObject(pGameManager),_moveSpeed(moveSpeed), _targetActor(targetActor), _moveTime(0.0f),_point0X(point0x),_point1X(point1x),
	_pGameManager(pGameManager),_isDelete(false),_rotate(0.0f)
{

}



void TrackerBullet::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_rotate += Time::DeltaTime * 16.0f;

	if (_AnimationComponent->GetCurrentState() == "Generate")
	{
		return;
	}

	if (_isDelete && _AnimationComponent->GetCurrentState() == "End")
	{
		Destroy();
	}

	if (_isDelete)return;

	auto targetPos = SimpleMath::Vector3::Zero;

	if (!_targetActor->GetDestroyFlag())
	{
		targetPos = _targetActor->GetPosition();
	}

	auto pos = GetPosition();

	//_moveSpeed += Time::DeltaTime * 0.01f;
	_moveTime += Time::DeltaTime * _moveSpeed;

	SetRotation(GetRotation() + SimpleMath::Vector3(0, 0, _rotate));

	if (_moveTime >= 2.0f)
	{
		Destroy();
		return;
	}

	auto point0 = SimpleMath::Vector3(_createPoint.x + _point0X, _createPoint.y + _point0X, _createPoint.z);
	auto point1 = SimpleMath::Vector3(targetPos.x + _point1X, targetPos.y, targetPos.z);
	SetPosition(MathUtility::GetBezierCube(_createPoint, targetPos, point0, point1, std::clamp(_moveTime,0.0f,1.0f)));


}

void TrackerBullet::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);


	_AnimationComponent = std::make_shared<AnimationComponent>(this);
	AddComponent(_AnimationComponent);

	auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, GetScale(), m_Scale, 2.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));

	auto destroyAnimationCommandList = std::make_shared<AnimationCommandList>();
	destroyAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(GetScale(), SimpleMath::Vector3::Zero, m_Scale, 6.0f));

	_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	_AnimationComponent->AddAnimationState(destroyAnimationCommandList, "Destroy", AnimationQue::StandardAnimationStateType::AnimationStateType_End);
	_AnimationComponent->PlayAnimation("Generate");

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_createPoint = GetPosition();

	SetTag("Target");
	SetActorName("TrackerBullet");
}

void TrackerBullet::Shutdown()
{
	_instance->Destroy();
	_pCollisionComponent->Delete();
}

void TrackerBullet::OnCollsion(Actor* other)
{
	if (_isDelete)return;

	if (other->IsContainsTag("Bullet"))
	{
		_AnimationComponent->PlayAnimation("Destroy");
		_isDelete = true;
		_pGameManager->AddScore(1000);
		return;
	}

	if(other->IsContainsTag("Player"))
	{
		_AnimationComponent->PlayAnimation("Destroy");
		_isDelete = true;
		return;
	}
}
