#include "NineSideCube.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "TargetCube.h"

NineSideCube::NineSideCube(GameManager* pGameManager)
	:_pGameManager(pGameManager), _cos(0.0f),_moveSpeed(40.0f), _verticalRange(30.0f)
{
	SetTag("NineSideCube");
}

void NineSideCube::UpdateActor()
{
	Move();
}

void NineSideCube::CreateBody()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (y == 1 && x == 1 && z == 1)
				{
					continue;
				}

				auto pos = SimpleMath::Vector3(-2.0f + (2.0f * x), -2.0f + (2.0f * y), -2.0f + (2.0f * z));
				auto targetCube = new TargetCube(1, "RedCube", _pGameManager);
				targetCube->SetPosition(pos);
				targetCube->SetScale(SimpleMath::Vector3::One);
				SetChild(targetCube);
			}
		}
	}
}

void NineSideCube::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("ClearCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc(0x08);

	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(),GetScale(), "TargetObject");
	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	CreateBody();
}

void NineSideCube::Shutdown()
{
}

void NineSideCube::OnCollsion(Actor* other)
{
}

void NineSideCube::Move()
{
	_cos += Time::DeltaTime;
	auto cos = MathUtility::Cos(_cos);

	auto pos = GetPosition();
	pos += SimpleMath::Vector3::Left * cos * Time::DeltaTime * _verticalRange;
	pos += SimpleMath::Vector3::Backward * _moveSpeed * Time::DeltaTime;

	SetPosition(pos);
}
