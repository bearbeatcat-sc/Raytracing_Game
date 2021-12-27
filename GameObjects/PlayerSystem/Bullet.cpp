#include "Bullet.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>

Bullet::Bullet(const float moveSpeed, Actor* targetActor)
	:_moveSpeed(moveSpeed),_targetActor(targetActor), _moveTime(0.0f),_rotate(0.0f)
{
}

void Bullet::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_rotate += Time::DeltaTime * 16.0f;

	auto targetPos = SimpleMath::Vector3::Zero;

	if(!_targetActor->GetDestroyFlag())
	{
		targetPos = _targetActor->GetPosition();
	}

	auto pos = GetPosition();

	_moveSpeed += Time::DeltaTime;
	_moveTime += Time::DeltaTime * _moveSpeed;
	SetRotation(GetRotation() + SimpleMath::Vector3(0, 0, _rotate));

	if (_moveTime >= 2.0f)
	{
		Destroy();
		return;
	}

	auto point0 = SimpleMath::Vector3(_createPoint.x + _point0X, _createPoint.y, _createPoint.z);
	auto point1 = SimpleMath::Vector3(targetPos.x + _point1X, targetPos.y, targetPos.z);
	SetPosition(MathUtility::GetBezierCube(_createPoint, targetPos, point0, point1, std::clamp(_moveTime, 0.0f, 1.0f)));


}

void Bullet::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("GrayCube", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_createPoint = GetPosition();

	_point0X = Random::GetRandom(-12.0f, 12.0f);
	_point1X = Random::GetRandom(-12.0f, 12.0f);

	SetTag("PlayerBullet");
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
