#include "Tracker.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Components/Physics/RigidBodyComponent.h>

#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>
#include <Utility/Math/MathUtility.h>

Tracker::Tracker(Actor* pPlayer)
	:Actor(),_pPlayer(pPlayer), _moveSpeed(1.0f)
{
	_ChangeTargetRotateTimer = std::make_shared<Timer>(3.0f);
	SetTag("Tracker");
}

void Tracker::AddPoint(std::vector<SimpleMath::Vector3>& points)
{
	_movePoints.insert(_movePoints.end(), points.begin(), points.end());
}


void Tracker::UpdateActor()
{
	Movement();

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	auto rotate = GetVecRotation();
	SetRotation(SimpleMath::Vector3::Lerp(rotate,_targetRotate,Time::DeltaTime));

	_ChangeTargetRotateTimer->Update();
	if(_ChangeTargetRotateTimer->IsTime())
	{
		const float rand_x = Random::GetRandom(-10.0f, 10.0f);
		const float rand_y = Random::GetRandom(-10.0f, 10.0f);

		_ChangeTargetRotateTimer->Reset();
		_targetRotate = SimpleMath::Vector3(rand_x, rand_y, 0);

	}
}

void Tracker::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	m_pCollisionComponent = new SphereCollisionComponent(this,3.0f,"DynamicObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");


	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	
}

void Tracker::Shutdown()
{
	m_pCollisionComponent->Delete();
	_instance->Destroy();
}

void Tracker::OnCollsion(Actor* other)
{
}

void Tracker::Movement()
{
	if (_movePoints.size() <= 0)return;

	auto targetPoint = _movePoints.front();
	auto currentPos = GetPosition();

	_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 2.0f, 1.0f, 24.0f);
	auto setPos = SimpleMath::Vector3(MathUtility::Lerp(currentPos.x, targetPoint.x, Time::DeltaTime * _moveSpeed), MathUtility::Lerp(currentPos.y, targetPoint.y, Time::DeltaTime * _moveSpeed), MathUtility::Lerp(currentPos.z, targetPoint.z, Time::DeltaTime * _moveSpeed));


	SetPosition(setPos);

	if (SimpleMath::Vector3::Distance(currentPos, targetPoint) <= 1.0f)
	{
		_movePoints.erase(_movePoints.begin());
	}
}