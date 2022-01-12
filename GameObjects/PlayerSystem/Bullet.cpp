#include "Bullet.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include <Components/Animations/AnimationCommand.h>
#include <Components/Animations/AnimationComponent.h>
#include <Components/Animations/AnimationQue.h>
#include <Components/Animations/Vector3AnimationCommand.h>

#include "../TargetObjects/TargetObject.h"

Bullet::Bullet(const float moveSpeed, TargetObject* targetActor)
	:_moveSpeed(0.0f), _addmoveSpeed(moveSpeed), _targetActor(targetActor), _moveTime(0.0f), _rotate(0.0f),
	_isTargetLost(false)
{

}

void Bullet::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	//if (_AnimationComponent->GetCurrentState() == "Generate")return;


	if(_targetActor->IsDelete())
	{
		_isTargetLost = true;
 		Destroy();
		return;
	}

	_addRotate = std::clamp(_addRotate + 16.0f * Time::DeltaTime, 0.0f, 16.0f);
	_rotate += Time::DeltaTime * _addRotate;

	//auto targetPos = SimpleMath::Vector3::Zero;

	//if (!_isTargetLost)
	//{
	//	targetPos = _targetActor->GetPosition();
	//}

	auto pos = GetPosition();

	pos += _moveVec * Time::DeltaTime * _moveSpeed;

	//_moveSpeed += _addmoveSpeed * Time::DeltaTime;
	//_moveTime += Time::DeltaTime * _moveSpeed;


	_moveSpeed = 120.0f;

	SetRotation(GetEulerRotation() + SimpleMath::Vector3(0, 0, _rotate));

	//if (_moveTime >= 2.0f)
	//{
	//	Destroy();
	//	return;
	//}

	//auto point0 = SimpleMath::Vector3(_createPoint.x + _point0X, _createPoint.y, _createPoint.z);
	//auto point1 = SimpleMath::Vector3(targetPos.x + _point1X, targetPos.y, targetPos.z);
	//SetPosition(MathUtility::GetBezierCube(_createPoint, targetPos, point0, point1, std::clamp(_moveTime, 0.0f, 1.0f)));

	SetPosition(pos);

}

void Bullet::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), SimpleMath::Vector3(0.4f), "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("BlackCube", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_createPoint = GetPosition();

	if(_targetActor->IsDelete())
	{
		Destroy();
		return;
	}

	_moveVec = _targetActor->GetPosition() - GetPosition();
	_moveVec.Normalize();

	_point0X = Random::GetRandom(-12.0f, 12.0f);
	_point1X = Random::GetRandom(-12.0f, 12.0f);

	SetTag("PlayerBullet");

	//_AnimationComponent = std::make_shared<AnimationComponent>(this);
	//AddComponent(_AnimationComponent);

	//auto generateAnimationCommandList = std::make_shared<AnimationCommandList>();
	//generateAnimationCommandList->AddAnimation(std::make_shared<Vector3AnimationCommand>(SimpleMath::Vector3::Zero, SimpleMath::Vector3(0.4f), m_Scale, 6.0f, AnimationCommand::AnimationSpeedType::AnimationSpeedType_InCubic));
	//_AnimationComponent->AddAnimationState(generateAnimationCommandList, "Generate", AnimationQue::StandardAnimationStateType::AnimationStateType_None);
	//_AnimationComponent->PlayAnimation("Generate");
}

void Bullet::Shutdown()
{
	_instance->Destroy();
	_pCollisionComponent->Delete();
}

void Bullet::OnCollsion(Actor* other)
{
	if (other->IsContainsTag("Player") || other->IsContainsTag("LockOnArea"))return;

	Destroy();
}
