#include "Tracker.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

#include "TrackerBody.h"
#include "TrackerShotObject.h"
#include "../Cube.h"

Tracker::Tracker(Actor* pPlayer, GameManager* pGameManager)
	:Actor(), _pPlayer(pPlayer), _moveSpeed(1.0f),_pGameManager(pGameManager)
{
	_ChangeTargetRotateTimer = std::make_shared<Timer>(3.0f);
	_ShootCoolTimer = std::make_shared<Timer>(6.0f);
	SetTag("Tracker");
}

void Tracker::AddPoint(std::vector<SimpleMath::Vector3>& points)
{
	if (_movePoints.size() > 1)
	{
		const SimpleMath::Vector3 connectionPoint = _movePoints.back();


		for (int i = 0; i < 100; ++i)
		{
			_movePoints.push_back(SimpleMath::Vector3::Lerp(connectionPoint, *(points.begin() + 20), 0.01f * i));
		}

		points.erase(points.begin(), points.begin() + 20);
	}



	_movePoints.insert(_movePoints.end(), points.begin(), points.end());
}

void Tracker::Attack()
{
	_ShootCoolTimer->Update();

	if(_ShootCoolTimer->IsTime())
	{
		_ShootCoolTimer->Reset();

		auto shotObject = new TrackerShotObject(_pPlayer, _pGameManager,0.01f * _moveSpeed);
		shotObject->SetPosition(SimpleMath::Vector3::Zero);
		SetChild(shotObject);
	}
}


void Tracker::UpdateActor()
{
	Movement();
	Attack();

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

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

void Tracker::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	m_pCollisionComponent = new SphereCollisionComponent(this, 3.0f, "DynamicObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");


	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	for(int x = 0; x < 3; x++)
	{
		for(int z = 0; z <3; z++)
		{
			for(int y = 0; y < 3; y++)
			{
				if(y == 1 && x == 1 && z == 1)
				{
					continue;
				}

				auto pos = SimpleMath::Vector3(-2.0f + (2.0f * x), -2.0f + (2.0f * y), -2.0f + (2.0f * z));
				auto targetCube = new TrackerBody(30,"WhiteCube",_pGameManager);
				targetCube->SetPosition(pos);
				targetCube->SetScale(SimpleMath::Vector3::One);
				SetChild(targetCube);
			}
		}
	}

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
	//if (_movePoints.size() <= 0)return;

	//auto targetPoint = _movePoints.front();
	//auto currentPos = GetPosition();

	//auto vec = (targetPoint - currentPos);
	//const float distance = vec.Length();

	//_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 4.0f, 1.0f, 8.0f);
	//auto setPos = SimpleMath::Vector3::Lerp(currentPos, targetPoint, Time::DeltaTime * _moveSpeed);


	//SetPosition(setPos);

	//if (SimpleMath::Vector3::Distance(currentPos, targetPoint) <= 1.0f)
	//{
	//	_movePoints.erase(_movePoints.begin());
	//}

	_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 4.0f, 1.0f, _maxMoveSpeed);
	_moveSpeed = _maxMoveSpeed;
	auto setPos = GetPosition() + (SimpleMath::Vector3::Backward * _moveSpeed * Time::DeltaTime);
	SetPosition(setPos);
}