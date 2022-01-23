#include "BombBullet.h"

#include <Components/Animations/AnimationCommandList.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Random.h>
#include <Utility/Time.h>

#include <Components/Animations/AnimationQue.h>
#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>

#include "BombArea.h"
#include "../BreakEffect.h"


BombBullet::BombBullet(const float moveSpeed, const SimpleMath::Vector3& vec)
:_moveSpeed(120.0f), _addmoveSpeed(moveSpeed), _moveVec(vec), _moveTime(0.0f), _rotate(0.0f)
{
}

void BombBullet::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	_addRotate = std::clamp(_addRotate + 16.0f * Time::DeltaTime, 0.0f, 16.0f);
	_rotate += Time::DeltaTime * _addRotate;

	auto pos = GetPosition();

	pos += _moveVec * Time::DeltaTime * _moveSpeed;
	_moveVec += SimpleMath::Vector3(0, -0.2f, 0) * Time::DeltaTime;

	SetRotation(GetEulerRotation() + SimpleMath::Vector3(0, 0, _rotate));
	SetPosition(pos);

	if(DirectXInput::GetInstance().IsKeyDown(DIK_B))
	{
		Destroy();
		Explode();
	}

}

void BombBullet:: Explode()
{
	auto bombArea = new BombArea(8.0f);
	bombArea->SetPosition(GetPosition());
	ActorManager::GetInstance().AddActor(bombArea);

	for (int i = 0; i < 120; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		//float y = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		float cos = std::cosf(i * 1.0f);
		float sin = std::sinf(i * 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(cos, 0, sin) * 40.0f, "ClearCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.1f) * 1.0f);
		breakEffect->SetRotation(SimpleMath::Vector3(x, 0, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}

	for (int i = 0; i < 120; ++i)
	{
		float x = Random::GetRandom(-1.0f, 1.0f);
		float z = Random::GetRandom(-1.0f, 1.0f);

		float cos = std::cosf(i * 1.0f);
		float sin = std::sinf(i * 1.0f);

		auto breakEffect = new BreakEffect(SimpleMath::Vector3(0, cos, sin) * 40.0f, "ClearCube");
		breakEffect->SetPosition(GetPosition());
		breakEffect->Destroy(4.0f);
		breakEffect->SetScale(SimpleMath::Vector3(0.1f) * 1.0f);
		breakEffect->SetRotation(SimpleMath::Vector3(x, 0, z));
		ActorManager::GetInstance().AddActor(breakEffect);
	}
}

void BombBullet::Init()
{
	_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), SimpleMath::Vector3(0.4f), "PlayerObject");
	CollisionManager::GetInstance().AddComponent(_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_pCollisionComponent);

	_instance = DXRPipeLine::GetInstance().AddInstance("ClearSphere", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	SetTag("PlayerBullet");

}

void BombBullet::Shutdown()
{
	_instance->Destroy();
	_pCollisionComponent->Delete();
}

void BombBullet::OnCollsion(Actor* other)
{
	if (other->IsContainsTag("Player") || other->IsContainsTag("LockOnArea"))return;

	Destroy();
	Explode();
}
