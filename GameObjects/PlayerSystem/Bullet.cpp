#include "Bullet.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Random.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>

Bullet::Bullet(const float moveSpeed, Actor* targetActor)
	:_moveSpeed(moveSpeed),_targetActor(targetActor)
{
	_moveTimer = std::make_shared<Timer>(moveSpeed);
}

void Bullet::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	auto targetPos = SimpleMath::Vector3::Zero;

	if(!_targetActor->GetDestroyFlag())
	{
		targetPos = _targetActor->GetPosition();
	}

	auto pos = GetPosition();

	_moveTimer->Update();
	if(_moveTimer->IsTime())
	{
		Destroy();
		return;
	}

	auto point0 = SimpleMath::Vector3(_createPoint.x + _point0X, _createPoint.y, _createPoint.z);
	auto point1 = SimpleMath::Vector3(targetPos.x + _point1X, targetPos.y, targetPos.z);
	SetPosition(MathUtility::GetBezierCube(_createPoint, targetPos, point0, point1, _moveTimer->GetRatio()));


}

void Bullet::Init()
{
	_sphereCollisionComponent = new SphereCollisionComponent(this, m_Scale.x * 0.5f, "StaticObject");
	CollisionManager::GetInstance().AddComponent(_sphereCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_sphereCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("RoughSphere", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_createPoint = GetPosition();

	_point0X = Random::GetRandom(-12.0f, 12.0f);
	_point1X = Random::GetRandom(-12.0f, 12.0f);

	SetTag("Bullet");
}

void Bullet::Shutdown()
{
	_instance->Destroy();
	_sphereCollisionComponent->Delete();
}

void Bullet::OnCollsion(Actor* other)
{

}
